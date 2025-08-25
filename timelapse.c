#include "timelapse.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

int timelapse_int(int (*func)(const char*), const char* str) {
  clock_t start, end;
  double time_taken;

  start = clock();
  int res = func(str);
  for (int i = 0; i < 1e6; i++) {
    func(str);
  }
  
  end = clock();

  time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
  printf("Süre: %f ms\n", time_taken*1000);
  return res;
}

double timelapse_double(double (*func)(double x, double y), double x, double y) {
  clock_t start, end;
  double time_taken;

  start = clock();
  int res = func(x, y);
  for (int i = 0; i < 1e6; i++) {
    func(x, y);
  }
  
  end = clock();

  time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
  printf("Süre: %f ms\n", time_taken*1000);
  return res;
}

long long timelapse_powers(long long (*func)(long long base, long long exp), long long base, long long exp) {
  clock_t start, end;
  double time_taken;

  start = clock();
  long long res = func(base, exp);
  for (int i = 0; i < 1e6; i++) {
    func(base, exp);
  }
  
  end = clock();

  time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
  printf("Süre: %f ms\n", time_taken*1000);
  return res;
}