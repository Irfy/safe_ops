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
#   include "safe_cast_cxx98compat.h"
#endif

#endif // _SAFE_COMMON_H_
