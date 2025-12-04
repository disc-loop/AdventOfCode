#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 6
#define DIAL_START 50
#define DIAL_END 100

char *filename;
FILE *file;
char buf[BUFFER_SIZE];
int linesRead = 0;
int zeros = 0;
int dial = DIAL_START;

int valid(char buf[BUFFER_SIZE]);

int main (int argc, char **argv) {
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

    if (!valid(buf)) {
      fprintf(stderr, "Invalid command on line %d\n", linesRead);
    }

    if (buf[0] == 'L') {
      printf("Command: L%d\n", atoi(&buf[1]));
      dial = (dial - atoi(&buf[1])) % DIAL_END;
      if (dial < 0) {
        dial = DIAL_END + dial;
      }
    } else {
      printf("Command: R%d\n", atoi(&buf[1]));
      dial = (dial + atoi(&buf[1])) % DIAL_END;
    }
    printf("Dial: %d\n", dial);
    if (dial == 0) {
      zeros++;
    }
  }
  fclose(file);

  printf("Answer: %d\n", zeros);

  return EXIT_SUCCESS;
}

int valid(char buf[BUFFER_SIZE]) {
  return (buf[0] == 'R' || buf[0] == 'L') &&
         isdigit(buf[1]) && (
          (isdigit(buf[2]) && buf[3] == '\n') ||
          (isdigit(buf[3]) && buf[4] == '\n')
         );
}
