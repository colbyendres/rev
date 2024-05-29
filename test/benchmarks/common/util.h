// See LICENSE for license details.
// clang-format off
#ifndef __UTIL_H
#define __UTIL_H

extern void setStats(int enable);

#include <stdint.h>
#include <math.h>

#define static_assert(cond) switch(0) { case 0: case !!(long)(cond): ; }
#define assert(x)                                                              \
  do                                                                           \
    if (!(x)) {                                                                \
      asm(".dword 0x00000000");                                                \
    }                                                                          \
  while (0)

static int verify(int n, const volatile int* test, const int* verify)
{
  int i;
  // Unrolled for faster verification
  for (i = 0; i < n/2*2; i+=2)
  {
    int t0 = test[i], t1 = test[i+1];
    int v0 = verify[i], v1 = verify[i+1];
    if (t0 != v0) return i+1;
    if (t1 != v1) return i+2;
  }
  if (n % 2 != 0 && test[n-1] != verify[n-1])
    return n;
  return 0;
}

// Based on NumPy's allClose function
static int verifyDouble(int n, const volatile double* test, const double* verify)
{
  const double atol = 1e-8;
  const double rtol = 1e-5;
  int         i;

  // Unrolled for faster verification
  for( i = 0; i < n / 2 * 2; i += 2 ) {
    int eq1 = fabs( test[i] - verify[i] ) <= atol + rtol * fabs( verify[i] );
    int eq2 = fabs( test[i + 1] - verify[i + 1] ) <= atol + rtol * fabs( verify[i + 1] );
    if( !( eq1 & eq2 ) ) {
      return i + 1 + eq1;
    }
  }

  if( n % 2 == 1 && fabs( test[n - 1] - verify[n - 1] ) > atol + rtol * fabs( verify[n - 1] ) ) {
    return n;
  }

  return 0;
}

static int verifyFloat(int n, const volatile float* test, const float* verify)
{
  const float atol = 1e-8;
  const float rtol = 1e-5;
  int         i;

  for( i = 0; i < n / 2 * 2; i += 2 ) {
    int eq1 = fabsf( test[i] - verify[i] ) <= atol + rtol * fabsf( verify[i] );
    int eq2 = fabsf( test[i + 1] - verify[i + 1] ) <= atol + rtol * fabsf( verify[i + 1] );
    if( !( eq1 & eq2 ) ) {
      return i + 1 + eq1;
    }
  }

  if( n % 2 == 1 && fabsf( test[n - 1] - verify[n - 1] ) > atol + rtol * fabsf( verify[n - 1] ) ) {
    return n;
  }

  return 0;
}

static void __attribute__((noinline)) barrier(int ncores)
{
  static volatile int sense;
  static volatile int count;
  static __thread int threadsense;

  __sync_synchronize();

  threadsense = !threadsense;
  if (__sync_fetch_and_add(&count, 1) == ncores-1)
  {
    count = 0;
    sense = threadsense;
  }
  else while(sense != threadsense)
    ;

  __sync_synchronize();
}

static uint64_t lfsr(uint64_t x)
{
  uint64_t bit = (x ^ (x >> 1)) & 1;
  return (x >> 1) | (bit << 62);
}

static uintptr_t insn_len(uintptr_t pc)
{
  return (*(unsigned short*)pc & 3) ? 4 : 2;
}

#ifdef __riscv
#include "encoding.h"
#endif

#define stringify_1(s) #s
#define stringify(s) stringify_1(s)
#define stats(code, iter) do { \
    unsigned long _c = -read_csr(mcycle), _i = -read_csr(minstret); \
    code; \
    _c += read_csr(mcycle), _i += read_csr(minstret); \
    if (cid == 0) \
      printf("\n%s: %ld cycles, %ld.%ld cycles/iter, %ld.%ld CPI\n", \
             stringify(code), _c, _c/iter, 10*_c/iter%10, _c/_i, 10*_c/_i%10); \
  } while(0)

#endif //__UTIL_H
