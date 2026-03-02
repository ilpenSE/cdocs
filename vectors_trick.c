#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_CAPACITY 256

// all the metadata we need
typedef struct {
  size_t cap;
  size_t len;
} Header;

#define arr_push(arr, x)                                                \
  do {                                                                  \
    Header* header;                                                     \
    /* if array is NULL, instansiate init */                            \
    if (!(arr)) {                                                       \
      header = (Header*)malloc(INITIAL_CAPACITY * sizeof(*(arr)) + sizeof(Header)); \
      if (!header) exit(1);                                             \
      header->cap = INITIAL_CAPACITY;                                   \
      header->len = 0;                                                  \
      arr = (void*)(header + 1); /* this'll implicitly converted to header + sizeof(Header) */ \
    } else {                                                            \
      header = (Header*)arr - 1;                                        \
    }                                                                   \
    /* If we exceeding memory */                                        \
    if (header->len >= header->cap) {                                   \
      header->cap *= 2;                                                 \
      header = realloc(header, header->cap * sizeof(*(arr)) + sizeof(Header)); \
      arr = (void*)(header + 1);                                        \
    }                                                                   \
    (arr)[header->len++] = (x);                                         \
  } while (0)

#define arr_len(arr) (((Header*)(arr) - 1)->len)

#define arr_free(arr) do { free((Header*)(arr) - 1); } while (0)

int main() {
  int* arr = NULL;
  arr_push(arr, 1);
  arr_push(arr, 2);
  arr_push(arr, 3);
  arr_push(arr, 4);

  for (size_t i = 0; i < arr_len(arr); i++) printf("%d\n", arr[i]);

  arr_free(arr);
  return 0;
}
