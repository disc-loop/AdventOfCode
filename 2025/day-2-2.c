#include <ctype.h>
#include <inttypes.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define BUF_SIZE 256

int f_read_to(char[BUF_SIZE], FILE*, char);

typedef struct {
  uint64_t start;
  uint64_t end;
} Range;

int init_range(Range*, char[BUF_SIZE]);

int is_odd(uint64_t);

int is_invalid(uint64_t);

int multiple_of(int,int);

int split(char[BUF_SIZE],int,char[BUF_SIZE][BUF_SIZE]);

int segments_equal(char[BUF_SIZE][BUF_SIZE], int);

int main(int argc, char **argv) {
  uint64_t total = 0;
  FILE *file;
  char *filename;
  char buf[BUF_SIZE];
  int chars_read = 0;
  Range range;
  uint64_t i;

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

  while ((ch = fgetc(file)) != EOF && ch != delim && ch != '\n') {
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

  r->start = (uint64_t)strtoumax(start, NULL, 10);
  r->end = (uint64_t)strtoumax(end, NULL, 10);

  return 0;
}

int is_odd(uint64_t i) {
  return i & 1;
}

int is_invalid(uint64_t idNum) {
  char id[BUF_SIZE];
  int patternLength;
  char segments[BUF_SIZE][BUF_SIZE];
  int numberOfSegments;

  sprintf(id, "%llu", idNum);
  for (patternLength = 1; patternLength <= (int)(strlen(id) / 2); patternLength++) {
    if (multiple_of(strlen(id), patternLength)) {
      numberOfSegments = split(id, patternLength, segments);
      if (segments_equal(segments, numberOfSegments)) {
        return 1;
      }
    }
  }
  return 0;
  // for (patternLength = 1; patternLength <= (strlen(id) / 2); patternLength++) {
  //   cooked = 0;
  //   if (strlen(id) % patternlength == 0) {
  //     for (segmentoffset = 0; segmentoffset < patternlength; segmentoffset++) {
  //       comparison = id[segmentoffset];
  //       for (i = 1; i < (strlen(id)/patternlength) + 1; i++) {
  //         if (id[segmentoffset+i*patternlength] != comparison) {
  //           cooked = 1;
  //           break;
  //         }
  //       }
  //       if (cooked) {
  //         break;
  //       }
  //       return 1;
  //     }
  //   }
  // }

  return 0;
}

int multiple_of(int n, int divisor) {
  return n % divisor == 0;
}

int split(char str[BUF_SIZE], int size, char segments[BUF_SIZE][BUF_SIZE]) {
  int offset, i;
  int parts = strlen(str) / size;

  for (offset = 0; offset <= parts; offset++) {
    for (i = 0; i < size; i++) {
      segments[offset][i] = str[i+offset*size];
    }
    segments[offset][size] = '\0';
  }

  return parts;
}

int segments_equal(char segments[BUF_SIZE][BUF_SIZE], int len) {
  int i;

  for (i = 1; i < len; i++) {
    if (strcmp(segments[0], segments[i])) {
      return 0;
    }
  }

  return 1;
}
