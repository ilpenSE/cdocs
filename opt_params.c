#include <stdio.h>
#include <stdbool.h>

typedef struct {
  int x, y;
} Position;

typedef struct {
  bool is_this;
  int a;
  Position pos;
} FuncParams;

/*
  Wrapper of func_impl, func_impl must accept FuncParams for optional parameters
  Mandatory arguments first then optional parameters come.
  You can define default values. If you don't it'll zero-initialize them
  And you don't have to assign a to 0 because integers are zero by default or the position struct
*/
#define func(s, ...) func_impl((s), (FuncParams){ .a = 0, ##__VA_ARGS__ })

// This must accept optional parameters by struct value
void func_impl(const char* str, FuncParams params) {
  if (params.is_this) printf("DEEEZ %s!\n", str);
  else printf("DA %s!\n", str);
  printf("Position: x = %d, y = %d\n", params.pos.x, params.pos.y);
  printf("a = %d\n", params.a);
  printf("=======\n");
}

int main() {
  func("miin"); // everything is automatically zeroed
  func("main", .is_this = true); // pos is automatically zeroed
  func("NUTZ", .is_this = true, .pos = {67, 69});
  func("mein", .pos = {67, 69});
  func("mien", .is_this = false, .a = 10, .pos = {10, 20});
}
