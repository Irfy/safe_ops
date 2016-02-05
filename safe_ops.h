// Copyright (c) 2016 Irfan Adilovic
// This software is licensed under the terms of the MIT license.
// See LICENSE for the text of the license.

#ifndef _SAFE_COMMON_H_
#define _SAFE_COMMON_H_

#include <typeinfo> // std::bad_cast for policy_throw
#include <limits>
#include <stdint.h> // <cstdint> is not c++03 conformant

#ifndef assert // use user-provided assert macro -- useful for testing
#include <cassert> // for policy_assert
#endif

#define IF(expr)            typename std::enable_if<(expr), void>::type
#define IS(T,U)             std::is_same<T,U>::value
#define ISNT(T,U)           !std::is_same<T,U>::value
#define SIZE(T,cmp,U)       (sizeof(T) cmp sizeof(U))

#define SIGNED(T)           std::is_signed<T>::value
#define UNSIGNED(T)         std::is_unsigned<T>::value
#define INTEGRAL(T)         std::is_integral<T>::value
#define FLOATING(T)         std::is_floating_point<T>::value

#define BOTH_SIGNED         (SIGNED(Target) && SIGNED(Source))
#define SAME_INTEGRALITY    (INTEGRAL(Target) == INTEGRAL(Source))
#define TARGET_LT           SIZE(Target, <, Source)
#define TARGET_LE           SIZE(Target, <=, Source)
#define TARGET_GT           SIZE(Target, >, Source)
#define TARGET_GE           SIZE(Target, >=, Source)
#define INT_GT_FLOAT(T,U)   (IS(U,float) && INTEGRAL(T) && sizeof(T) > 16 && SIGNED(T)) // __int128 fits in float completely. so must be larger than that
#define UINT_GT_FLOAT(T,U)  (IS(U,float) && INTEGRAL(T) && sizeof(T) >= 16 && UNSIGNED(T)) // unsigned __int128 doesn't fit in float. so greater-or-equal
#define INTEGRAL_GT_FLOAT(T,U)  (INT_GT_FLOAT(T,U) || UINT_GT_FLOAT(T,U))

#define MAX(T) numeric_limits_compat<T>::max()
#define MIN(T) numeric_limits_compat<T>::lowest()

#if !defined(__STRICT_ANSI__) && defined(_GLIBCXX_USE_INT128)
#define SAFE_USE_INT128
namespace safe_ops {
__extension__ typedef __int128 int128_t;
__extension__ typedef unsigned __int128 uint128_t;
}
#endif

#if __cplusplus >= 201103L
#   include <type_traits>
#else

/******************************************************************************
 * Compatibility traits for c++98 mode (present in c++11 and later)
 *****************************************************************************/

namespace std {
template<bool value, typename T>
struct enable_if {};

    template<typename T>
    struct enable_if<true, T> {
        typedef T type;
    };

template<bool value, typename T, typename F>
struct conditional {
    typedef F type;
};

    template<typename T, typename F>
    struct conditional<true, T, F> {
        typedef T type;
    };

template<typename T, typename U>
struct is_same {
    static const bool value = false;
};

    template<typename T>
    struct is_same<T,T> {
        static const bool value = true;
    };

template<typename T>
struct is_floating_point {
    static const bool value = false;
};

#define gen_floating(type) \
    template<> \
    struct is_floating_point<type> { \
        static const bool value = true; \
    }

    gen_floating(float);
    gen_floating(double);
    gen_floating(long double);

#undef gen_floating

template<typename T>
struct is_integral {
    static const bool value = false;
};

#define gen_integral(type) \
    __extension__ template<> \
    struct is_integral<signed type> { \
        static const bool value = true; \
    }; \
    __extension__ template<> \
    struct is_integral<unsigned type> { \
        static const bool value = true; \
    }

    gen_integral(char);
    gen_integral(short);
    gen_integral(int);
    gen_integral(long);
#ifdef SAFE_USE_INT128
    gen_integral(__int128);
#endif

#undef gen_integral

template<typename T>
struct is_signed {
    static const bool value = false;
};

#define gen_signed(type) \
    template<> \
    struct is_signed<type> { \
        static const bool value = true; \
    }

