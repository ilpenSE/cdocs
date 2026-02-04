#include <stdio.h>
#include "../str.h"
#include "prettyp.h"

void print_str(const string* s) {
  pprint("data: \"%s\"", s->data);
  pprint("count: %zu", s->count);
  pprint("capacity: %zu", s->capacity);
  printf("==================\n");
}

/* EXPECTED OUTPUT:
chars.c:6: data: "NABER"
chars.c:7: count: 5
chars.c:8: capacity: 22
==================
chars.c:6: data: "naber"
chars.c:7: count: 5
chars.c:8: capacity: 22
==================
chars.c:6: data: "naber1"
chars.c:7: count: 6
chars.c:8: capacity: 23
==================
chars.c:6: data: "14 naber1"
chars.c:7: count: 9
chars.c:8: capacity: 26
==================
chars.c:67: is s1 alpha: 1
chars.c:68: is s2 alpha: 1
chars.c:69: is s3 alpha: 0
chars.c:70: is s4 alpha: 0
==================
chars.c:73: is s1 alphanumeric: 1
chars.c:74: is s2 alphanumeric: 1
chars.c:75: is s3 alphanumeric: 1
chars.c:76: is s4 alphanumeric: 0
==================
chars.c:6: data: "Naber"
chars.c:7: count: 5
chars.c:8: capacity: 22
==================
chars.c:6: data: "Naber"
chars.c:7: count: 5
chars.c:8: capacity: 22
==================
chars.c:6: data: "Naber1"
chars.c:7: count: 6
chars.c:8: capacity: 23
==================
chars.c:6: data: "14 naber1"
chars.c:7: count: 9
chars.c:8: capacity: 26
==================
 */
int main() {
  string s1 = str_new("naber");
  string s2 = str_new("NABER");
  string s3 = str_new("NABeR1");
  string s4 = str_new("14 NABeR1");

  str_toupper(&s1);
  str_tolower(&s2);
  str_tolower(&s3);
  str_tolower(&s4);
  
  print_str(&s1);
  print_str(&s2);
  print_str(&s3);
  print_str(&s4);

  pprint("is s1 alpha: %d", str_isalpha(&s1));
  pprint("is s2 alpha: %d", str_isalpha(&s2));
  pprint("is s3 alpha: %d", str_isalpha(&s3));
  pprint("is s4 alpha: %d", str_isalpha(&s4));
  printf("==================\n");

  pprint("is s1 alphanumeric: %d", str_isalphanum(&s1));
  pprint("is s2 alphanumeric: %d", str_isalphanum(&s2));
  pprint("is s3 alphanumeric: %d", str_isalphanum(&s3));
  pprint("is s4 alphanumeric: %d", str_isalphanum(&s4));
  printf("==================\n");
  
  str_capitalize(&s1);
  str_capitalize(&s2);
  str_capitalize(&s3);
  str_capitalize(&s4);

  print_str(&s1);
  print_str(&s2);
  print_str(&s3);
  print_str(&s4);
  
  str_free(&s1);
  str_free(&s2);
  str_free(&s3);
  str_free(&s4);
  return 0;
}
