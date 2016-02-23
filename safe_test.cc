// Copyright (c) 2016 Irfan Adilovic
// This software is licensed under the terms of the MIT license.
// See LICENSE for the text of the license.

#include <iostream>
#include <iomanip>
#include <limits>
#include <stdint.h>
#include <cstdio>
#include <typeinfo>
#include <inttypes.h>

using std::cout;
using std::cerr;
using std::endl;
using std::bad_cast;

#undef assert // for testability
//#define assert(e) ((e) ? (void)0 : ((void)printf ("%s:%u: failed assertion `%s'\n", __FILE__, __LINE__, #e)))
#define assert(e) ((e) ? (void)0 : ((void)printf ("failed assertion `%s'\n", #e)))
#define assert_op(e1, op, e2) ((e1) op (e2) ? (void)0 : ((void)(cout << "failed assertion `" << #e1 << " " #op " " << #e2 << ": [" << (e1) << "] isn't " #op " [" << (e2) << "]\n")))
#define assert_eq(e1, e2) assert_op(e1, ==, e2)
#define assert_ne(e1, e2) assert_op(e1, !=, e2)

#include "safe_ops.h"
using namespace safe_ops;

#define MAX(T) numeric_limits_compat<T>::max()
#define MIN(T) numeric_limits_compat<T>::lowest()

#ifdef SAFE_USE_INT128
std::ostream& operator<<(std::ostream& os, uint128_t t) {
    os << std::hex << "0x" << std::setw(16) <<std::setfill('0') << (uint64_t)(t>>64);
    return os << std::setw(16) << std::setfill('0') << (uint64_t)(t) << std::dec;
}

std::ostream& operator<<(std::ostream& os, int128_t t) {
    return os << (uint128_t)t;
}
#endif


#if __cplusplus < 201103L
struct Lambda {
    void operator ()(int result) { cout << "lambda: " << (result < 0 ? "under" : "over") << "flow detected\n"; };
} lambda;
#endif

const char *progname = "";

struct FakeLogger {
    void log(const char *level, const char *str) { printf("[log]\t%s\t%s\t%s\n", progname, level, str); }
};

