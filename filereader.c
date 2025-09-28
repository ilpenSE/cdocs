#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
  if (argc < 2) {
    fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
    return 1;
  }

  const char* filename = argv[1];
  FILE* file = fopen(filename, "r");
  if (!file) {
    perror("Failed to open file");
    return 1;
  }

  fseek(file, 0, SEEK_END);
  long filesize = ftell(file);
  fseek(file, 0, SEEK_SET);

  char* buffer = (char*)malloc(filesize + 1);
  if (!buffer) {
    perror("Failed to allocate memory");
    fclose(file);
    return 1;
  }

  size_t readsize = fread(buffer, 1, filesize, file);
  buffer[readsize] = '\0'; // Null-terminate the string
  printf("File content (%ld bytes):\n%s\n", readsize, buffer);
  
  free(buffer);
  fclose(file);
  return 0;
}