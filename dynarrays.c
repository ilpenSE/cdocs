#include <stdio.h>
#include <stdlib.h>

typedef struct {
  int *items;
  size_t count;
  size_t capacity;
} Numbers;

typedef struct {
  float *items;
  size_t count;
  size_t capacity;
} Prices;

typedef struct {
  char* *items;
  size_t count;
  size_t capacity;
} Names;

// assumes xs as NON-NULL pointer (you have to check)
#define da_append(xs, x) do {                                           \
    size_t new_cap = (xs)->capacity;                                    \
    if ((xs)->count >= new_cap) {                                       \
      new_cap = new_cap == 0 ? 64 : new_cap * 2;                        \
      (xs)->items = realloc((xs)->items, new_cap * sizeof(*(xs)->items)); \
    }                                                                   \
    if ((xs)->items) {                                                  \
      (xs)->items[(xs)->count++] = x;                                   \
      (xs)->capacity = new_cap;                                         \
    }                                                                   \
  } while (0)

int main() {
  Numbers nums = {0};
  for (int i = 0; i < 10; ++i) da_append(&nums, i);
  for (size_t i = 0; i < nums.count; ++i) printf("%d\n", nums.items[i]);

  Prices prices = {0};
  for (float i = 0.0f; i < 50.0f; i += 1.0f) da_append(&prices, i);
  for (size_t i = 0; i < prices.count; ++i) printf("%f\n", prices.items[i]);

  Names names = {0};
  da_append(&names, "Hello!");        
  da_append(&names, "World");
  da_append(&names, "Freunde!");
  da_append(&names, "ilpeN");

  for (size_t i = 0; i < names.count; ++i) printf("%s\n", names.items[i]);
  return 0;
}
