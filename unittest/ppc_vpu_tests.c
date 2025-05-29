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
#include <vec_types.h>
#include <altivec.h>

#if LK_DEBUGLEVEL > 2
#define TEST_FAILURE_CASES 1
#else
#define TEST_FAILURE_CASES 0
#endif

extern void unittest_printf(const char* format, ...);

#define INFINITY (__builtin_inff())
#define NAN (__builtin_nanf(""))

int testInts[] = {
  0, 1, 2, 3, 4, -1, -2, -3, -4,
  0x00033333, 0x00fffff0, 0x10000000, 0x12345678,
  0x1fffffff, 0x42385722, 0x70000000, 0x72233411,
  0x7fffffff, 0x80000000, 0x80000001, 0x8fffffff,
};
float testFloats[] = {
  0.0, -0.0,  1.0, -1.0,  1.5, -1.5,
  1.6, -1.6,  1.4, -1.4,  2.0, -2.0,
  4.0, -10000000.4, 20000000.0, -20000.5, 20000.6,
  FLT_MIN, FLT_MAX, INFINITY, -INFINITY, NAN,
};

#define PRINT_VEC_SINT(v0) do { \
    union { vec_int4 v; int32_t i[4]; } _u; \
    _u.v = v0; \
    for(int _j=0; _j<4; _j++) unittest_printf("%08x ", _u.i[_j]); \
    unittest_printf("\n"); \
} while(0)

#define PRINT_VEC_UINT(v0) do { \
    union { vec_uint4 v; uint32_t i[4]; } _u; \
    _u.v = v0; \
    for(int _j=0; _j<4; _j++) unittest_printf("%08x ", _u.i[_j]); \
    unittest_printf("\n"); \
} while(0)

#define PRINT_VEC_FLT(v0) do { \
    union { vec_float4 v; double f[4]; } _u; \
    _u.v = v0; \
    for(int _j=0; _j<4; _j++) unittest_printf("%lf ", _u.f[_j]); \
    unittest_printf("\n"); \
} while(0)


#define PRINT_VEC_SB(v0) do { \
    union { __vector signed char v; int8_t b[16]; } _u; \
    _u.v = v0; \
    for(int _j=0; _j<16; _j++) unittest_printf("%02x ", _u.b[_j]); \
    unittest_printf("\n"); \
} while(0)

#define PRINT_VEC_SS(v0) do { \
    union { __vector short int v; int16_t s[8]; } _u; \
    _u.v = v0; \
    for(int _j=0; _j<8; _j++) unittest_printf("%04x ", _u.s[_j]); \
    unittest_printf("\n"); \
} while(0)

#define PRINT_VEC_US(v0) do { \
    union { __vector short unsigned int v; uint16_t s[8]; } _u; \
    _u.v = v0; \
    for(int _j=0; _j<8; _j++) unittest_printf("%04x ", _u.s[_j]); \
    unittest_printf("\n"); \
} while(0)

#define PRINT_VEC_UB(v0) do { \
    union { __vector unsigned char v; uint8_t b[16]; } _u; \
    _u.v = v0; \
    for(int _j=0; _j<16; _j++) unittest_printf("%02x ", _u.b[_j]); \
    unittest_printf("\n"); \
} while(0)

#define PRINT_VEC_BC(v0) do { \
    union { __vector bool char v; bool b[16]; } _u; \
    _u.v = v0; \
    for(int _j=0; _j<16; _j++) unittest_printf("%02x ", _u.b[_j]); \
    unittest_printf("\n"); \
} while(0)

#define PRINT_VEC_BS(v0) do { \
    union { __vector bool short v; bool b[16]; } _u; \
    _u.v = v0; \
    for(int _j=0; _j<16; _j++) unittest_printf("%02x ", _u.b[_j]); \
    unittest_printf("\n"); \
} while(0)

#define PRINT_VEC_BI(v0) do { \
    union { __vector bool int v; bool b[16]; } _u; \
    _u.v = v0; \
    for(int _j=0; _j<16; _j++) unittest_printf("%02x ", _u.b[_j]); \
    unittest_printf("\n"); \
} while(0)

#define PRINT_VEC_PX(v0) do { \
    union { __vector pixel v; uint16_t s[8]; } _u; \
    _u.v = v0; \
    for(int _j=0; _j<8; _j++) unittest_printf("%04x ", _u.s[_j]); \
    unittest_printf("\n"); \
} while(0)


#define PRINT_MEMORY(p,N) do { \
  for(int i = 0; i < (N); i++) { \
    unittest_printf("%02hhx", ((uint8_t*)(p))[i]); \
  } \
  unittest_printf("\n"); \
} while(0)


