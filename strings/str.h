#ifndef STR_H
#define STR_H

#include <stddef.h>
#include <stdbool.h>

#if defined(_WIN32) || defined(__CYGWIN__)
  #ifdef STR_BUILD
    #define STR_API __declspec(dllexport)
  #else
    #define STR_API __declspec(dllimport)
  #endif
#else
  #define STR_API __attribute__((visibility("hidden")))
#endif

typedef struct {
  char* data;
  size_t count; // does not include \0
  size_t capacity;
} string;

// assumes xs as NON-NULL pointer (you have to check)
#define da_append(xs, x) do {                                           \
    size_t new_cap = (xs)->capacity;                                    \
    if ((xs)->count >= new_cap) {                                       \
      new_cap = new_cap == 0 ? 64 : new_cap * 2;                        \
      (xs)->data = realloc((xs)->data, new_cap * sizeof(*(xs)->data));  \
    }                                                                   \
    if ((xs)->data) {                                                   \
      (xs)->data[(xs)->count++] = x;                                    \
      (xs)->capacity = new_cap;                                         \
    }                                                                   \
  } while (0)

/*
  Makes and returns string struct from C-strings with length
  If you have len value, use this
  But if you dont have and if you're using strlen, you can use
  str_from_cstr, it does strlen
 */
STR_API string str_newn(const char* buf, size_t len);

/*
  str_newn with no len value, it calculates len via strlen
  If you dont have your string's length value or just dont want
  to use strlen by yourself, you can use this
 */
STR_API string str_new(const char* buf);

/*
  Returns char in that string by a pos value
  pos stands for position (the index, starts from zero)
 */
STR_API char str_idx(const string* s, size_t pos);

/*
  It appends char into that string
  Automatically puts \0 at the end
 */
STR_API bool str_append(string* s, char c);

/*
  str_append's "many" version, it accepts array of chars (C-string)
  and in a while loop, it calls da_append like str_append
  It manipulates buf (increments the pointer)
*/
STR_API bool str_append_many(string* s, char* buf);

/*
  It concatenates dest string and src string
  It puts src string's chars into dest string and
  puts \0 at the end and updates count and capacity (if needed)
 */
STR_API bool str_cat(string* dest, const string* src);

/*
  It gets "data" field in that string
  You can always use s->data or s.data
  But this checks NULL or empty conditions
 */
STR_API char* str_to_cstr(const string* s);

/*
  Calls "free" in stdlib.h and clears the string
  Use this by your hand if you dont want memory leaks
 */
STR_API void str_free(string* s);

/*
  It clears the string, sets count to zero
  and makes data's first char \0, NOT NULL!
  Technically string is clear but it does have trash
  Use this function, this is way faster than zeroing all the data
  Capacity still there and this function DOES NOT FREE THE MEMORY!
 */
STR_API void str_clear(string* s);

/*
  Zeroes whole string buffer
  Uses memset, it is way slower than str_clear
  Use str_clear if you dont have any special use-case
 */
STR_API void str_clear_whole(string* s);

/*
  Nukes whitespaces at the end and begin
 */
STR_API void str_trim(string* s);

/*
  Closes the string if not closed or corrupted
  Inserts \0 at the end (depends on count field)
 */
STR_API void str_close(string* s);

/*
  Checks if string is ended with \0
 */
STR_API bool str_is_closed(const string* s);

/*
  Repeats string with provided count, modifies original string
  It appends that string at the end count - 1 times
  that means if you provide "xx" as string and 4 as count,
  you get: "xxxxxxxx" (=4*"xx")
 */
STR_API void str_repeat(string* s, size_t count);

/*
  Makes all alpha characters in that string lowercased
 */
STR_API void str_tolower(string* s);

/*
  Makes all alpha characters in that string uppercased
*/
STR_API void str_toupper(string* s);

/*
  Checks if all of chars in that string is alpha (uses UTF-8)
 */
STR_API bool str_isalpha(const string* s);

/*
  Checks if all chars in string is alphanumeric (UTF-8 chars and numbers)
 */
STR_API bool str_isalphanum(const string* s);

/*
  Capitalizes string like this:
  hello -> Hello
  heLLo -> Hello
  hell1o -> Hello1o
  Doesn't touch non-alpha chars
  Uppers first char if it is alpha and lowers the rest (alpha ones)
  1hello -> 1hello (because 1 is not alpha)
 */
STR_API void str_capitalize(string* s);

#ifdef STR_IMPLEMENTATION

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

string str_newn(const char* buf, size_t len) {
  string r = {0};
  if (!buf) return r;

  r.capacity = len + 1 + 16; // this 16 is for pre-allocation
  r.data = (char*)malloc(r.capacity);
  if (!r.data) return r;

  memcpy(r.data, buf, len);
  r.count = len;
  
  r.data[r.count] = '\0';
  return r;
}

