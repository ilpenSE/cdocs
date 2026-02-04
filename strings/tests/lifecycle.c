#include <stdio.h>
#include "../str.h"
#include "prettyp.h"

void print_str(const string* s) {
  pprint("data: \"%s\"", s->data);
  pprint("count: %zu", s->count);
  pprint("capacity: %zu", s->capacity);
  printf("==================\n");
}

/*
  EXPECTED OUTPUT:
lifecycle.c:7: data: "selam"
lifecycle.c:8: count: 5
lifecycle.c:9: capacity: 22
==================
lifecycle.c:7: data: ""
lifecycle.c:8: count: 0
lifecycle.c:9: capacity: 22
==================
lifecycle.c:7: data: "(null)"
lifecycle.c:8: count: 0
lifecycle.c:9: capacity: 0
==================
 */
int main() {
  string s = str_new("selam");
  print_str(&s);
  
  str_clear(&s);
  print_str(&s);
  
  str_free(&s);
  print_str(&s);
  return 0;
}
