#include <stdio.h>
#include <stdlib.h>
#include "powers.h"

// iterasyon kadar hızlı olmasa da recursive olarak çalışan üs alma fonksiyonu
long long fast_pow(long long base, long long exp) {
  if (exp == 0) return 1;
  if (exp % 2 == 0) {
      long long half = fast_pow(base, exp / 2);
      return half * half;
  } else {
      return base * fast_pow(base, exp - 1);
  }
}

// her durumda en hızlısı budur (base ve/veya exp floating point değilse ama binpow ile çok fark yok)
long long fast_pow_iter(long long base, long long exp) {
  long long result = 1;
  while (exp > 0) {
      if (exp % 2 == 1) {
          result *= base;
      }
      base *= base;
      exp /= 2;
  }
  return result;
}

// a ve b'nin floating point olması durumunda daha hızlı çalışır.
long long binpow(long long a, long long b) {
    long long res = 1;
    while (b > 0) {
        if (b & 1)
            res = res * a;
        a = a * a;
        b >>= 1;
    }
    return res;
}
