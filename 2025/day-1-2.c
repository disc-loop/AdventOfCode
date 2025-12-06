#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 6
#define DIAL_START 50
#define DIAL_END 100

int valid(char buf[BUFFER_SIZE]);
int rotate(int, int);
int count_zeroes(int, int);

int main (int argc, char **argv) {
  char *filename;
  FILE *file;
  char buf[BUFFER_SIZE];
  int linesRead = 0;
  int zeros = 0;
  int dial = DIAL_START;
  int rotations = 0;

  if (argc != 2) {
    fprintf(stderr, "Expected 1 arg\n");
    return EXIT_FAILURE;
  }
  filename = argv[1];

  file = fopen(filename, "r");
  if (file == NULL) {
    fprintf(stderr, "Unable to open file '%s'\n", filename);
    return EXIT_FAILURE;
  }

  while (fgets(buf, BUFFER_SIZE, file) != NULL) {
    linesRead++;
    printf("Dial: %d\n", dial);

    if (!valid(buf)) {
      fprintf(stderr, "Invalid command on line %d: '%s'\n", linesRead, buf);
      return EXIT_FAILURE;
    }
    rotations = buf[0] == 'L' ? -atoi(&buf[1]) : atoi(&buf[1]);

    printf("Command: %s", buf);
    zeros += 1 * (abs(rotations) / DIAL_END);
    if (dial != 0 && (dial + (rotations % DIAL_END) < 1 || dial + (rotations % DIAL_END) > DIAL_END - 1)) {
      zeros++;
    }
    dial = rotate(dial, rotations);
    dial = (dial + rotations) % DIAL_END;
    if (dial < 0) {
      dial = DIAL_END + dial;
    }
    printf("Zeros: %d\n\n", zeros);
  }
  fclose(file);

  printf("Answer: %d\n", zeros);

  return EXIT_SUCCESS;
}

int valid(char buf[BUFFER_SIZE]) {
  return (buf[0] == 'R' || buf[0] == 'L') && (
           (isdigit(buf[1]) && buf[2] == '\n') ||
           (isdigit(buf[2]) && buf[3] == '\n') ||
           (isdigit(buf[3]) && buf[4] == '\n')
         );
}