    gen_signed(signed char);
    gen_signed(signed int);
    gen_signed(signed short);
    gen_signed(signed long);
    gen_signed(float);
    gen_signed(double);
    gen_signed(long double);
#ifdef SAFE_USE_INT128
    gen_signed(safe_ops::int128_t);
#endif

#undef gen_signed

template<typename T>
struct is_unsigned {
    static const bool value = INTEGRAL(T) && !SIGNED(T);
};

template<typename T>
struct make_signed {
    typedef T type;
};

#define gen_make_signed(type_) \
    __extension__ template<> \
    struct make_signed<unsigned type_> { \
        typedef signed type_ type; \
    }

    gen_make_signed(char);
    gen_make_signed(int);
    gen_make_signed(short);
    gen_make_signed(long);
#ifdef SAFE_USE_INT128
    gen_make_signed(__int128);
#endif

#undef gen_make_signed
}
#endif

namespace safe_ops {

/******************************************************************************
 * safe_cast implementation
 *****************************************************************************/

/* There are five classes of conversions, represented by the following five definitions
 * The template-boolean expressions used herein have been heavily simplified (as in
 * "Karnaugh maps") and may not appear obvious on first inspection.
 */

#define CASTABLE_ALWAYS(Source,Target) \
IF( (SAME_INTEGRALITY && TARGET_GT && (SIGNED(Target) || UNSIGNED(Source))) \
 || (FLOATING(Target) && INTEGRAL(Source) && !INTEGRAL_GT_FLOAT(Source,Target)) \
 || INT_GT_FLOAT(Target,Source) \
)

#define CASTABLE_WHEN_POSITIVE(Source,Target) \
IF( (SAME_INTEGRALITY && TARGET_GE && UNSIGNED(Target) && SIGNED(Source)) \
 || UINT_GT_FLOAT(Target,Source) \
)

#define CASTABLE_FROM_UNSIGNED_WHEN_SMALL_ENOUGH(Source,Target) \
IF( (SAME_INTEGRALITY && TARGET_LE && UNSIGNED(Source)) \
 || UINT_GT_FLOAT(Source,Target) \
)

#define CASTABLE_WHEN_POSITIVE_AND_SMALL_ENOUGH(Source,Target) \
IF( (SAME_INTEGRALITY && TARGET_LT && UNSIGNED(Target) && SIGNED(Source)) \
 || (INTEGRAL(Target) && FLOATING(Source) && UNSIGNED(Target) && !UINT_GT_FLOAT(Target,Source)) \
)

#define CASTABLE_WHEN_SMALL_ENOUGH(Source,Target) \
IF( (SAME_INTEGRALITY && TARGET_LE && BOTH_SIGNED) \
 || (INTEGRAL(Target) && FLOATING(Source) && SIGNED(Target) && !INT_GT_FLOAT(Target,Source)) \
 || INT_GT_FLOAT(Source,Target) \
)

#if __cplusplus < 201103L
// add the missing lowest() definition to our compat version of numeric_limits
template<typename T, typename Enable = void>
struct numeric_limits_compat: public std::numeric_limits<T> {
    static T lowest() { return std::numeric_limits<T>::min(); }
};
    template<typename T>
    struct numeric_limits_compat<T,IF(FLOATING(T))> : public std::numeric_limits<T> {
        static T lowest() { return -std::numeric_limits<T>::max(); }
    };
#else
// for c++11, make the compat struct identical to the original struct
template<typename T>
struct numeric_limits_compat: public std::numeric_limits<T> {};
#endif

// safe_cast_impl

template<typename Target, typename Source, template<typename, typename> class Policy, typename PolicyArg = void*, typename Enable = void>
struct safe_cast_impl {
#if __cplusplus >= 201103L
    static_assert(std::is_fundamental<Target>::value, "Target type of overflow checking is not supported");
    static_assert(std::is_fundamental<Source>::value, "Source type for overflow checking is not supported");
#endif // for c++98, the call to cast() will simply fail to bind, instead of getting a nice error message
};

template<typename Target, typename Source, template<typename, typename> class Policy, typename PolicyArg>
struct safe_cast_impl<Target, Source, Policy, PolicyArg, CASTABLE_ALWAYS(Source,Target)> {
    static Target cast(Source x, PolicyArg = PolicyArg()) {
        return static_cast<Target>(x);
    }
};

template<typename Target, typename Source, template<typename, typename> class Policy, typename PolicyArg>
struct safe_cast_impl<Target, Source, Policy, PolicyArg, CASTABLE_WHEN_POSITIVE(Source,Target)> {
    static Target cast(Source x, PolicyArg policy_arg = PolicyArg()) {
        if (x < 0)
            return Policy<Target, PolicyArg>::handle_underflow(policy_arg);
        else
            return static_cast<Target>(x);
    }
};

template<typename Target, typename Source, template<typename, typename> class Policy, typename PolicyArg>
struct safe_cast_impl<Target, Source, Policy, PolicyArg, CASTABLE_FROM_UNSIGNED_WHEN_SMALL_ENOUGH(Source,Target)> {
    static Target cast(Source x, PolicyArg policy_arg = PolicyArg()) {
        return static_cast<Source>(MAX(Target)) < x ? Policy<Target, PolicyArg>::handle_overflow(policy_arg) : static_cast<Target>(x);
    }
};

template<typename Target, typename Source, template<typename, typename> class Policy, typename PolicyArg>
struct safe_cast_impl<Target, Source, Policy, PolicyArg, CASTABLE_WHEN_POSITIVE_AND_SMALL_ENOUGH(Source,Target)> {
    static Target cast(Source x, PolicyArg policy_arg = PolicyArg()) {
        if (x < 0)
            return Policy<Target, PolicyArg>::handle_underflow(policy_arg);
        else
            return static_cast<Source>(MAX(Target)) < x ? Policy<Target, PolicyArg>::handle_overflow(policy_arg) : static_cast<Target>(x);
    }
};

template<typename Target, typename Source, template<typename, typename> class Policy, typename PolicyArg>
struct safe_cast_impl<Target, Source, Policy, PolicyArg, CASTABLE_WHEN_SMALL_ENOUGH(Source,Target)> {
    static Target cast(Source x, PolicyArg policy_arg = PolicyArg()) {
        if (x < 0)
            return static_cast<Source>(MIN(Target)) > x ? Policy<Target, PolicyArg>::handle_underflow(policy_arg) : static_cast<Target>(x);
        else
            return static_cast<Source>(MAX(Target)) < x ? Policy<Target, PolicyArg>::handle_overflow(policy_arg) : static_cast<Target>(x);
    }
};

#undef CASTABLE_ALWAYS
#undef CASTABLE_WHEN_POSITIVE
#undef CASTABLE_FROM_UNSIGNED_WHEN_SMALL_ENOUGH
#undef CASTABLE_WHEN_POSITIVE_AND_SMALL_ENOUGH
#undef CASTABLE_WHEN_SMALL_ENOUGH

#undef BOTH_SIGNED
#undef SAME_INTEGRALITY
#undef TARGET_LT
#undef TARGET_LE
#undef TARGET_GT
#undef TARGET_GE
#undef INT_GT_FLOAT
#undef UINT_GT_FLOAT
#undef INTEGRAL_GT_FLOAT

template<typename Target, typename PolicyArg = void*>
struct policy_truncate {
    static Target handle_overflow(PolicyArg) { return MAX(Target); }
    static Target handle_underflow(PolicyArg) { return MIN(Target); }
};

template<typename Target, typename Result = int*>
struct policy_result {
    static Target handle_overflow(Result result) { *result = 1; return MAX(Target); }
    static Target handle_underflow(Result result) { *result = -1; return MIN(Target); }
};

template<typename Target, typename Lambda>
struct policy_exec {
    static Target handle_overflow(Lambda lambda) { lambda(1); return MAX(Target); }
    static Target handle_underflow(Lambda lambda) { lambda(-1); return MIN(Target); }
};

template<typename Target, typename PolicyArg = void*>
struct policy_assert {
    static Target handle_overflow(PolicyArg) { assert("Overflow detected" == NULL); return MAX(Target); }
    static Target handle_underflow(PolicyArg) { assert("Underflow detected" == NULL); return MIN(Target); }
};

template<typename Target, typename PolicyArg = void*>
struct policy_throw {
    static Target handle_overflow(PolicyArg) { throw std::bad_cast(); }
    static Target handle_underflow(PolicyArg) { throw std::bad_cast(); }
};

template<typename Target, typename Logger>
struct policy_log {
    static Target handle_overflow(Logger logger) { logger->log ("E", "Overflow detected"); return MAX(Target); }
    static Target handle_underflow(Logger logger) { logger->log ("E", "Underflow detected"); return MIN(Target); }
};

#undef MAX
#undef MIN

// main entry point for casting

template<typename Target, typename Source>
Target safe_cast_trunc(Source x) {
    return safe_cast_impl<Target, Source, policy_truncate, void*>::cast(x);
}

template<typename Target, typename Source, typename Result>
Target safe_cast_result(Source x, Result result) {
    return safe_cast_impl<Target, Source, policy_result, Result>::cast(x, result);
}

template<typename Target, typename Source, typename Lambda>
Target safe_cast_exec(Source x, Lambda lambda) {
    return safe_cast_impl<Target, Source, policy_exec, Lambda>::cast(x, lambda);
}

template<typename Target, typename Source>
Target safe_cast_assert(Source x) {
    return safe_cast_impl<Target, Source, policy_assert, void*>::cast(x);
}

template<typename Target, typename Source>
Target safe_cast_throw(Source x) {
    return safe_cast_impl<Target, Source, policy_throw, void*>::cast(x);
}

template<typename Target, typename Source, typename Logger>
Target safe_cast_log(Source x, Logger logger) {
    return safe_cast_impl<Target, Source, policy_log, Logger>::cast(x, logger);
}

/******************************************************************************
 * safe_cmp implementation
 *****************************************************************************/

/*
C++14 Standard Section 5, paragraph 10 [expr]:
1 If either operand is of type long double, the other shall be converted to long double. => nothing to do
2 Otherwise, if either operand is double, the other shall be converted to double. => nothing to do
3! Otherwise, if either operand is float, the other shall be converted to float. => handle unsigned __int128
4 Otherwise, the integral promotions (4.5 [conv.prom]) shall be performed on both operands. Then the following
rules shall be applied to the promoted operands:
  5 If both operands have the same type, no further conversion is needed. => nothing to do
  6 Otherwise, if both operands have signed integer types or both have unsigned integer types, the
  operand with the type of lesser integer conversion rank shall be converted to the type of the
  operand with greater rank. => nothing to do
  7! Otherwise, if the operand that has unsigned integer type has rank greater than or equal to the
  rank of the type of the other operand, the operand with signed integer type shall be converted to
  the type of the operand with unsigned integer type. => handle sign underflow due to conversion
  8 Otherwise, if the type of the operand with signed integer type can represent all of the values of
  the type of the operand with unsigned integer type, the operand with unsigned integer type shall
  be converted to the type of the operand with signed integer type. => nothing to do
  -- i.e. signed integer type has rank greater than the rank of the other, which must be unsigned
  -- *and* signed integer type is greater in size than the other type
  9! Otherwise, both operands shall be converted to the unsigned integer type corresponding to the
  type of the operand with signed integer type. => handle sign underflow due to conversion
  -- i.e. signed integer type has rank greater than the rank of the other, which must be unsigned
  -- *and* signed integer type is equal in size to the other type; this happens when sizeof(int) == sizeof(long)
  -- or sizeof(long) == sizeof(long long). Language guarantees that rank(long long) > rank(long) > rank(int).
  -- For all practical purposes, this case can be ignored, and case 7 applied as if type sizes are compared.

Case summary:

- floating
- integers
  - same rank, same sign => 5
  - same rank, different sign => 7 (signed -> unsigned conversion, possible sign underflow)
  - T1 > T2, same sign => 6
  - S > U => 8 (unsigned -> signed, no loss)
  - U > S => 7 (signed -> unsigned, possible sign underflow)
  - obscure case: when types of different rank have same size: cygwin64 has that for long long and long.
    in this case, S > U in rank, but S == U in size, then => 9 (same as 7: signed -> unsigned, possible sign underflow)
    [see also http://stackoverflow.com/questions/26963429/c-usual-arithmetic-conversions-last-case-in-standard]

Full summary:

- For integral types, the language *forces* integer promotion first, in all cases. So the minimum size of any argument is int or unsigned int.
- Whenever an overflow/underflow can be avoided, the language rules avoid it (e.g. smaller unsigned to larger signed promotion, etc.)
- The *only* configuration where an overflow/underflow cannot be avoided is when the types have different sign and the signed type is smaller or equal in size than the unsigned type.
  + In that case, underflow is preferred over overflow: signed => unsigned conversion, i.e., large positive values are *never* cut off due to conversion.

*/

// next_larger<T>: the smallest type that is strictly larger than T and can contain all its types.
// if T is unsigned, and a larger unsigned type exists, the result will be that unsigned type
// in all other cases, a signed or floating type will be returned

template<typename T, typename Enable = void>
struct next_larger {};

// double and long double both go to long double
template<typename T>
struct next_larger<T, IF(IS(T, double) || IS(T, long double))> {
    typedef long double type;
};

// float, uint128_t and larger integrals : double
template<typename T>
struct next_larger<T, IF(IS(T, float) // float
                         || (INTEGRAL(T)
                             && (sizeof(T) > 128/8 // [u]ints larger than 128 bits
                                 || (UNSIGNED(T) && sizeof(T) == 128/8))))> { // uint128_t
    typedef double type;
};

// generate all versions for the usual integral types

#define gen_next_larger(bits, bits2) \
template<typename T> \
struct next_larger<T, IF(sizeof(T) == bits/8 && SIGNED(T) && INTEGRAL(T))> { \
    typedef int ## bits2 ## _t type; \
}; \
template<typename T> \
struct next_larger<T, IF(sizeof(T) == bits/8 && UNSIGNED(T) && INTEGRAL(T))> { \
    typedef uint ## bits2 ## _t type; \
};

gen_next_larger(8, 16)
gen_next_larger(16, 32)
gen_next_larger(32, 64)
#ifdef SAFE_USE_INT128 // we need it by name, can't just use sizeof here
gen_next_larger(64, 128)
#else // when int128 unavailable, make 64-bits go to float
template<typename T>
struct next_larger<T, IF(sizeof(T) == 64/8 && INTEGRAL(T))> {
    typedef float type;  // can't use int128 types, go directly to float
};
#endif
// int128_t is a special case -- #ifdef guard not needed, we only use sizeof here
template<typename T>
struct next_larger<T, IF(INTEGRAL(T) && SIGNED(T) && sizeof(T) == 128/8)> {
    typedef float type;
};

#undef gen_next_larger

// next_larger2<T, U>: smallest type strictly larger than both T and U which can contain all values of both types.
// if both are unsigned integrals, and a larger-than-both unsigned type exists, will that unsigned type be returned.
// otherwise a signed or floating type will be returned.

template<typename T, typename U, typename Enable = void>
struct next_larger2 {};

#define SAME_ATTR1_OR_ATTR2(T, U, ATTR1, ATTR2) ((ATTR1(T) && ATTR1(U)) || (ATTR2(T) && ATTR2(U)))
#define SAME_INTEGRALITY(T, U) SAME_ATTR1_OR_ATTR2(T, U, INTEGRAL, FLOATING)
#define SAME_SIGNEDNESS(T, U) SAME_ATTR1_OR_ATTR2(T, U, SIGNED, UNSIGNED)
#define COMPARABLE(T, U) (SAME_INTEGRALITY(T, U) && SAME_SIGNEDNESS(T, U))

template<typename T, typename U>
struct next_larger2<T, U, IF(COMPARABLE(T, U))> {
    typedef typename next_larger<IFELSE(SIZE(T, >=, U), T, U)>::type type;
};

template<typename T, typename U>
struct next_larger2<T, U, IF(SAME_INTEGRALITY(T, U) && !SAME_SIGNEDNESS(T, U))>
    : next_larger2<typename std::make_signed<T>::type, typename std::make_signed<U>::type> {
};

template<typename T, typename U>
struct next_larger2<T, U, IF(!SAME_INTEGRALITY(T, U))>
    : next_larger<IFELSE(FLOATING(T), T, U)> {
        // choose the floating one out of T and U, and apply next_larger to it -> minimum double result
};

#undef SAME_ATTR1_OR_ATTR2
#undef SAME_INTEGRALITY
#undef SAME_SIGNEDNESS
#undef COMPARABLE

// safe_cmp, defaults to the language-defined comparison, which is good-enough for most cases

#define generate_ops(generator) \
    generator(eq, ==) \
    generator(ne, !=) \
    generator(gt, >) \
    generator(lt, <) \
    generator(ge, >=) \
    generator(le, <=)

template<typename Left, typename Right, typename Enable = void>
struct safe_cmp {
#define generator(name, op) \
    static bool name(Left x, Right y) { \
        /* std::cout << "matched generic safe_cmp\n"; */ \
        return x op y; \
    }
    generate_ops(generator)
#undef generator
};

#define SLEFT_LE_URIGHT(Left, Right) (INTEGRAL(Left) && INTEGRAL(Right) && SIGNED(Left) && UNSIGNED(Right) && SIZE(Left, <=, Right))
#define DIFFERENT_SIGN_AND_SIGNED_LE_UNSIGNED(T, U) IF(SLEFT_LE_URIGHT(T, U) || SLEFT_LE_URIGHT(U, T))

template<typename Left, typename Right>
struct safe_cmp<Left, Right, DIFFERENT_SIGN_AND_SIGNED_LE_UNSIGNED(Left, Right)> {
    typedef typename next_larger2<Left, Right>::type target_type;
#define generator(name, op) static bool name(Left x, Right y) { \
        /* std::cout << "matched promoting safe_cmp\n"; */ \
        return static_cast<target_type>(x) op static_cast<target_type>(y); \
    }

