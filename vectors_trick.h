#ifndef VECTOR_H
#define VECTOR_H

#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define VECTOR_INITIAL_CAPACITY 256

#define HEADER_FIELDS(X)                        \
  X(cap)                                        \
  X(len)                                        \
  X(elem_size)

typedef struct {
#define FIELD(name) size_t name;
  HEADER_FIELDS(FIELD)
#undef FIELD
} Header;

#define vec_header(vec)    ((Header*)(vec) - 1)
#define vec_data(header)   ((void*)(header + 1))
#define vec_len(vec)       (vec_header(vec)->len)
#define vec_cap(vec)       (vec_header(vec)->cap)
#define vec_elem_size(vec) (vec_header(vec)->elem_size)

#define VEC_ASSERT(cond, msg) assert(cond && msg)

#define vec_reserve(vec, extra)                                         \
  do {                                                                  \
    Header* header = vec ? vec_header(vec) : NULL;                      \
    size_t new_cap = (header ? header->cap : 0) + (extra);              \
    if (!header) {                                                      \
      header = malloc(sizeof(Header) + new_cap * sizeof(*(vec)));       \
      if (!header) abort();                                             \
      header->cap = new_cap;                                            \
      header->len = 0;                                                  \
      header->elem_size = sizeof(*(vec));                               \
    } else {                                                            \
      header = realloc(header, sizeof(Header) + new_cap * header->elem_size); \
      if (!header) abort();                                             \
      header->cap = new_cap;                                            \
    }                                                                   \
    vec = (void*)vec_data(header);                                      \
  } while(0)

#define vec_push(vec, x)                                            \
  do {                                                              \
    if (!(vec)) vec_reserve(vec, VECTOR_INITIAL_CAPACITY);          \
    if (vec_len(vec) >= vec_cap(vec))                               \
      vec_reserve(vec, vec_cap(vec));                               \
    (vec)[vec_header(vec)->len++] = (x);                            \
  } while(0)

#define vec_pop(vec)                            \
  do {                                          \
    Header* header = vec_header((vec));         \
    if (header->len > 0) header->len -= 1;      \
  } while (0)

#define vec_shrink_to_fit(vec)                                          \
  do {                                                                  \
    Header* header = vec_header((vec));                                 \
    header->cap = header->len + 1;                                      \
    header = realloc(header, sizeof(Header) + header->cap * sizeof(*(vec))); \
    if (!header) abort();                                               \
    vec = vec_data(header);                                             \
  } while (0)

#define vec_foreach(T, var_name, vec) \
  for (T* var_name = vec; var_name != (vec) + vec_len((vec)); (var_name)++)

#define vec_last(vec) (vec)[vec_len((vec)) - 1]
#define vec_islast(vec, item) (vec_last((vec)) == (item))

#define vec_equals(lhs, rhs) (vec_len(lhs) == vec_len(rhs) && \
    vec_elem_size(lhs) == vec_elem_size(rhs) && \
    memcmp(lhs, rhs, vec_len(lhs) * vec_elem_size(lhs)) == 0)

#define vec_free(vec) do { free(vec_header(vec)); } while (0)

#endif // VECTOR_H
