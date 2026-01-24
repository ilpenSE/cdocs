#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


typedef struct {
	char* items;
	size_t count;
	size_t capacity;
} string;

#define da_append(xs, x) \
	do { \
		if (xs.count >= xs.capacity) { \
		  if (xs.capacity == 0) xs.capacity = 32; \
			else xs.capacity *= 2; \
			xs.items = realloc(xs.items, xs.capacity * sizeof(*xs.items));	\
		} \
		xs.items[xs.count++] = x; \
	} while (0)

string str_new(const char* src) {
	string s = {0};
	while (*src != '\0') {
		da_append(s, *src++);
	}
	da_append(s, '\0');
	return s;
}

void str_free(string* s) {
	free(s->items);
	s->items = NULL;
	s->count = 0;
	s->capacity = 0;
}

int main() {
	printf("pid = %d\n", getpid());
	char* buf = "fdsfğdslfdsfdskfdsfodskf";
	string s = str_new(buf);
	printf("items: %s\n", s.items);
	printf("cnt: %zu\n", s.count);
	printf("cap: %zu\n", s.capacity);
	str_free(&s);
	return 0;
}
