#include <ctype.h>
#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define BUF_SIZE 256

int f_read_to(char[BUF_SIZE], FILE*, char);

typedef struct {
  u_int64_t start;
  u_int64_t end;
} Range;

int init_range(Range*, char[BUF_SIZE]);

int is_odd(u_int64_t);

int is_invalid(u_int64_t);

int main(int argc, char **argv) {
  u_int64_t total = 0;
  FILE *file;
  char *filename;
  char buf[BUF_SIZE];
  int chars_read = 0;
  Range range;
  u_int64_t i;

  if (argc != 2) {
    fprintf(stderr, "Expected 1 argument.\n");
    return EXIT_FAILURE;
  }
  filename = argv[1];

  file = fopen(filename, "r");
  if (!file) {
    fprintf(stderr, "Unable to open file '%s'\n", filename);
    return EXIT_FAILURE;
  }

  while ((chars_read = f_read_to(buf, file, ',')) != 0) {
    if (init_range(&range, buf) != 0) {
      fprintf(stderr, "Failed to initialise range for '%s'\n", buf);
      return EXIT_FAILURE;
    }
    for (i = range.start; i < range.end+1; i++) {
      if (is_invalid(i)) {
        printf("Invalid ID: %llu\n", i);
        total += i;
      }
    }
  }
  fclose(file);

  printf("Answer: %llu\n", total);

  return EXIT_SUCCESS;
}

int f_read_to(char buf[BUF_SIZE], FILE *file, char delim) {
  char ch;
  int i = 0;

  while ((ch = fgetc(file)) != EOF && ch != delim) {
    buf[i++] = ch;
  }
  buf[i] = '\0';

  return i;
}

int init_range(Range *r, char buf[BUF_SIZE]) {
  char start[BUF_SIZE], end[BUF_SIZE];
  int i = 0, len = 0;

  len = strlen(buf);

  for (i = 0; i < len; i++) {
    if (buf[i] == '\0') {
      return -1;
    }
    if (buf[i] == '-') {
      start[i] = '\0';
      break;
    }
    if (!isdigit(buf[i])) {
      return -1;
    }
    start[i] = buf[i];
  }

  while (i < len) {
    i++;
    if (buf[i] == '\0') {
      end[i-strlen(start)-1] = '\0';
      break;
    }
    if (!isdigit(buf[i])) {
      return -1;
    }
    end[i-strlen(start)-1] = buf[i];
  }

  r->start = (u_int64_t)strtoumax(start, NULL, 10);
  r->end = (u_int64_t)strtoumax(end, NULL, 10);

  return 0;
}

int is_odd(u_int64_t i) {
  return i & 1;
}

int is_invalid(u_int64_t id) {
  int i, len;
  char str[BUF_SIZE];

  sprintf(str, "%llu", id);
  len = strlen(str);
  if (is_odd(len)) {
    return 0;
  }

  for (i = 0; i < len/2; i++) {
    if (str[i] != str[i+len/2]) {
      return 0;
    }
  }

  return 1;
}
