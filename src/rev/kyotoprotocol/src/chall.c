#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

struct sigaction sigact;
static void signal_handler(int);

FILE *file;
int fd;
char *map;

const char b_name[] = "#!/usr/bin/env bash\n./chall\n";
const char b_len = 28;
const int line_length = 202;
unsigned int line_no;
unsigned int last_pos;

int write_line(char *str, int pos) {
  char buffer[line_length];
  memset(buffer, ' ', line_length);
  memcpy(buffer, str, strlen(str));
  buffer[line_length - 1] = '\0';
  buffer[line_length - 2] = '\n';
  int suc = fseek(file, (pos - 1) * (line_length - 1) + b_len, SEEK_SET);
  if (suc != 0) {
    return -1;
  }
  fwrite(buffer, sizeof(char), line_length - 1, file);
  return 0;
}

int write_buffer(char *buffer, int pos, int length) {
  int suc = fseek(file, (pos - 1) * (line_length - 1) + b_len, SEEK_SET);
  if (suc != 0) {
    return -1;
  }
  fwrite(buffer, sizeof(char), length, file);
  return 0;
}

void clear_before_n(unsigned int line, int n) {
  int limit = line - 1 < n ? 1 : line - n;
  char* buffer = malloc( (line_length - 1) * (line - limit + 1) );
  memset(buffer, ' ', (line_length - 1) * (line - limit + 1));
  for (int i = 0; i < line - limit + 1; i++) {
    buffer[(i + 1) * (line_length - 1) - 1] = '\n';
  }
  write_buffer(buffer, limit, (line - limit + 1) * (line_length - 1));
  free(buffer);
}


void clear_before(unsigned int line) {
  clear_before_n(line, 3);
}

void revert_file() {
  file = freopen("chall.sh", "w", file);
  if (file == NULL) {
    return;
  }
  fwrite(b_name, sizeof(char), b_len, file);
}


void init_signals(void){
    sigact.sa_handler = signal_handler;
    sigemptyset(&sigact.sa_mask);
    sigact.sa_flags = 0;
    sigaction(SIGINT, &sigact, (struct sigaction *)NULL);
}


void leave(int code) {
  close(fd);
  exit(code);
}

static void signal_handler(int sig){
    if (sig == SIGINT) {
      revert_file();
      leave(0);
    }
}



void do_op(int argc, char** argv);


int main(int argc, char *argv[]) {
  init_signals();
  file = fopen("chall.sh", "r+");
  if (file == NULL) {
    printf("Usage: bash ./chall.sh\n");
    return -1;
  }

  if (argc < 3) {
    revert_file();
    int parent = getppid();
    char parent_path[50];
    snprintf(parent_path, sizeof(parent_path), "/proc/%d/cmdline", parent);
    FILE *parent_file = fopen(parent_path, "r");
    if (parent_file == NULL) {
      printf("Usage: bash ./chall.sh\n");
      return -1;
    }
    char parent_cmdline[200];
    int n_items = fread(parent_cmdline, sizeof(char), sizeof(parent_cmdline) - 1, parent_file);
    for (int i = 0; i < n_items; i++) {
      if (parent_cmdline[i] == '\0') {
        parent_cmdline[i] = ' ';
      }
    }
    fclose(parent_file);
    if (strstr(parent_cmdline, "bash") == NULL ||
        strstr(parent_cmdline, "chall.sh") == NULL) {
      printf("Usage: bash ./chall.sh\n");
      return -1;
    }
  }

  do_op(argc, argv);
  leave(0);
  return 0;

}