    generate_ops(generator)
#undef generator
};

#undef SLEFT_LE_URIGHT
#undef DIFFERENT_SIGN_AND_SIGNED_LE_UNSIGNED

#undef IF
#undef IFELSE
#undef IS
#undef ISNT
#undef SIZE

#undef SIGNED
#undef UNSIGNED
#undef INTEGRAL
#undef FLOATING

/******************************************************************************
 * safe/safe_t utility function/struct, combining both safe_cast and safe_cmp
 *****************************************************************************/

template<typename T, template<typename, typename> class CastPolicy = policy_truncate, typename PolicyArg = void*>
struct safe_t {
    const T x;
    PolicyArg policy_arg;
    explicit safe_t(T x_, PolicyArg policy_arg_ = PolicyArg()) : x(x_), policy_arg(policy_arg_) {}

    // generic policy helper: with argument
    template<template<typename, typename> class NewCastPolicy, typename NewPolicyArg>
    safe_t<T, NewCastPolicy, NewPolicyArg> policy(NewPolicyArg new_policy_arg) {
        return safe_t<T, NewCastPolicy, NewPolicyArg>(x, new_policy_arg);
    }

    // generic policy helper: without argument
    template<template<typename, typename> class NewCastPolicy>
    safe_t<T, NewCastPolicy> policy() { return policy<NewCastPolicy, void*>(NULL); }

