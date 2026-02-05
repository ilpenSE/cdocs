#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "powers.h"
#include "timelapse.h"

int strtoint(const char* str) {
  int res = 0;
  for (int i = 0; i < strlen(str); i++) {
    if (str[i] >= '0' && str[i] <= '9') {
      res = res * 10 + (str[i] - '0');
    } else {
      res = 0;
      break;
    }
  }
  return res;
}

int main(int argc, char** argv) {
  char str[21];

  printf("String gir: ");
  scanf("%20s", &str);

  int res = timelapse_int(strtoint, str);
  int resatoi = timelapse_int(atoi, str);

  printf("Parsed integer with my func: %d\n", res);
  printf("Parsed integer with atoi: %d\n", resatoi);

  long long base = 2.76;
  long long exp = 592.311;

  long long fastpower_res = timelapse_powers(fast_pow, base, exp); // özel fonksiyon
  long long fastpoweriter_res = timelapse_powers(fast_pow_iter, base, exp); // özel fonk ama iterasyonlu
  long long binpower_res = timelapse_powers(binpow, base, exp); // özel fonk ama bitwise (floating point için daha hızlı)
  double stdpower_res = timelapse_double(pow, base, exp); // C'deki math.h
  return 0;
}