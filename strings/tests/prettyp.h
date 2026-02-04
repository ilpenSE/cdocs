#ifndef PRETTY_PRINT_H
#define PRETTY_PRINT_H

#include <stdio.h>

#define COLOR_RED     "\033[0;31m"
#define COLOR_GREEN   "\033[0;32m"
#define COLOR_YELLOW  "\033[0;33m"
#define COLOR_BLUE    "\033[0;34m"
#define COLOR_RESET   "\033[0;0m"

#define pprint(fmt, ...) do { \
    printf(COLOR_BLUE "%s:%d: " COLOR_RESET \
           fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__);  \
  } while (0)

#define pperror(fmt, ...) do { \
    printf(COLOR_RED "[ERROR]" COLOR_BLUE "%s:%d: " COLOR_RESET \
           fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__);  \
  } while (0)

#endif // PRETTY_PRINT_H
