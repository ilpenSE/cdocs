#ifndef MYASSERT_H
#define MYASSERT_H

/*
  This is my implementation for assert.h
  It requires min C11
*/

#define MYASSERT(expr) \
  ((expr) ? (void)0 : __vec_assert_fail(#expr, __FILE__, __LINE__, __func__))

extern _Noreturn void __vec_assert_fail(const char *assertion, const char *file,
                                        unsigned int line, const char *function);

#ifdef MYASSERT_IMPLEMENTATION
extern _Noreturn void __myassert_fail(const char *assertion, const char *file,
                                 unsigned int line, const char *function) {
  fprintf(stderr, "%s:%u: %s: "
          "\033[1;31m" "ASSERTION FAILED"
          "\033[0m" ": '"
          "\033[1;36m" "%s"
          "\033[0m" "'\n", file, line, function, assertion);
  abort();
}
#endif // MYASSERT_IMPLEMENTATION

#endif // MYASSERT_H
