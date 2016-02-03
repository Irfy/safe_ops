// these will be undeffed manually
#define IF(expr)            typename std::enable_if<(expr), void>::type
#define IS(T,U)             std::is_same<T,U>::value
#define ISNT(T,U)           !std::is_same<T,U>::value
#define SIZE(T,cmp,U)       (sizeof(T) cmp sizeof(U))

#define SIGNED(T)           std::is_signed<T>::value
#define UNSIGNED(T)         std::is_unsigned<T>::value
#define INTEGRAL(T)         std::is_integral<T>::value
#define FLOATING(T)         std::is_floating_point<T>::value

#ifndef _SAFE_COMMON_H_
#define _SAFE_COMMON_H_

#if !defined(__STRICT_ANSI__) && defined(_GLIBCXX_USE_INT128)
#define SAFE_USE_INT128
__extension__ typedef __int128 safe_int128_t;
__extension__ typedef unsigned __int128 safe_uint128_t;
#endif

#include <stdint.h>

#if __cplusplus >= 201103L
#   include <type_traits>
#else
// add needed type traits that are missing in c++98:
// enable_if, is_same, is_floating_point, is_integral, is_signed and is_unsigned

namespace std {
template<bool value, typename T>
struct enable_if {};

    template<typename T>
    struct enable_if<true, T> {
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
    static const bool value = !is_floating_point<T>::value;
};

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
    gen_signed(signed short);
    gen_signed(signed int);
    gen_signed(signed long);
    gen_signed(float);
    gen_signed(double);
    gen_signed(long double);
#ifdef SAFE_USE_INT128
    gen_signed(safe_int128_t);
#endif

#undef gen_signed

template<typename T>
struct is_unsigned {
    static const bool value = !is_signed<T>::value;
};

}
#endif

#endif // _SAFE_COMMON_H_
