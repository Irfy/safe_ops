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

#ifdef SAFE_USE_INT128
std::ostream& operator<<(std::ostream& os, safe_uint128_t t) {
    os << std::hex << "0x" << std::setw(16) <<std::setfill('0') << (uint64_t)(t>>64);
    return os << std::setw(16) << std::setfill('0') << (uint64_t)(t) << std::dec;
}

std::ostream& operator<<(std::ostream& os, safe_int128_t t) {
    return os << (safe_uint128_t)t;
}
#endif

const char *progname = "";

struct FakeLogger {
    void log(const char *level, const char *str) { printf("[log]\t%s\t%s\t%s\n", progname, level, str); }
};

int main(int, char **argv) {
    progname = argv[0];
    FakeLogger logger;

    int x = safe(2.5);
    assert(x == 2);
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
printf("    in the following tests, if a 'safe' test were to fail, it would look like this:\n");
    assert(safe(1) > 2);

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
    safe_cmp_assert2(numeric_limits_compat<float>::max(), <, <=, numeric_limits_compat<double>::max(), >, >=);
    safe_cmp_assert2(numeric_limits_compat<double>::max(), <, <=, numeric_limits_compat<long double>::max(), >, >=);
#ifdef SAFE_USE_INT128
    safe_cmp_assert2(numeric_limits_compat<safe_int128_t>::max(), <, <=, numeric_limits_compat<float>::max(), >, >=);

// special casing:
    safe_cmp_assert2(numeric_limits_compat<safe_uint128_t>::max(), >, >=, numeric_limits_compat<float>::max(), <, <=);
    // conversion to float yields inf so it works mathematically correct
    safe_cmp_assert2(numeric_limits_compat<safe_uint128_t>::max(), <, <=, numeric_limits_compat<double>::max(), >, >=);
    safe_cmp_assert2(numeric_limits_compat<safe_uint128_t>::max(), <, <=, numeric_limits_compat<long double>::max(), >, >=);
#endif

printf("ad-hoc tests passed\n");

printf("    expecting two asserts...\n");
    safe_cast_assert<int>(numeric_limits_compat<long>::max());
    safe_cast_assert<int>(numeric_limits_compat<long>::min());

printf("    expecting two log entries...\n");
    safe_cast_log<int>(numeric_limits_compat<long>::max(), &logger);
    safe_cast_log<int>(numeric_limits_compat<long>::min(), &logger);

    try {
        safe_cast_throw<int>(numeric_limits_compat<long>::max());
        assert(0);
    } catch (bad_cast &) {
        printf ("expected bad_cast due to overflow\n");
    }

    try {
        safe_cast_throw<int>(numeric_limits_compat<long>::min());
        assert(0);
    } catch (bad_cast &) {
        printf ("expected bad_cast due to underflow\n");
    }

#define generic_expect(T1, T2, SmallerPositive, SmallerNegative) \
    assert_eq((T1)safe((T2)(0)), 0); \
    assert_eq((T1)safe(numeric_limits_compat<T2>::max()), (T1)numeric_limits_compat<SmallerPositive>::max()); \
    assert_eq((T1)safe(numeric_limits_compat<T2>::lowest()), (T1)numeric_limits_compat<SmallerNegative>::lowest()); \
    assert_eq((T1)numeric_limits_compat<T2>::max(), (T1)numeric_limits_compat<SmallerPositive>::max()); \
    assert_eq((T1)numeric_limits_compat<T2>::lowest(), (T1)numeric_limits_compat<SmallerNegative>::lowest())

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

printf("non-truncating tests passed\n");

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
    expect_lower_higher2(float, safe_uint128_t);
    expect_smaller_larger2(safe_int128_t, float);
/// safe_int128_t is less than float, but test special-case handling regardless

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