static bool test_vpu(void) {
  BEGIN_TEST;

  for (uint32_t i = 0; i < sizeof(testInts) / sizeof(int); i++) {
    for (uint32_t j = 0; j < sizeof(testInts) / sizeof(int); j++) {
      vec_int4 v1sw = (vec_int4){ testInts[i] };
      vec_int4 v2sw = (vec_int4){ testInts[j], testInts[j], 0.0, 0.0 };
      vec_short8 v1sh = *(vec_short8*)&v1sw;
      vec_short8 v2sh = *(vec_short8*)&v2sw;
      vec_char16 v1sb = *(vec_char16*)&v1sw;
      vec_char16 v2sb = *(vec_char16*)&v2sw;

      vec_uint4 v1uw = (vec_uint4){ testInts[i] };
      vec_uint4 v2uw = (vec_uint4){ testInts[j], testInts[j], 0.0, 0.0 };
      vec_ushort8 v1uh = *(vec_ushort8*)&v1uw;
      vec_ushort8 v2uh = *(vec_ushort8*)&v2uw;
      vec_uchar16 v1ub = *(vec_uchar16*)&v1uw;
      vec_uchar16 v2ub = *(vec_uchar16*)&v2uw;

      // Vector Integer Arithmetic Instructions
      unittest_printf("vec_vaddsbs  ([%02d],[%02d]) -> ", i, j); PRINT_VEC_SB(vec_vaddsbs(v1sb,v2sb));
      unittest_printf("vec_vaddshs  ([%02d],[%02d]) -> ", i, j); PRINT_VEC_SS(vec_vaddshs(v1sh,v2sh));
      unittest_printf("vec_vaddsws  ([%02d],[%02d]) -> ", i, j); PRINT_VEC_SINT(vec_vaddsws(v1sw,v2sw));
      unittest_printf("vec_vaddubm  ([%02d],[%02d]) -> ", i, j); PRINT_VEC_UB(vec_vaddubm(v1ub,v2ub));
      unittest_printf("vec_vadduhm  ([%02d],[%02d]) -> ", i, j); PRINT_VEC_US(vec_vadduhm(v1uh,v2uh));
      unittest_printf("vec_vadduwm  ([%02d],[%02d]) -> ", i, j); PRINT_VEC_UINT(vec_vadduwm(v1uw,v2uw));
      unittest_printf("vec_vaddubs  ([%02d],[%02d]) -> ", i, j); PRINT_VEC_UB(vec_vaddubs(v1ub,v2ub));
      unittest_printf("vec_vadduhs  ([%02d],[%02d]) -> ", i, j); PRINT_VEC_US(vec_vadduhs(v1uh,v2uh));
      unittest_printf("vec_vadduws  ([%02d],[%02d]) -> ", i, j); PRINT_VEC_UINT(vec_vadduws(v1uw,v2uw));
      unittest_printf("vec_vaddcuw  ([%02d],[%02d]) -> ", i, j); PRINT_VEC_UINT(vec_vaddcuw(v1uw,v2uw));
      unittest_printf("vec_vsubsbs  ([%02d],[%02d]) -> ", i, j); PRINT_VEC_SB(vec_vsubsbs(v1sb,v2sb));
      unittest_printf("vec_vsubshs  ([%02d],[%02d]) -> ", i, j); PRINT_VEC_SS(vec_vsubshs(v1sh,v2sh));
      unittest_printf("vec_vsubsws  ([%02d],[%02d]) -> ", i, j); PRINT_VEC_SINT(vec_vsubsws(v1sw,v2sw));
      unittest_printf("vec_vsububm  ([%02d],[%02d]) -> ", i, j); PRINT_VEC_UB(vec_vsububm(v1ub,v2ub));
      unittest_printf("vec_vsubuhm  ([%02d],[%02d]) -> ", i, j); PRINT_VEC_US(vec_vsubuhm(v1uh,v2uh));
      unittest_printf("vec_vsubuwm  ([%02d],[%02d]) -> ", i, j); PRINT_VEC_UINT(vec_vsubuwm(v1uw,v2uw));
      unittest_printf("vec_vsububs  ([%02d],[%02d]) -> ", i, j); PRINT_VEC_UB(vec_vsububs(v1ub,v2ub));
      unittest_printf("vec_vsubuhs  ([%02d],[%02d]) -> ", i, j); PRINT_VEC_US(vec_vsubuhs(v1uh,v2uh));
      unittest_printf("vec_vsubuws  ([%02d],[%02d]) -> ", i, j); PRINT_VEC_UINT(vec_vsubuws(v1uw,v2uw));
      unittest_printf("vec_vsubcuw  ([%02d],[%02d]) -> ", i, j); PRINT_VEC_UINT(vec_vsubcuw(v1uw,v2uw));
      //unittest_printf("vec_vmuloub  ([%02d],[%02d]) -> ", i, j); PRINT_VEC_US(vec_vmuloub(v1ub,v2ub));
      //unittest_printf("vec_vmulouh  ([%02d],[%02d]) -> ", i, j); PRINT_VEC_UINT(vec_vmulouh(v1uh,v2uh));
      //unittest_printf("vec_vmulosb  ([%02d],[%02d]) -> ", i, j); PRINT_VEC_SS(vec_vmulosb(v1sb,v2sb));
      //unittest_printf("vec_vmulosh  ([%02d],[%02d]) -> ", i, j); PRINT_VEC_SINT(vec_vmulosh(v1sh,v2sh));
      //unittest_printf("vec_vmuleub  ([%02d],[%02d]) -> ", i, j); PRINT_VEC_US(vec_vmuleub(v1ub,v2ub));
      //unittest_printf("vec_vmuleuh  ([%02d],[%02d]) -> ", i, j); PRINT_VEC_UINT(vec_vmuleuh(v1uh,v2uh));
      //unittest_printf("vec_vmulesb  ([%02d],[%02d]) -> ", i, j); PRINT_VEC_SS(vec_vmulesb(v1sb,v2sb));
      //unittest_printf("vec_vmulesh  ([%02d],[%02d]) -> ", i, j); PRINT_VEC_SINT(vec_vmulesh(v1sh,v2sh));
      //unittest_printf("vec_vsumsws  ([%02d],[%02d]) -> ", i, j); PRINT_VEC_SINT(vec_vsumsws(v1sw,v2sw));
      //unittest_printf("vec_vsum2sws ([%02d],[%02d]) -> ", i, j); PRINT_VEC_SINT(vec_vsum2sws(v1sw,v2sw));
      //unittest_printf("vec_vsum4ubs ([%02d],[%02d]) -> ", i, j); PRINT_VEC_UINT(vec_vsum4ubs(v1ub,v2uw));
      //unittest_printf("vec_vsum4sbs ([%02d],[%02d]) -> ", i, j); PRINT_VEC_SINT(vec_vsum4sbs(v1sb,v2sw));
      //unittest_printf("vec_vsum4shs ([%02d],[%02d]) -> ", i, j); PRINT_VEC_SINT(vec_vsum4shs(v1sh,v2sw));
      unittest_printf("vec_vavgub   ([%02d],[%02d]) -> ", i, j); PRINT_VEC_UB(vec_vavgub(v1ub,v2ub));
      unittest_printf("vec_vavguh   ([%02d],[%02d]) -> ", i, j); PRINT_VEC_US(vec_vavguh(v1uh,v2uh));
      unittest_printf("vec_vavguw   ([%02d],[%02d]) -> ", i, j); PRINT_VEC_UINT(vec_vavguw(v1uw,v2uw));
      unittest_printf("vec_vavgsb   ([%02d],[%02d]) -> ", i, j); PRINT_VEC_SB(vec_vavgsb(v1sb,v2sb));
      unittest_printf("vec_vavgsh   ([%02d],[%02d]) -> ", i, j); PRINT_VEC_SS(vec_vavgsh(v1sh,v2sh));
      unittest_printf("vec_vavgsw   ([%02d],[%02d]) -> ", i, j); PRINT_VEC_SINT(vec_vavgsw(v1sw,v2sw));
      unittest_printf("vec_vmaxub   ([%02d],[%02d]) -> ", i, j); PRINT_VEC_UB(vec_vmaxub(v1ub,v2ub));
      unittest_printf("vec_vmaxuh   ([%02d],[%02d]) -> ", i, j); PRINT_VEC_US(vec_vmaxuh(v1uh,v2uh));
      unittest_printf("vec_vmaxuw   ([%02d],[%02d]) -> ", i, j); PRINT_VEC_UINT(vec_vmaxuw(v1uw,v2uw));
      unittest_printf("vec_vmaxsb   ([%02d],[%02d]) -> ", i, j); PRINT_VEC_SB(vec_vmaxsb(v1sb,v2sb));
      unittest_printf("vec_vmaxsh   ([%02d],[%02d]) -> ", i, j); PRINT_VEC_SS(vec_vmaxsh(v1sh,v2sh));
      unittest_printf("vec_vmaxsw   ([%02d],[%02d]) -> ", i, j); PRINT_VEC_SINT(vec_vmaxsw(v1sw,v2sw));
      unittest_printf("vec_vminub   ([%02d],[%02d]) -> ", i, j); PRINT_VEC_UB(vec_vminub(v1ub,v2ub));
      unittest_printf("vec_vminuh   ([%02d],[%02d]) -> ", i, j); PRINT_VEC_US(vec_vminuh(v1uh,v2uh));
      unittest_printf("vec_vminuw   ([%02d],[%02d]) -> ", i, j); PRINT_VEC_UINT(vec_vminuw(v1uw,v2uw));
      unittest_printf("vec_vminsb   ([%02d],[%02d]) -> ", i, j); PRINT_VEC_SB(vec_vminsb(v1sb,v2sb));
      unittest_printf("vec_vminsh   ([%02d],[%02d]) -> ", i, j); PRINT_VEC_SS(vec_vminsh(v1sh,v2sh));
      unittest_printf("vec_vminsw   ([%02d],[%02d]) -> ", i, j); PRINT_VEC_SINT(vec_vminsw(v1sw,v2sw));

      // Vector Integer Compare Instructions
      unittest_printf("vec_vcmpgtub ([%02d],[%02d]) -> ", i, j); PRINT_VEC_BC(vec_vcmpgtub(v1ub,v2ub));
      unittest_printf("vec_vcmpgtuh ([%02d],[%02d]) -> ", i, j); PRINT_VEC_BS(vec_vcmpgtuh(v1uh,v2uh));
      unittest_printf("vec_vcmpgtuw ([%02d],[%02d]) -> ", i, j); PRINT_VEC_BI(vec_vcmpgtuw(v1uw,v2uw));
      unittest_printf("vec_vcmpgtsb ([%02d],[%02d]) -> ", i, j); PRINT_VEC_BC(vec_vcmpgtsb(v1sb,v2sb));
      unittest_printf("vec_vcmpgtsh ([%02d],[%02d]) -> ", i, j); PRINT_VEC_BS(vec_vcmpgtsh(v1sh,v2sh));
      unittest_printf("vec_vcmpgtsw ([%02d],[%02d]) -> ", i, j); PRINT_VEC_BI(vec_vcmpgtsw(v1sw,v2sw));
      unittest_printf("vec_vcmpequb ([%02d],[%02d]) -> ", i, j); PRINT_VEC_BC(vec_vcmpequb(v1ub,v2ub));
      unittest_printf("vec_vcmpequh ([%02d],[%02d]) -> ", i, j); PRINT_VEC_BS(vec_vcmpequh(v1uh,v2uh));
      unittest_printf("vec_vcmpequw ([%02d],[%02d]) -> ", i, j); PRINT_VEC_BI(vec_vcmpequw(v1uw,v2uw));

      // Vector Integer Logical Instructions
      unittest_printf("vec_vand     ([%02d],[%02d]) -> ", i, j); PRINT_VEC_UINT(vec_vand(v1uw,v2uw));
      unittest_printf("vec_vor      ([%02d],[%02d]) -> ", i, j); PRINT_VEC_UINT(vec_vor(v1uw,v2uw));
      unittest_printf("vec_vxor     ([%02d],[%02d]) -> ", i, j); PRINT_VEC_UINT(vec_vxor(v1uw,v2uw));
      unittest_printf("vec_vandc    ([%02d],[%02d]) -> ", i, j); PRINT_VEC_UINT(vec_vandc(v1uw,v2uw));
      unittest_printf("vec_vnor     ([%02d],[%02d]) -> ", i, j); PRINT_VEC_UINT(vec_vnor(v1uw,v2uw));

      // Vector Integer Rotate and Shift Instructions
      unittest_printf("vec_vrlb     ([%02d],[%02d]) -> ", i, j); PRINT_VEC_UB(vec_vrlb(v1ub,v2ub));
      unittest_printf("vec_vrlh     ([%02d],[%02d]) -> ", i, j); PRINT_VEC_US(vec_vrlh(v1uh,v2uh));
      unittest_printf("vec_vrlw     ([%02d],[%02d]) -> ", i, j); PRINT_VEC_UINT(vec_vrlw(v1uw,v2uw));
      unittest_printf("vec_vslb     ([%02d],[%02d]) -> ", i, j); PRINT_VEC_UB(vec_vslb(v1ub,v2ub));
      unittest_printf("vec_vslh     ([%02d],[%02d]) -> ", i, j); PRINT_VEC_US(vec_vslh(v1uh,v2uh));
      unittest_printf("vec_vslw     ([%02d],[%02d]) -> ", i, j); PRINT_VEC_UINT(vec_vslw(v1uw,v2uw));
      unittest_printf("vec_vsrb     ([%02d],[%02d]) -> ", i, j); PRINT_VEC_UB(vec_vsrb(v1ub,v2ub));
      unittest_printf("vec_vsrh     ([%02d],[%02d]) -> ", i, j); PRINT_VEC_US(vec_vsrh(v1uh,v2uh));
      unittest_printf("vec_vsrw     ([%02d],[%02d]) -> ", i, j); PRINT_VEC_UINT(vec_vsrw(v1uw,v2uw));
      unittest_printf("vec_vsrab    ([%02d],[%02d]) -> ", i, j); PRINT_VEC_UB(vec_vsrab(v1ub,v2ub));
      unittest_printf("vec_vsrah    ([%02d],[%02d]) -> ", i, j); PRINT_VEC_US(vec_vsrah(v1uh,v2uh));
      unittest_printf("vec_vsraw    ([%02d],[%02d]) -> ", i, j); PRINT_VEC_UINT(vec_vsraw(v1uw,v2uw));
    }
  }

  for (uint32_t i = 0; i<sizeof(testFloats) / sizeof(float); i++) {
    vec_float4 v1 = (vec_float4){ testFloats[i] };

    // Vector Floating-Point Rounding and Conversion Instructions
    unittest_printf("vec_vrfin     ([%02d])    -> ", i); PRINT_VEC_FLT(vec_vrfin(v1));
    unittest_printf("vec_vrfiz     ([%02d])    -> ", i); PRINT_VEC_FLT(vec_vrfiz(v1));
    unittest_printf("vec_vrfip     ([%02d])    -> ", i); PRINT_VEC_FLT(vec_vrfip(v1));
    unittest_printf("vec_vrfim     ([%02d])    -> ", i); PRINT_VEC_FLT(vec_vrfim(v1));
    unittest_printf("vec_vctuxs    ([%02d], 0) -> ", i); PRINT_VEC_UINT(vec_vctuxs(v1, 0));
    unittest_printf("vec_vctsxs    ([%02d], 0) -> ", i); PRINT_VEC_SINT(vec_vctsxs(v1, 0));
    unittest_printf("vec_vctuxs    ([%02d], 1) -> ", i); PRINT_VEC_UINT(vec_vctuxs(v1, 1));
    unittest_printf("vec_vctsxs    ([%02d], 1) -> ", i); PRINT_VEC_SINT(vec_vctsxs(v1, 1));
    unittest_printf("vec_vctuxs    ([%02d], 2) -> ", i); PRINT_VEC_UINT(vec_vctuxs(v1, 2));
    unittest_printf("vec_vctsxs    ([%02d], 2) -> ", i); PRINT_VEC_SINT(vec_vctsxs(v1, 2));
    unittest_printf("vec_vctuxs    ([%02d],31) -> ", i); PRINT_VEC_UINT(vec_vctuxs(v1, 31));
    unittest_printf("vec_vctsxs    ([%02d],31) -> ", i); PRINT_VEC_SINT(vec_vctsxs(v1, 31));

    // Vector Floating-Point Estimate Instructions
    unittest_printf("vec_vrefp     ([%02d])    -> ", i); PRINT_VEC_FLT(vec_vrefp(v1));
    unittest_printf("vec_vrsqrtefp ([%02d])    -> ", i); PRINT_VEC_FLT(vec_vrsqrtefp(v1));
    unittest_printf("vec_vlogefp   ([%02d])    -> ", i); PRINT_VEC_FLT(vec_vlogefp(v1));
    unittest_printf("vec_vexptefp  ([%02d])    -> ", i); PRINT_VEC_FLT (vec_vexptefp(v1));
  }

  for (uint32_t i = 0; i<sizeof(testInts) / sizeof(int); i++) {
    vec_uint4 v1 = (vec_uint4){ testInts[i] };
    vec_int4 v2 = (vec_int4){ testInts[i] };

    // Vector Floating-Point Rounding and Conversion Instructions
    unittest_printf("vec_vcfux  ([%02d], 0) -> ", i); PRINT_VEC_FLT(vec_vcfux(v1, 0));
    unittest_printf("vec_vcfsx  ([%02d], 0) -> ", i); PRINT_VEC_FLT(vec_vcfsx(v2, 0));
    unittest_printf("vec_vcfux  ([%02d], 1) -> ", i); PRINT_VEC_FLT(vec_vcfux(v1, 1));
    unittest_printf("vec_vcfsx  ([%02d], 1) -> ", i); PRINT_VEC_FLT(vec_vcfsx(v2, 1));
    unittest_printf("vec_vcfux  ([%02d], 2) -> ", i); PRINT_VEC_FLT(vec_vcfux(v1, 2));
    unittest_printf("vec_vcfsx  ([%02d], 2) -> ", i); PRINT_VEC_FLT(vec_vcfsx(v2, 2));
    unittest_printf("vec_vcfux  ([%02d],31) -> ", i); PRINT_VEC_FLT(vec_vcfux(v1, 31));
    unittest_printf("vec_vcfsx  ([%02d],31) -> ", i); PRINT_VEC_FLT(vec_vcfsx(v2, 31));  
  }

  // Input arguments: 2 vec_float4
  for (uint32_t i = 0; i<sizeof(testFloats) / sizeof(float); i++) {
    for (uint32_t j = 0; j<sizeof(testFloats) / sizeof(float); j++) {
      vec_float4 v1 = (vec_float4){ testFloats[i] };
      vec_float4 v2 = (vec_float4){ testFloats[j], testFloats[j], 0.0, 0.0 };

      // Floating-Point Arithmetic Instructions
      unittest_printf("vec_vaddfp   ([%02d],[%02d]) -> ", i, j); PRINT_VEC_FLT(vec_vaddfp(v1,v2));
      unittest_printf("vec_vsubfp   ([%02d],[%02d]) -> ", i, j); PRINT_VEC_FLT(vec_vsubfp(v1,v2));
      unittest_printf("vec_vminfp   ([%02d],[%02d]) -> ", i, j); PRINT_VEC_FLT(vec_vminfp(v1,v2));
      unittest_printf("vec_vmaxfp   ([%02d],[%02d]) -> ", i, j); PRINT_VEC_FLT(vec_vmaxfp(v1,v2));

      // Vector Floating-Point Compare Instructions
      unittest_printf("vec_vcmpbfp  ([%02d],[%02d]) -> ", i, j); PRINT_VEC_SINT(vec_vcmpbfp(v1,v2));
      unittest_printf("vec_vcmpeqfp ([%02d],[%02d]) -> ", i, j); PRINT_VEC_BI(vec_vcmpeqfp(v1,v2));
      unittest_printf("vec_vcmpgefp ([%02d],[%02d]) -> ", i, j); PRINT_VEC_BI(vec_vcmpgefp(v1,v2));
      unittest_printf("vec_vcmpgefp ([%02d],[%02d]) -> ", i, j); PRINT_VEC_BI(vec_vcmpgtfp(v1,v2));
    }
  }

  // Input arguments: 3 vec_float4
  for (uint32_t i = 0; i<sizeof(testFloats) / sizeof(float); i++) {
    for (uint32_t j = 0; j<sizeof(testFloats) / sizeof(float); j++) {
      for (uint32_t k = 0; k<sizeof(testFloats) / sizeof(float); k++) {
        vec_float4 v1 = (vec_float4){ testFloats[i] };
        vec_float4 v2 = (vec_float4){ testFloats[j], testFloats[j], 0.0, 0.0 };
        vec_float4 v3 = (vec_float4){ testFloats[k], 0.0, testFloats[k], 0.0 };

        // Vector Floating-Point Multiply-Add Instructions
        unittest_printf("vec_vmaddfp  ([%02d],[%02d],[%02d]) -> ", i, j, k); PRINT_VEC_FLT(vec_vmaddfp(v1,v2,v3));
        unittest_printf("vec_vnmsubfp ([%02d],[%02d],[%02d]) -> ", i, j, k); PRINT_VEC_FLT(vec_vnmsubfp(v1,v2,v3));
      }
    }
  }

  uint8_t srcUChars[16]   = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
  uint16_t srcUShorts[16] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
  uint32_t srcUInts[16]   = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };

  vec_uchar16 srcVecChar  = (vec_uchar16){ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
  vec_ushort8 srcVecShort = (vec_ushort8){ 0, 1, 2, 3, 4, 5, 6, 7 };
  vec_uint4 srcVecInt     = (vec_uint4){ 0, 1, 2, 3 };

  uint8_t dstUChars[16]   = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
  uint16_t dstUShorts[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
  uint32_t dstUInts[16]   = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

  // Vector Load Instructions
  unittest_printf("vec_lvebx  (0, testUChars)  -> "); PRINT_VEC_UB(vec_lvebx(0,srcUChars));
  unittest_printf("vec_lvehx  (5, testUShorts) -> "); PRINT_VEC_US(vec_lvehx(5,srcUShorts));
  unittest_printf("vec_lvewx  (16,testUInts)   -> "); PRINT_VEC_UINT(vec_lvewx(16,srcUInts));
  unittest_printf("vec_lvx    (16,testUInts)   -> "); PRINT_VEC_UINT(vec_lvx(32,srcUInts));
  unittest_printf("vec_lvxl   (16,testUInts)   -> "); PRINT_VEC_UINT(vec_lvxl(32,srcUInts));
  //unittest_printf("vec_lvlx   (16,testUInts)   -> "); PRINT_VEC_UINT(vec_lvlx(32,srcUInts));
  //unittest_printf("vec_lvlxl  (16,testUInts)   -> "); PRINT_VEC_UINT(vec_lvlxl(32,srcUInts));
  //unittest_printf("vec_lvrx   (16,testUInts)   -> "); PRINT_VEC_UINT(vec_lvrx(32,srcUInts));
  unittest_printf("vec_lvsl   (16,testUInts)   -> "); PRINT_VEC_UB(vec_lvsl(32,srcUInts));
  unittest_printf("vec_lvsr   (16,testUInts)   -> "); PRINT_VEC_UB(vec_lvsr(32,srcUInts));

  // Vector Store Instructions (TODO: I am not sure if this makes any sense)
  unittest_printf("vec_stvebx (srcVecChar,  0, dstUChars)  -> "); vec_stvebx(srcVecChar, 0, dstUChars); PRINT_MEMORY(dstUChars, 16);
  unittest_printf("vec_stvehx (srcVecShort, 5, dstUShorts) -> "); vec_stvehx(srcVecShort, 5, dstUShorts); PRINT_MEMORY(dstUShorts, 32);
  unittest_printf("vec_stvewx (srcVecInt,  16, dstUInts)   -> "); vec_stvewx(srcVecInt, 16, dstUInts); PRINT_MEMORY(dstUInts, 64);

  srcVecInt = vec_vadduwm(srcVecInt, srcVecInt);
  unittest_printf("vec_stvx   (srcVecInt,  32, dstUInts)   -> "); vec_stvx(srcVecInt, 32, dstUInts); PRINT_MEMORY(dstUInts, 64);
  srcVecInt = vec_vadduwm(srcVecInt, srcVecInt);
  unittest_printf("vec_stvxl  (srcVecInt,  32, dstUInts)   -> "); vec_stvxl(srcVecInt, 32, dstUInts); PRINT_MEMORY(dstUInts, 64);

  // Input arguments: 1 SIMM
  // Vector Splat Instructions
  unittest_printf("vec_vspltisb (-1) -> "); PRINT_VEC_SB(vec_vspltisb(-1));
  unittest_printf("vec_vspltisb ( 0) -> "); PRINT_VEC_SB(vec_vspltisb(0));
  unittest_printf("vec_vspltisb ( 1) -> "); PRINT_VEC_SB(vec_vspltisb(1));
  unittest_printf("vec_vspltisb ( 2) -> "); PRINT_VEC_SB(vec_vspltisb(2));
  unittest_printf("vec_vspltish (-1) -> "); PRINT_VEC_SS(vec_vspltish(-1));
  unittest_printf("vec_vspltish ( 0) -> "); PRINT_VEC_SS(vec_vspltish(0));
  unittest_printf("vec_vspltish ( 1) -> "); PRINT_VEC_SS(vec_vspltish(1));
  unittest_printf("vec_vspltish ( 2) -> "); PRINT_VEC_SS(vec_vspltish(2));
  unittest_printf("vec_vspltisw (-1) -> "); PRINT_VEC_SINT(vec_vspltisw(-1));
  unittest_printf("vec_vspltisw ( 0) -> "); PRINT_VEC_SINT(vec_vspltisw(0));
  unittest_printf("vec_vspltisw ( 1) -> "); PRINT_VEC_SINT(vec_vspltisw(1));
  unittest_printf("vec_vspltisw ( 2) -> "); PRINT_VEC_SINT(vec_vspltisw(2));


  // Input arguments: 1 vec_(u)int4 / vec_(u)short8 / vec_(u)char16, (1 UIMM)
  for (uint32_t i = 0; i < sizeof(testInts) / sizeof(int); i++) {
    vec_int4 v1sw = (vec_int4){ testInts[i] };
    vec_short8 v1sh = *(vec_short8*)&v1sw;
    vec_char16 v1sb = *(vec_char16*)&v1sw;

    vec_uint4 v1uw = (vec_uint4){ testInts[i], 0, 0, 0 };
    vec_ushort8 v1uh = *(vec_ushort8*)&v1uw;
    vec_uchar16 v1ub = *(vec_uchar16*)&v1uw;

    // Vector Unpack Instructions
    unittest_printf("vec_vupkhsb ([%02d]) -> ", i); PRINT_VEC_SS(vec_vupkhsb(v1sb));
    unittest_printf("vec_vupkhsh ([%02d]) -> ", i); PRINT_VEC_SINT(vec_vupkhsh(v1sh));
    unittest_printf("vec_vupkhpx ([%02d]) -> ", i); PRINT_VEC_UINT(vec_vupkhpx(v1uh));
    unittest_printf("vec_vupklsb ([%02d]) -> ", i); PRINT_VEC_SS(vec_vupklsb(v1sb));
    unittest_printf("vec_vupklsh ([%02d]) -> ", i); PRINT_VEC_SINT(vec_vupklsh(v1sh));
    unittest_printf("vec_vupklpx ([%02d]) -> ", i); PRINT_VEC_UINT(vec_vupklpx(v1uh));

    // Vector Splat Instructions
    unittest_printf("vec_vspltb ([%02d],15) -> ", i); PRINT_VEC_UB(vec_vspltb(v1ub, 15));
    unittest_printf("vec_vspltb ([%02d], 0) -> ", i); PRINT_VEC_UB(vec_vspltb(v1ub, 0));
    unittest_printf("vec_vspltb ([%02d], 1) -> ", i); PRINT_VEC_UB(vec_vspltb(v1ub, 1));
    unittest_printf("vec_vspltb ([%02d], 2) -> ", i); PRINT_VEC_UB(vec_vspltb(v1ub, 2));
    unittest_printf("vec_vsplth ([%02d],7) -> ", i); PRINT_VEC_US(vec_vsplth(v1uh, 7));
    unittest_printf("vec_vsplth ([%02d], 0) -> ", i); PRINT_VEC_US(vec_vsplth(v1uh, 0));
    unittest_printf("vec_vsplth ([%02d], 1) -> ", i); PRINT_VEC_US(vec_vsplth(v1uh, 1));
    unittest_printf("vec_vsplth ([%02d], 2) -> ", i); PRINT_VEC_US(vec_vsplth(v1uh, 2));
    unittest_printf("vec_vspltw ([%02d],3) -> ", i); PRINT_VEC_UINT(vec_vspltw(v1uw, 3));
    unittest_printf("vec_vspltw ([%02d], 0) -> ", i); PRINT_VEC_UINT(vec_vspltw(v1uw, 0));
    unittest_printf("vec_vspltw ([%02d], 1) -> ", i); PRINT_VEC_UINT(vec_vspltw(v1uw, 1));
    unittest_printf("vec_vspltw ([%02d], 2) -> ", i); PRINT_VEC_UINT(vec_vspltw(v1uw, 2));
  }
  
  // Input arguments: 2 vec_(u)int4 / vec_(u)short8 / vec_(u)char16, (1 UIMM)
  for (uint32_t i = 0; i< sizeof(testInts) / sizeof(int); i++) {
    for (uint32_t j = 0; j<sizeof(testInts) / sizeof(int); j++) {
      vec_int4 v1sw = (vec_int4){ testInts[i] };
      vec_int4 v2sw = (vec_int4){ testInts[j], testInts[j], 0.0, 0.0 };
      vec_short8 v1sh = *(vec_short8*)&v1sw;
      vec_short8 v2sh = *(vec_short8*)&v2sw;

      vec_uint4 v1uw = (vec_uint4){ testInts[i] };
      vec_uint4 v2uw = (vec_uint4){ testInts[j], testInts[j], 0.0, 0.0 };
      vec_ushort8 v1uh = *(vec_ushort8*)&v1uw;
      vec_ushort8 v2uh = *(vec_ushort8*)&v2uw;
      vec_uchar16 v1ub = *(vec_uchar16*)&v1uw;
      vec_uchar16 v2ub = *(vec_uchar16*)&v2uw;

  //    // Vector Pack Instructions
  //    unittest_printf("vec_vpkuhum ([%02d],[%02d])    -> ", i, j); PRINT_VEC_UB(vec_vpkuhum(v1uh,v2uh));
  //    unittest_printf("vec_vpkuwum ([%02d],[%02d])    -> ", i, j); PRINT_VEC_US(vec_vpkuwum(v1uw,v2uw));
  //    unittest_printf("vec_vpkuhus ([%02d],[%02d])    -> ", i, j); PRINT_VEC_UB(vec_vpkuhus(v1uh,v2uh));
  //    unittest_printf("vec_vpkuwus ([%02d],[%02d])    -> ", i, j); PRINT_VEC_US(vec_vpkuwus(v1uw,v2uw));
  //    unittest_printf("vec_vpkshus ([%02d],[%02d])    -> ", i, j); PRINT_VEC_UB(vec_vpkshus(v1sh,v2sh));
  //    unittest_printf("vec_vpkswus ([%02d],[%02d])    -> ", i, j); PRINT_VEC_US(vec_vpkswus(v1sw,v2sw));
  //    unittest_printf("vec_vpkshss ([%02d],[%02d])    -> ", i, j); PRINT_VEC_SB(vec_vpkshss(v1sh,v2sh));
  //    unittest_printf("vec_vpkswss ([%02d],[%02d])    -> ", i, j); PRINT_VEC_SS(vec_vpkswss(v1sw,v2sw));
  //    unittest_printf("vec_vpkpx   ([%02d],[%02d])    -> ", i, j); PRINT_VEC_PX(vec_vpkpx(v1uw,v2uw));

  //    // Vector Merge Instructions
  //    unittest_printf("vec_vmrghb  ([%02d],[%02d])    -> ", i, j); PRINT_VEC_UB(vec_vmrghb(v1ub,v2ub));
  //    unittest_printf("vec_vmrghh  ([%02d],[%02d])    -> ", i, j); PRINT_VEC_US(vec_vmrghh(v1uh,v2uh));
  //    unittest_printf("vec_vmrghw  ([%02d],[%02d])    -> ", i, j); PRINT_VEC_UINT(vec_vmrghw(v1uw,v2uw));
  //    unittest_printf("vec_vmrglb  ([%02d],[%02d])    -> ", i, j); PRINT_VEC_UB(vec_vmrglb(v1ub,v2ub));
  //    unittest_printf("vec_vmrglh  ([%02d],[%02d])    -> ", i, j); PRINT_VEC_US(vec_vmrglh(v1uh,v2uh));
  //    unittest_printf("vec_vmrglw  ([%02d],[%02d])    -> ", i, j); PRINT_VEC_UINT(vec_vmrglw(v1uw,v2uw));

      // Vector Shift Instructions
      unittest_printf("vec_vsl     ([%02d],[%02d])    -> ", i, j); PRINT_VEC_UINT(vec_vsl(v1uw,v2uw));
      unittest_printf("vec_vsldoi  ([%02d],[%02d], 0) -> ", i, j); PRINT_VEC_UINT(vec_vsldoi(v1uw,v2uw, 0));
      unittest_printf("vec_vsldoi  ([%02d],[%02d], 1) -> ", i, j); PRINT_VEC_UINT(vec_vsldoi(v1uw,v2uw, 1));
      unittest_printf("vec_vsldoi  ([%02d],[%02d], 2) -> ", i, j); PRINT_VEC_UINT(vec_vsldoi(v1uw,v2uw, 2));
      unittest_printf("vec_vsldoi  ([%02d],[%02d],15) -> ", i, j); PRINT_VEC_UINT(vec_vsldoi(v1uw,v2uw,15));
      unittest_printf("vec_vslo    ([%02d],[%02d])    -> ", i, j); PRINT_VEC_UB(vec_vslo(v1ub,v2ub));
      unittest_printf("vec_vsr     ([%02d],[%02d])    -> ", i, j); PRINT_VEC_UINT(vec_vsr(v1uw,v2uw));
      unittest_printf("vec_vsro    ([%02d],[%02d])    -> ", i, j); PRINT_VEC_UB(vec_vsro(v1ub,v2ub));
    }
  }
  
  // Input arguments: 3 vec_uint4 / vec_uchar16
  for (uint32_t i = 0; i < sizeof(testInts) / sizeof(int); i++) {
    for (uint32_t j = 0; j < sizeof(testInts) / sizeof(int); j++) {
      for (uint32_t k = 0; k < sizeof(testInts) / sizeof(int); k++) {
        vec_uint4 v1uw = (vec_uint4){ testInts[i] };
        vec_uint4 v2uw = (vec_uint4){ testInts[j], testInts[j], 0.0, 0.0 };
        vec_uint4 v3uw = (vec_uint4){ testFloats[k], 0.0, testFloats[k], 0.0 };
        vec_uchar16 v1ub = *(vec_uchar16*)&v1uw;
        vec_uchar16 v2ub = *(vec_uchar16*)&v2uw;
        vec_uchar16 v3ub = *(vec_uchar16*)&v3uw;

        // Vector Permute Instruction
        //unittest_printf("vec_vperm ([%02d],[%02d],[%02d]) -> ", i, j, k); PRINT_VEC_UB(vec_vperm(v1ub,v2ub,v3ub));

        // Vector Select Instruction
        unittest_printf("vec_vsel  ([%02d],[%02d],[%02d]) -> ", i, j, k); PRINT_VEC_UINT(vec_vsel(v1uw,v2uw,v3uw));
      }
    }
  }

  END_TEST;
}

BEGIN_TEST_CASE(ppc_vpu)
RUN_TEST(test_vpu);
END_TEST_CASE(ppc_vpu)