    // built-in policy helpers
    safe_t<T, policy_truncate>      ptruncate()          { return policy<policy_truncate>(); }
    safe_t<T, policy_result, int*>  presult(int *result) { return policy<policy_result, int*>(result); }
    template<typename Lambda>
    safe_t<T, policy_exec, Lambda>  pexec(Lambda lambda) { return policy<policy_exec, Lambda>(lambda); }
    safe_t<T, policy_assert>        passert()            { return policy<policy_assert>(); }
    safe_t<T, policy_throw>         pthrow()             { return policy<policy_throw>(); }
    template<typename Logger>
    safe_t<T, policy_log, Logger>   plog(Logger logger)  { return policy<policy_log, Logger>(logger); }

    template<typename Target>
    operator Target() {
        return safe_cast_impl<Target, T, CastPolicy, PolicyArg>::cast(x, policy_arg);
    }

#define generator(name, op) \
    template<typename U> \
    friend bool operator op(safe_t safe, U y) { \
        return safe_cmp<T, U>::name(safe.x, y); \
    } \
    template<typename U> \
    friend bool operator op(U y, safe_t safe) { \
        return safe_cmp<U, T>::name(y, safe.x); \
    }

    generate_ops(generator)
#undef generator
};

#undef generate_ops

template<typename T>
safe_t<T> safe(T x) { return safe_t<T>(x); }

// you are encouraged to define your own project-wide xxx_safe generator that uses a different default policy

} // namespace safe_ops
#endif // _SAFE_COMMON_H_