int main(int, char **argv) {
    progname = argv[0];

printf("    If a 'safe' test were to fail, the assertion text would contain 'safe(' and would be easy to identify. E.g.:\n");
    assert(safe(1) > 2);

// precision tests
safe_ops::intmax_t intmax = MAX(safe_ops::intmax_t);
assert(safe(intmax-1) < safe((safe_ops::uintmax_t)(intmax))); // promotion to float/double due to signed/unsigned comparison
assert(safe(intmax-2)+1 < safe(intmax)); // promotion to float/double due to addition

#define assert_safe_eq(s, result) assert_eq((s).value(), result)

#define safe_arith_assert_impl(x, arith_op, y, z, nosafe_eq_op) \
    assert_safe_eq(safe(x) arith_op y, z); \
    assert_safe_eq(x arith_op safe(y), z); \
    assert_op(x arith_op (y), nosafe_eq_op, (z))

#define safe_arith_assert(x, op, y, z, nosafe_eq_op) \
    safe_arith_assert_impl(x, op, y, z, nosafe_eq_op)

#define safe_arith_assert_comm(x, op, y, z, nosafe_eq_op) \
    safe_arith_assert_impl(x, op, y, z, nosafe_eq_op); \
    safe_arith_assert_impl(y, op, x, z, nosafe_eq_op)

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wtype-limits"

#define gen_safe_arith_tests_nomod(T1, T2, AddMultT, SubT, DivModT) \
    safe_arith_assert_comm(MAX(T1), +, MAX(T2),       (AddMultT)(MAX(T1)) + MAX(T2),        !=); \
    safe_arith_assert_comm(MIN(T1), +, MIN(T2),       (AddMultT)(MIN(T1)) + MIN(T2),        !=); \
    safe_arith_assert(     MIN(T1), -, MAX(T2),       (SubT)    (MIN(T1)) - MAX(T2),        !=); \
    safe_arith_assert(     MAX(T1), -, MIN(T2),       (SubT)    (MAX(T1)) - MIN(T2),        !=); \
    safe_arith_assert_comm(MAX(T1), *, MAX(T2),       (AddMultT)(MAX(T1)) * MAX(T2),        !=); \
    safe_arith_assert_comm(MIN(T1), *, MIN(T2),       (AddMultT)(MIN(T1)) * MIN(T2),        !=); \
    safe_arith_assert(     MIN(T1), /, (T2)1,         (DivModT) (MIN(T1)) / (T2)1  ,        ==); \
    safe_arith_assert(     MAX(T1), /, (T2)-1,        (DivModT) (MAX(T1)) / (T2)-1 ,        ==)

#define gen_safe_arith_tests(T1, T2, AddMultT, SubT, DivModT) \
    gen_safe_arith_tests_nomod(T1, T2, AddMultT, SubT, DivModT); \
    safe_arith_assert(     MIN(T1), %, (T2)1,         (DivModT) (MIN(T1)) % (T2)1  ,        ==); \
    safe_arith_assert(     MAX(T1), %, (T2)-1,        (DivModT) (MAX(T1)) % (T2)-1 ,        ==)

// there will be cases where T2 is unsigned and div/mod with (unsigned)-1 is tested for.
// any failures there shall be ignored.

#define gen_safe_arith_tests_float2(T1, T2, AddMultT, SubT, DivModT) \
    gen_safe_arith_tests_nomod(T1, T2, AddMultT, SubT, DivModT); \
    safe_arith_assert(     MIN(T1), /, (T2)1e-30,       (DivModT) (MIN(T1)) / (T2)1e-30  ,        ==); \
    safe_arith_assert(     MAX(T1), /, (T2)-1e-30,      (DivModT) (MAX(T1)) / (T2)-1e-30 ,        ==)

// the arithmetic tests generator states basically:
//  1) + and * shall be tested commutatively
//  2) +, * and - shall always fail to produce a mathematically correct result without safe_t
//  3) / and % shall always produce the same result as safe_t, which is assumed to be mathematically correct
// to fulfill 2), appropriate MIN/MAX values are always chosen to overflow non-safe_t calculations.

printf("    safe_arith test int/int: no asserts expected\n");
gen_safe_arith_tests(int, int, long, long, int);
printf("    safe_arith test unsigned/unsigned: 5 asserts expected, unsigned lowest()\n");
gen_safe_arith_tests(unsigned, unsigned, unsigned long, long, unsigned);
printf("    safe_arith test int/unsigned: 4 asserts expected, unsigned lowest() and negative division\n");
gen_safe_arith_tests(int, unsigned, long, long, long);
printf("    safe_arith test unsigned/int: 3 asserts expected, unsigned lowest() and negative division\n");
gen_safe_arith_tests(unsigned, int, long, long, long);
printf("    safe_arith test int/long: no asserts expected\n");
gen_safe_arith_tests(int, long, float, float, long);
printf("    safe_arith test long/int: no asserts expected\n");
gen_safe_arith_tests(long, int, float, float, long);
printf("    safe_arith test float/uint64_t: 8 asserts expected, int too small to make a difference and 0-multiplication\n");
gen_safe_arith_tests_nomod(float, uint64_t, double, double, float);
printf("    safe_arith test uint64_t/float: 9 asserts expected, int too small to make a difference, 0-multiplication and non-zero division by small number\n");
gen_safe_arith_tests_float2(uint64_t, float, double, double, double);
printf("    safe_arith test float/float: 2 asserts expected, non-zero division by small number\n");
gen_safe_arith_tests_float2(float, float, double, double, double);
printf("    safe_arith test double/double: 2 asserts expected, non-zero division by small number\n");
gen_safe_arith_tests_float2(double, double, long double, long double, long double);
#ifdef SAFE_USE_INT128
printf("    safe_arith test float/int128_t: no asserts expected\n");
gen_safe_arith_tests_nomod(float, int128_t, double, double, float); // float/int128 fits in float
printf("    safe_arith test int128_t/float: 2 asserts expected, non-zero division by small number\n");
gen_safe_arith_tests_float2(int128_t, float, double, double, double);
printf("    safe_arith test float/uint128_t: 6 asserts expected, unsigned lowest ops and unsigned -1 division\n");
gen_safe_arith_tests_nomod(float, uint128_t, double, double, double); // float/uint128 fits in double
printf("    safe_arith test uint128_t/float: 7 asserts expected, unsigned lowest ops and divisions\n");
gen_safe_arith_tests_float2(uint128_t, float, double, double, double);
#endif
#pragma GCC diagnostic pop

// ad-hoc cross-functional test: safe_arith + policy_throw + safe-safe operator
    try {
        assert(safe(1) + safe(1) == safe(2)); // test both cmp and arith safe-safe operators
        (int)(safe(MAX(int)).pthrow() + safe(1).passert());
    } catch (...) {
        cout << "ad-hoc test 'policy vs operator+': caught expected bad_cast: MAX(int) + 1 is no longer an int\n";
    }

// cout << "# Testing " << x << ' ' << #op << ' ' << y << endl;
#define safe_cmp_assert_impl(x, op, y) \
    assert(x op y); \
    assert(safe(x) op y); \
    assert(x op safe(y));

#define safe_cmp_assert(x, op, y, rev_op) \
    safe_cmp_assert_impl(x, op, y) \
    safe_cmp_assert_impl(y, rev_op, x)

#define safe_cmp_assert2(x, op, op2, y, rev_op, rev_op2) \
    safe_cmp_assert(x, op, y, rev_op) \
    safe_cmp_assert(x, op2, y, rev_op2)

cerr << "sizeof(int): " << sizeof(int) << endl;
cerr << "sizeof(long): " << sizeof(long) << endl;

cerr << std::boolalpha;
#define cerr_trait(type, trait) \
    cerr << "std::" #trait "<" #type ">::value : " << std::trait<type>::value << endl
#define cerr_traits(type) \
    cerr << "sizeof(" #type "): " << sizeof(type) << endl; \
    cerr_trait(type, is_signed); \
    cerr_trait(type, is_unsigned)

// cerr_traits(long long);
// cerr_traits(unsigned long long);

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsign-compare"
printf("    expecting 1u >/>= int8_t(-1) to fail, but safe variants to succeed...\n");
    safe_cmp_assert2(1u, >, >=, int8_t(-1), <, <=);

printf("    expecting 0u >/>= -1 to fail, but safe variants to succeed...\n");
    safe_cmp_assert2(0u, >, >=, -1, <, <=);

printf("    expecting 1ul >/>= -1 to fail, but safe variants to succeed...\n");
    safe_cmp_assert2(1ul, >, >=, -1, <, <=);

printf("    expecting 4294967296ul >/>= -1 to fail, but safe variants to succeed...\n");
    safe_cmp_assert2(4294967296ul, >, >=, -1, <, <=);

printf("    expecting 1ul >/>= -1l to fail, but safe variants to succeed...\n");
    safe_cmp_assert2(1ul, >, >=, -1l, <, <=);

printf("    expecting 2147483648u >/!= -2147483648 to fail, but safe variants to succeed...\n");
    safe_cmp_assert2(2147483648u, >, !=, (-2147483647-1), <, !=);
    //NB: http://stackoverflow.com/questions/35130890/understanding-231-and-231-integer-promotion

printf("    expecting 4294967295u >/!= -1 to fail, but safe variants to succeed...\n");
    safe_cmp_assert2(4294967295u, >, !=, -1, <, !=);
#pragma GCC diagnostic pop

printf("    no failures expected in any of the floating point comparisons (neither native nor 'safe')...\n");
    safe_cmp_assert2(MAX(float), <, <=, MAX(double), >, >=);
    safe_cmp_assert2(MAX(double), <, <=, MAX(long double), >, >=);
#ifdef SAFE_USE_INT128
    safe_cmp_assert2(MAX(int128_t), <, <=, MAX(float), >, >=);

// special casing:
    safe_cmp_assert2(MAX(uint128_t), >, >=, MAX(float), <, <=);
    // conversion to float yields inf so it works mathematically correct
    safe_cmp_assert2(MAX(uint128_t), <, <=, MAX(double), >, >=);
    safe_cmp_assert2(MAX(uint128_t), <, <=, MAX(long double), >, >=);
#endif

printf("safe_cmp tests passed\n");

    int i;
    int result = 0;
    const long lmax = MAX(long);
    const long lmin = MIN(long);

printf("    safe_cast_assert: expecting two asserts...\n");
    i = safe(lmax).passert(); // usage through safe generator + .policy modifier
    i = safe_t<long, policy_assert>(lmin); // usage through direct safe_t instantiation
    i = safe_cast_assert<int>(0l); // usage through safe_cast_* helpers

printf("    safe_cast_result: expecting no asserts...\n");
    result = 0;
    i = safe(lmax).presult(&result);
    assert(result == 1);
    result = 0;
    i = safe_t<long, policy_result, int*>(lmin, &result);
    assert(result == -1);
    result = 0;
    safe_cast_result<int>(0l, &result);
    assert(result == 0); // actually: unmodified

printf("    safe_cast_lambda: expecting two 'lambda: ...' messages...\n");
#if __cplusplus >= 201103L
    auto lambda = [](int result){ cout << "lambda: " << (result < 0 ? "under" : "over") << "flow detected\n"; };
    i = safe(lmax).pexec(lambda);
    i = safe_t<long, policy_exec, decltype(lambda)>(lmin, lambda);
#else
    // look before int main(), in global scope, there is a conditional definition of lambda old-style
    i = safe(lmax).pexec(lambda);
    i = safe_t<long, policy_exec, Lambda>(lmin, lambda);
#endif
    safe_cast_exec<int>(0l, lambda);

printf("    safe_cast_log: expecting two log entries...\n");
    FakeLogger logger;
    i = safe(lmax).plog(&logger);
    i = safe_t<long, policy_log, FakeLogger*>(lmin, &logger);
    safe_cast_log<int>(0l, &logger);

printf("    safe_cast_throw: expecting two bad_casts...\n");
    try {
        i = safe(lmax).pthrow();
        assert("unreachable after throw" == NULL);
    } catch (bad_cast &) {
        printf ("bad_cast caught due to overflow\n");
    }

    try {
        i = safe_t<long, policy_throw>(lmin);
        assert("unreachable after throw" == NULL);
    } catch (bad_cast &) {
        printf ("bad_cast caught due to underflow\n");
    }

    safe_cast_throw<int>(0l);

    (void)i; // silence compiler warnings

printf("non-truncating safe_cast functional tests passed\n");
printf("truncating full-coverage tests following:\n");

#define generic_expect(T1, T2, SmallerPositive, SmallerNegative) \
    assert_eq((T1)safe((T2)(0)), 0); \
    assert_eq((T1)safe(MAX(T2)), (T1)MAX(SmallerPositive)); \
    assert_eq((T1)safe(MIN(T2)), (T1)MIN(SmallerNegative)); \
    assert_eq((T1)MAX(T2), (T1)MAX(SmallerPositive)); \
    assert_eq((T1)MIN(T2), (T1)MIN(SmallerNegative))

// the latter two assertions will obviously produce expected failures

#define expect_smaller_larger(Smaller, Larger) generic_expect(Smaller, Larger,  Smaller, Smaller)
#define expect_larger_smaller(Larger, Smaller) generic_expect(Larger,  Smaller, Smaller, Smaller)
#define expect_lower_higher(Lower, Higher)     generic_expect(Lower,   Higher,  Lower,   Higher)
#define expect_higher_lower(Higher, Lower)     generic_expect(Higher,  Lower,   Lower,   Higher)

#define expect_smaller_larger2(Smaller, Larger) \
    expect_smaller_larger(Smaller, Larger); \
    expect_larger_smaller(Larger, Smaller)

#define expect_lower_higher2(Lower, Higher) \
    expect_lower_higher(Lower, Higher); \
    expect_higher_lower(Higher, Lower)

    expect_smaller_larger2(float, long double);
    expect_smaller_larger2(int, double);
    expect_smaller_larger2(unsigned, double);

printf("floating point tests passed\n");

/// naive size comparison is "wrong"
    expect_smaller_larger2(uint64_t, float);
    expect_smaller_larger2(int64_t, float);

/// naive size comparison is 'equal'
    expect_smaller_larger2(uint32_t, float);
    expect_smaller_larger2(int32_t, float);

printf("naive sizeof tests passed (float <-> int64/32_t)\n");

/// ints greater than float
#ifdef SAFE_USE_INT128
    expect_lower_higher2(float, uint128_t);
    expect_smaller_larger2(int128_t, float);
/// int128_t is less than float, but test special-case handling regardless

printf("extreme sizeof tests passed (float <-> safe_[u]int128_t)\n");

#endif

/// integers
    expect_smaller_larger(int, int);
    expect_smaller_larger(unsigned, unsigned);
    expect_lower_higher2(int, unsigned);

printf("same size integral tests passed\n");

    expect_smaller_larger2(int32_t, int64_t);
    expect_smaller_larger2(uint32_t, uint64_t);

    expect_smaller_larger2(uint32_t, int64_t);
    expect_lower_higher2(int32_t, uint64_t);

printf("different size integral tests passed\n");

    return 0;
}
