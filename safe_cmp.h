#ifndef _SAFE_CMP_H_
#define _SAFE_CMP_H_

#include "safe_common.h"

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

// next_larger_signed_type helper, defaults to double

template<typename T, typename Enable = void>
struct next_larger_signed_type {
    typedef double type; // catch-all
};

template<typename T>
struct next_larger_signed_type<T, IF(INTEGRAL(T) && sizeof(T) == 1)> {
    typedef int16_t type;
};

template<typename T>
struct next_larger_signed_type<T, IF(INTEGRAL(T) && sizeof(T) == 2)> {
    typedef int32_t type;
};

template<typename T>
struct next_larger_signed_type<T, IF(INTEGRAL(T) && sizeof(T) == 4)> {
    typedef int64_t type;
};

#if defined(__STRICT_ANSI__) && defined(_GLIBCXX_USE_INT128)
template<typename T>
struct next_larger_signed_type<T, IF(INTEGRAL(T) && sizeof(T) == 8)> {
    typedef __int128_t type;
};
#endif

// greater_or_equal_size_type helper, returns one of the types, whichever is larger, any of them if equal in size

template<typename T, typename U, typename Enable = void>
struct greater_or_equal_size_type {
};

template<typename T, typename U>
struct greater_or_equal_size_type<T, U, IF(INTEGRAL(T) && INTEGRAL(U) && SIZE(T, <, U))> {
    typedef U type;
};

template<typename T, typename U>
struct greater_or_equal_size_type<T, U, IF(INTEGRAL(T) && INTEGRAL(U) && SIZE(T, >=, U))> {
    typedef T type;
};

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
#define generator(name, op) static bool name(Left x, Right y) { \
        /* std::cout << "matched promoting safe_cmp\n"; */ \
        typedef typename greater_or_equal_size_type<Left, Right>::type larger_type_of_the_two; \
        typedef typename next_larger_signed_type<larger_type_of_the_two>::type target_type; \
        return static_cast<target_type>(x) op static_cast<target_type>(y); \
    }

    generate_ops(generator)
#undef generator
};

#undef SLEFT_LE_URIGHT
#undef DIFFERENT_SIGN_AND_SIGNED_LE_UNSIGNED

template<typename T>
struct safe_t {
    const T x;
    explicit safe_t(T x_) : x(x_) {}
#define generator(name, op) \
    template<typename U> \
    friend bool operator op(safe_t<T> safe, U y) { \
        return safe_cmp<T, U>::name(safe.x, y); \
    } \
    template<typename U> \
    friend bool operator op(U y, safe_t<T> safe) { \
        return safe_cmp<U, T>::name(y, safe.x); \
    }

    generate_ops(generator)
#undef generator
};

#undef generate_ops

template<typename Left>
safe_t<Left> safe(Left x) { return safe_t<Left>(x); }

#undef IF
#undef IS
#undef ISNT
#undef SIZE

#undef SIGNED
#undef UNSIGNED
#undef INTEGRAL
#undef FLOATING

#endif // _SAFE_CMP_H
