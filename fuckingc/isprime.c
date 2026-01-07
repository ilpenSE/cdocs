#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int is_prime_classic(int n) {
  if (n <= 1) return 0; // 0 and 1 are not prime numbers
  for (int i = 2; i * i <= n; i++) {
    if (n % i == 0) return 0; // Found a divisor, not prime
  }
  return 1; // No divisors found, it's prime
}

int main(int argc, char *argv[]) {
  int n = 0;

  printf("Enter a number: ");
  scanf("%d", &n);
  printf("\n");

  clock_t start, end;
  double time_taken;
  start = clock();
  for (int i = 0; i < 1; i++) {
    is_prime_classic(n);
  }
  end = clock();
  time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
  printf("Time taken using classic method: %f ms\n", time_taken*1000);
}