string str_new(const char* buf) {
  return str_newn(buf, strlen(buf));
}

void str_free(string* s) {
  if (!s || !s->data) return;
  free(s->data);
  s->data = NULL;
  s->count = 0;
  s->capacity = 0;
}

void str_clear(string *s) {
  s->count = 0;
  if (s && s->data && s->data[0] != '\0') {
    s->data[0] = '\0';
  }
}

void str_clear_whole(string *s) {
  if (!s || !s->data || s->count == 0) {
    memset(s->data, 0, s->count);
  }
  s->count = 0;
}

char str_get(const string* s, size_t pos) {
  if (!s || !s->data) return '\0';
  if (pos >= s->count) return '\0';
  return s->data[pos];
}

bool str_append(string* s, char c) {
  if (!s) return false;
  da_append(s, c);
  return true;
}

bool str_append_many(string* s, char* buf) {
  if (!s || !buf) return false;
  while (*buf != '\0') da_append(s, *buf++);
  return true;
}

bool str_cat(string* s, const string* c) {
  if (!s || !c) return false;
  size_t sum = s->count + c->count;
  if (sum >= s->capacity) {
    // we dont have enough capacity
    size_t new_cap = s->capacity == 0 ? 16 : s->capacity;
    while (sum >= new_cap)
      new_cap *= 2;
    s->capacity = new_cap;
    char* tmp = realloc(s->data, new_cap * sizeof(*s->data));
    if (!tmp) return false;
    else s->data = tmp;
  }
  memcpy(s->data + s->count, c->data, c->count);
  s->count = sum;
  s->data[s->count] = '\0';
  return true;
}

void str_trim(string* s) {
  if (!s || !s->data || s->count == 0) return;

  // nuke the whitespaces at the end
  while (s->count > 0 && isspace((unsigned char)s->data[s->count - 1])) {
    s->count -= 1;
  }
  s->data[s->count] = '\0';
  
  // count how many spaces at the begin
  size_t start = 0;
  while (start < s->count && isspace((unsigned char)s->data[start])) {
    start += 1;
  }

  // move string by "start" characters
  if (start > 0) {
    memmove(s->data, s->data + start, s->count - start + 1);
    s->count -= start;
  }
}

void str_repeat(string* s, size_t multiplier) {
	if (multiplier == 1) return;
	if (multiplier == 0) return;
	size_t new_cnt = s->count * multiplier;
  if (new_cnt >= s->capacity) {
    // we dont have enough capacity
    size_t new_cap = s->capacity == 0 ? 16 : s->capacity;
		
    while (new_cnt >= new_cap)
      new_cap *= 2;
		
    s->capacity = new_cap;
    char* tmp = realloc(s->data, new_cap * sizeof(*s->data));
    if (!tmp) return;
    else s->data = tmp;
  }

	for (size_t i = 1; i < multiplier; ++i) {
		memcpy(s->data + (i * s->count), s->data, s->count);
	}
	
	s->data[new_cnt] = '\0';
  s->count = new_cnt;
}

void str_tolower(string* s) {
  if (!s || !s->data || s->count == 0) return;
  // go char-by char and apply tolower from ctype.h
  for (size_t i = 0; i < s->count; ++i) {
    s->data[i] = (char)tolower((unsigned char)s->data[i]);
  }
}

void str_toupper(string* s) {
  if (!s || !s->data) return;
  // go char-by char and apply toupper from ctype.h
  for (size_t i = 0; i < s->count; ++i) {
    s->data[i] = (char)toupper((unsigned char)s->data[i]);
  }
}

void str_capitalize(string* s) {
  if (!s || !s->data) return;
  for (size_t i = 0; i < s->count; ++i) {
    if (i == 0) {
      s->data[i] = (char)toupper((unsigned char)s->data[i]);
    } else {
      s->data[i] = (char)tolower((unsigned char)s->data[i]);
    }
  }
}

bool str_isalpha(const string* s) {
  if (!s || !s->data) return false;
  for (size_t i = 0; i < s->count; ++i) {
    if (!isalpha((unsigned char)s->data[i])) return false;
  }
  return true;
}

bool str_isalphanum(const string* s) {
  if (!s || !s->data) return false;
  for (size_t i = 0; i < s->count; ++i) {
    bool a = isalpha((unsigned char)s->data[i]);
    bool n = s->data[i] >= '0' && s->data[i] <= '9';
    if (!n && !a) return false;
  }
  return true;
}

void str_close(string* s) {
  if (!s || !s->data) return;
  s->data[s->count] = '\0';
}

bool str_is_closed(const string* s) {
	return s->data[s->count] == '\0' ? true : false;
}

char* str_to_cstr(const string* s) {
  return s->data;
}

#endif // STR_IMPLEMENTATION

#endif // STR_H
