/*
 * Copyright (c) 2013, Google, Inc. All rights reserved
 *
 * Use of this source code is governed by a MIT-style
 * license that can be found in the LICENSE file or at
 * https://opensource.org/licenses/MIT
 */
/*
 * Functions for unit tests.  See lib/unittest/include/unittest.h for usage.
 */
#include <lib/unittest.h>

#include <lk/debug.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <float.h>

#if LK_DEBUGLEVEL > 2
#define TEST_FAILURE_CASES 1
#else
#define TEST_FAILURE_CASES 0
#endif

extern void unittest_printf(const char* format, ...);

// Get contents of the CR register
extern uint32_t getCR(void);

// Get contents of the XER register
extern uint64_t getXER(void);

// Clear CR register
extern void clearCR(void);

// Clear XER register
extern void clearXER(void);

#define INFINITY (__builtin_inff())
#define NAN (__builtin_nanf(""))
#define INFINITY_D (__builtin_inf())
#define NAN_D (__builtin_nan(""))

static bool test_fpu(void) {
  BEGIN_TEST;

  float testFloats[] = {
    0.0, -0.0,  1.0, -1.0,  1.5, -1.5,
    1.6, -1.6,  1.4, -1.4,  2.0, -2.0,
    4.0, -10000000.4, 20000000.0, -20000.5, 20000.6,
    FLT_MIN, FLT_MAX, INFINITY, -INFINITY, NAN,
  };
  double testDoubles[] = {
    0.0, -0.0,  1.0, -1.0,  1.5, -1.5,
    1.6, -1.6,  1.4, -1.4,  2.0, -2.0,
    4.0, -10000000.4, 20000000.0, -20000.5, 20000.6,
    DBL_MIN, DBL_MAX, INFINITY_D, -INFINITY_D, NAN_D,
  };
  // Show results
#define PRINT_FPR1(name,i,FPR) if (sizeof(FPR) == 4) unittest_printf(name "[%02d] -> %08x\n", i, *(int*)&FPR);\
      else unittest_printf(name "[%02d] -> %016llx\n", i, *(unsigned long long*)&FPR);
#define PRINT_FPR2(name,i,j,FPR) if (sizeof(FPR) == 4) unittest_printf(name "[%02d],[%02d]) -> %08x\n", i, j, *(int*)&FPR);\
      else unittest_printf(name "[%02d],[%02d]) -> %016llx\n", i, j, *(unsigned long long*)&FPR);
#define PRINT_FPR3(name,i,j,k,FPR) if (sizeof(FPR) ==4 ) unittest_printf(name "[%02d],[%02d],[%02d]) -> %08x\n", i, j, k, *(int*)&FPR);\
      else unittest_printf(name "[%02d],[%02d],[%02d]) -> %016llx\n", i, j, k, *(unsigned long long*)&FPR);
    
  // Iterate
#define ITERATE1f(x) \
  for (unsigned int i=0; i<sizeof(testFloats)/sizeof(float); i++) \
    {float f0, f1=testFloats[i]; x;}
#define ITERATE2f(x) \
  for (unsigned int i=0; i<sizeof(testFloats)/sizeof(float); i++) \
    for (unsigned int j=0; j<sizeof(testFloats)/sizeof(float); j++) \
      {float f0, f1=testFloats[i], f2=testFloats[j]; x;}
#define ITERATE3f(x) \
  for (unsigned int i=0; i<sizeof(testFloats)/sizeof(float); i++) \
    for (unsigned int j=0; j<sizeof(testFloats)/sizeof(float); j++) \
      for (unsigned int k=0; k<sizeof(testFloats)/sizeof(float); k++) \
        {float f0, f1=testFloats[i], f2=testFloats[j], f3=testFloats[k]; x;}
#define ITERATE1d(x) \
  for (unsigned int i=0; i<sizeof(testDoubles)/sizeof(double); i++) \
    {double d0, d1=testDoubles[i]; x;}
#define ITERATE2d(x) \
  for (unsigned int i=0; i<sizeof(testDoubles)/sizeof(double); i++) \
    for (unsigned int j=0; j<sizeof(testDoubles)/sizeof(double); j++) \
      {double d0, d1=testDoubles[i], d2=testDoubles[j]; x;}
#define ITERATE3d(x) \
  for (unsigned int i=0; i<sizeof(testDoubles)/sizeof(double); i++) \
    for (unsigned int j=0; j<sizeof(testDoubles)/sizeof(double); j++) \
      for (unsigned int k=0; k<sizeof(testDoubles)/sizeof(double); k++) \
        {double d0, d1=testDoubles[i], d2=testDoubles[j], d3=testDoubles[k]; x;}

	// Floating-Point Arithmetic Instructions
	ITERATE2d(__asm__("fadd    %0,%1,%2" : "=f"(d0) : "f"(d1), "f"(d2));  PRINT_FPR2("fadd  ",i,j,d0));
	ITERATE2f(__asm__("fadds   %0,%1,%2" : "=f"(f0) : "f"(f1), "f"(f2));  PRINT_FPR2("fadds ",i,j,f0));
	ITERATE2d(__asm__("fsub    %0,%1,%2" : "=f"(d0) : "f"(d1), "f"(d2));  PRINT_FPR2("fsub  ",i,j,d0));
	ITERATE2f(__asm__("fsubs   %0,%1,%2" : "=f"(f0) : "f"(f1), "f"(f2));  PRINT_FPR2("fsubs ",i,j,f0));
	ITERATE2d(__asm__("fmul    %0,%1,%2" : "=f"(d0) : "f"(d1), "f"(d2));  PRINT_FPR2("fmul  ",i,j,d0));
	ITERATE2f(__asm__("fmuls   %0,%1,%2" : "=f"(f0) : "f"(f1), "f"(f2));  PRINT_FPR2("fmuls ",i,j,f0));
	ITERATE2d(__asm__("fdiv    %0,%1,%2" : "=f"(d0) : "f"(d1), "f"(d2));  PRINT_FPR2("fdiv  ",i,j,d0));
	ITERATE2f(__asm__("fdivs   %0,%1,%2" : "=f"(f0) : "f"(f1), "f"(f2));  PRINT_FPR2("fdivs ",i,j,f0));
	ITERATE1d(__asm__("fsqrt   %0,%1" : "=f"(d0) : "f"(d1));  PRINT_FPR1("fsqrt   ",i,d0));
	ITERATE1f(__asm__("fsqrts  %0,%1" : "=f"(f0) : "f"(f1));  PRINT_FPR1("fsqrts  ",i,f0));
	ITERATE1f(__asm__("fres    %0,%1" : "=f"(f0) : "f"(f1));  PRINT_FPR1("fres    ",i,f0));
	ITERATE1d(__asm__("frsqrte %0,%1" : "=f"(d0) : "f"(d1));  PRINT_FPR1("frsqrte ",i,d0));
	ITERATE3d(__asm__("fsel    %0,%1,%2,%3" : "=f"(d0) : "f"(d1), "f"(d2), "f"(d3));  PRINT_FPR3("fsel    ",i,j,k,d0));

	// Floating-Point Multiply-Add Instructions
	ITERATE3d(__asm__("fmadd   %0,%1,%2,%3" : "=f"(d0) : "f"(d1), "f"(d2), "f"(d3));  PRINT_FPR3("fmadd   ",i,j,k,d0));
	ITERATE3f(__asm__("fmadds  %0,%1,%2,%3" : "=f"(f0) : "f"(f1), "f"(f2), "f"(f3));  PRINT_FPR3("fmadds  ",i,j,k,f0));
	ITERATE3d(__asm__("fmsub   %0,%1,%2,%3" : "=f"(d0) : "f"(d1), "f"(d2), "f"(d3));  PRINT_FPR3("fmsub   ",i,j,k,d0));
	ITERATE3f(__asm__("fmsubs  %0,%1,%2,%3" : "=f"(f0) : "f"(f1), "f"(f2), "f"(f3));  PRINT_FPR3("fmsubs  ",i,j,k,f0));
	ITERATE3d(__asm__("fnmadd  %0,%1,%2,%3" : "=f"(d0) : "f"(d1), "f"(d2), "f"(d3));  PRINT_FPR3("fnmadd  ",i,j,k,d0));
	ITERATE3f(__asm__("fnmadds %0,%1,%2,%3" : "=f"(f0) : "f"(f1), "f"(f2), "f"(f3));  PRINT_FPR3("fnmadds ",i,j,k,f0));
	ITERATE3d(__asm__("fnmsub  %0,%1,%2,%3" : "=f"(d0) : "f"(d1), "f"(d2), "f"(d3));  PRINT_FPR3("fnmsub  ",i,j,k,d0));
	ITERATE3f(__asm__("fnmsubs %0,%1,%2,%3" : "=f"(f0) : "f"(f1), "f"(f2), "f"(f3));  PRINT_FPR3("fnmsubs ",i,j,k,f0));

	// Floating-Point Rounding and Conversion Instructions
	ITERATE1d(__asm__("frsp    %0,%1" : "=f"(d0) : "f"(d1));  PRINT_FPR1("frsp   ",i,d0));
	ITERATE1f(__asm__("frsp    %0,%1" : "=f"(f0) : "f"(f1));  PRINT_FPR1("frsp   ",i,f0));
	ITERATE1d(__asm__("fcfid   %0,%1" : "=f"(d0) : "f"(d1));  PRINT_FPR1("fcfid  ",i,d0));
	ITERATE1f(__asm__("fcfid   %0,%1" : "=f"(f0) : "f"(f1));  PRINT_FPR1("fcfid  ",i,f0));
	ITERATE1d(__asm__("fctidz  %0,%1" : "=f"(d0) : "f"(d1));  PRINT_FPR1("fctidz ",i,d0));
	ITERATE1f(__asm__("fctidz  %0,%1" : "=f"(f0) : "f"(f1));  PRINT_FPR1("fctidz ",i,f0));
	ITERATE1d(__asm__("fctiw   %0,%1" : "=f"(d0) : "f"(d1));  PRINT_FPR1("fctiw  ",i,d0));
	ITERATE1f(__asm__("fctiw   %0,%1" : "=f"(f0) : "f"(f1));  PRINT_FPR1("fctiw  ",i,f0));
	ITERATE1d(__asm__("fctiwz  %0,%1" : "=f"(d0) : "f"(d1));  PRINT_FPR1("fctiwz ",i,d0));
	ITERATE1f(__asm__("fctiwz  %0,%1" : "=f"(f0) : "f"(f1));  PRINT_FPR1("fctiwz ",i,f0));

	// Floating-Point Compare Instructions
	ITERATE2d(__asm__("fcmpu   1,%1,%2\n" "mffs %0" : "=f"(d0) : "f"(d1), "f"(d2));  PRINT_FPR2("fcmpu ",i,j,d0));
	ITERATE2f(__asm__("fcmpu   1,%1,%2\n" "mffs %0" : "=f"(f0) : "f"(f1), "f"(f2));  PRINT_FPR2("fcmpu ",i,j,f0));
	ITERATE2d(__asm__("fcmpo   1,%1,%2\n" "mffs %0" : "=f"(d0) : "f"(d1), "f"(d2));  PRINT_FPR2("fcmpo ",i,j,d0));
	ITERATE2f(__asm__("fcmpo   1,%1,%2\n" "mffs %0" : "=f"(f0) : "f"(f1), "f"(f2));  PRINT_FPR2("fcmpo ",i,j,f0));

	// Floating-Point Status and Control Register Instructions
	// TODO ?

	// Floating-Point Move Instructions
	ITERATE1d(__asm__("fmr     %0,%1" : "=f"(d0) : "f"(d1));  PRINT_FPR1("fmr   ",i,d0));
	ITERATE1f(__asm__("fmr     %0,%1" : "=f"(f0) : "f"(f1));  PRINT_FPR1("fmr   ",i,f0));
	ITERATE1d(__asm__("fneg    %0,%1" : "=f"(d0) : "f"(d1));  PRINT_FPR1("fneg  ",i,d0));
	ITERATE1f(__asm__("fneg    %0,%1" : "=f"(f0) : "f"(f1));  PRINT_FPR1("fneg  ",i,f0));
	ITERATE1d(__asm__("fabs    %0,%1" : "=f"(d0) : "f"(d1));  PRINT_FPR1("fabs  ",i,d0));
	ITERATE1f(__asm__("fabs    %0,%1" : "=f"(f0) : "f"(f1));  PRINT_FPR1("fabs  ",i,f0));
	ITERATE1d(__asm__("fnabs   %0,%1" : "=f"(d0) : "f"(d1));  PRINT_FPR1("fnabs ",i,d0));
	ITERATE1f(__asm__("fnabs   %0,%1" : "=f"(f0) : "f"(f1));  PRINT_FPR1("fnabs ",i,f0));
  END_TEST;
}

BEGIN_TEST_CASE(ppc_fpu)
RUN_TEST(test_fpu);
END_TEST_CASE(ppc_fpu)