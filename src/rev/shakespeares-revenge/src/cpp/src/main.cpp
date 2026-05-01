#include "interpreter.hpp"
#include "model.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#ifndef SPL_ENABLE_IO_STYLES
#define SPL_ENABLE_IO_STYLES 0
#endif

namespace {

void print_usage() {
    std::cerr
        << "Usage:\n"
#if SPL_ENABLE_IO_STYLES
        << "  shakespeare_cpp run <file> [--input-style=basic|interactive] [--output-style=basic|verbose|debug]\n"
        << "  shakespeare_cpp <file> [--input-style=basic|interactive] [--output-style=basic|verbose|debug]\n";
#else
    << "  shakespeare_cpp run <file>\n"
    << "  shakespeare_cpp <file>\n";
#endif
}

bool read_file(const std::string& path, std::string& out_contents) {
    std::ifstream file(path);
    if (!file) {
        return false;
    }
    std::ostringstream stream;
    stream << file.rdbuf();
    out_contents = stream.str();
    return true;
}

#if SPL_ENABLE_IO_STYLES
bool parse_option(const std::string& argument, const std::string& key, std::string& out_value) {
    const std::string prefix = "--" + key + "=";
    if (argument.rfind(prefix, 0) == 0) {
        out_value = argument.substr(prefix.size());
        return true;
    }
    return false;
}
#endif

}  // namespace

int main(int argc, char** argv) {
    if (argc < 2) {
        print_usage();
        return 1;
    }

    std::vector<std::string> args;
    for (int i = 1; i < argc; ++i) {
        args.emplace_back(argv[i]);
    }

    std::string input_style = "basic";
    std::string output_style = "basic";

    bool has_run_subcommand = false;
    std::size_t index = 0;
    if (!args.empty() && args[0] == "run") {
        has_run_subcommand = true;
        index = 1;
    }

    if (index >= args.size()) {
        print_usage();
        return 1;
    }

    std::string file_path = args[index++];

    while (index < args.size()) {
#if SPL_ENABLE_IO_STYLES
        std::string value;
        if (parse_option(args[index], "input-style", value)) {
            input_style = value;
        } else if (parse_option(args[index], "output-style", value)) {
            output_style = value;
        } else {
            std::cerr << "Unknown argument: " << args[index] << "\n";
            print_usage();
            return 1;
        }
#else
        if (args[index].rfind("--input-style=", 0) == 0 || args[index].rfind("--output-style=", 0) == 0) {
            std::cerr << "Style options are disabled in this build\n";
        } else {
            std::cerr << "Unknown argument: " << args[index] << "\n";
        }
        print_usage();
        return 1;
#endif
        ++index;
    }

    (void)has_run_subcommand;

    std::string source;
    if (!read_file(file_path, source)) {
        std::cerr << "Failed to read file: " << file_path << "\n";
        return 1;
    }

    Play play;
    std::string parse_error;
    if (!parse_play_source(source, play, parse_error)) {
        std::cerr << "SPL parse error: " << parse_error << "\n";
        return 1;
    }

    try {
        ShakespeareInterpreter interpreter(play, input_style, output_style);
        interpreter.run();
    } catch (const std::exception& ex) {
        std::cerr << "SPL runtime error: " << ex.what() << "\n";
        return 1;
    }

    return 0;
}
