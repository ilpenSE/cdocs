#include <stdio.h>
#include <string.h>

int stringlength(const char* str) {
  int i = 0;
  while (str[i++]);
  return i-1;
}

void smemcpy(char* dest, const char* src, int n) {
  while (n--) {
    *dest++ = *src++;
  }
}

int main(int argc, char** argv) {
  char* str = "howareyounigga";
  printf("%d\n", stringlength(str));

  char dest[5];
  smemcpy(dest, str, 4);
  printf("Destination string: %s\n", dest);
  printf("Destination string length: %d\n", strlen(dest));
  return 0;
}