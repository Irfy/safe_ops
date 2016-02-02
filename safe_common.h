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

#include <stdint.h>

#if __cplusplus >= 201103L
#   include <type_traits>
#else
#   include "safe_cast_cxx98compat.h"
#endif

#include "safe_cast_ansicompat.h" // missing __int128_t specializations

#endif // _SAFE_COMMON_H_
