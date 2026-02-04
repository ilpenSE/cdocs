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
appends.c:6: data: "kanka"
appends.c:7: count: 5
appends.c:8: capacity: 22
==================
appends.c:6: data: "kanka! "
appends.c:7: count: 7
appends.c:8: capacity: 22
==================
appends.c:6: data: "kanka! hello "
appends.c:7: count: 13
appends.c:8: capacity: 22
==================
appends.c:6: data: "kanka! hello world"
appends.c:7: count: 18
appends.c:8: capacity: 22
==================
appends.c:6: data: "world"
appends.c:7: count: 5
appends.c:8: capacity: 22
==================
 */
int main() {
	string s = str_new("kanka");
	print_str(&s);

	str_append(&s, '!');
	str_append(&s, ' ');
	print_str(&s); // expected: "kanka! "

	str_append_many(&s, "hello ");
	print_str(&s); // expected: "kanka! hello "

	string c = str_new("world");
	str_cat(&s, &c);
	print_str(&s); // expected: "kanka! hello world"
	print_str(&c); // expected: "world"

	str_append_many(&s, "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA");
	print_str(&s);

	string a = str_new("BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB");
	str_cat(&s, &a);
	print_str(&s);
	print_str(&a);

	string b = str_new("xxx");
	str_repeat(&b, 5);
	print_str(&b); // xxxxxxxxxxxxxxx -> "xxx" * 5
	
	str_free(&s);
	str_free(&a);
	str_free(&b);
	str_free(&c);
	return 0;
}
