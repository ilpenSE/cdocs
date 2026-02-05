#ifndef POWERS_H
#define POWERS_H

// Recursive exponentiation function (less efficient than iterative for large exponents)
long long fast_pow(long long base, long long exp);

// Iterative exponentiation function (generally the fastest, especially for integer exponents/bases)
long long fast_pow_iter(long long base, long long exp);

// Bitwise exponentiation function (efficient for float/double exponents)
long long binpow(long long a, long long b);

#endif // POWERS_H