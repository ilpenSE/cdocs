#include <stdio.h>

int main() {
  int x = 5;

  // >> operator: floor(x/2)
  int y = x >> 1;
  printf("y: %d\n", y); // should be 2 (floor of 2.5)

  // << operator: x*2
  int z = x << 1;
  printf("z: %d\n", z); // should be 10

  int isOdd = x & 1; // check if x is odd using & operator
  printf("is x odd: %d\n", isOdd); // should be 1 (true)

  int a = x ^ 3; // bitwise XOR with 3           5     3
  printf("a (x ^ 3): %d\n", a); // should be 6 (101 ^ 011 = 110 = 6)
  // XOR LOGIC TABLE
  // 0 0 -> 0
  // 0 1 -> 1
  // 1 0 -> 1
  // 1 1 -> 0

  int b = ~x; // bitwise NOT, flips all bits
  printf("b (~x): %d\n", b); // should be -6 (5 = 0000...0101, ~5 = 1111...1010 which is -6 in two's complement)

  int c = x | 3; // bitwise OR with 3            5     3
  printf("c (x | 3): %d\n", c); // should be 7 (101 | 011 = 111 = 7)
  return 0;
}