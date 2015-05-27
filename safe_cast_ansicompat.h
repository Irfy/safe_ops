#ifndef _SAFE_CAST_ANSICOMPAT_H_
#define _SAFE_CAST_ANSICOMPAT_H_

#if defined(__STRICT_ANSI__) && defined(_GLIBCXX_USE_INT128)
// 128bit integers available, but missing specialization
#define max64u __uint128_t(0xffffffffffffffff)
#define max128u ((max64u<<64)|max64u)
#define max128i __int128_t((max64u<<63)|max64u)
#define min128i (-max128i-1)
namespace std {

#if __cplusplus >= 201103L
#define gen_fundamental(type) \
template<> \
struct is_fundamental<type> { \
    static const bool value = true; \
};
#else
#define gen_fundamental(type)
#endif

#define gen_templates(type, maxval, signed) \
template<> \
struct numeric_limits<type> { \
    static type max() { return maxval; } \
    static type min() { return -maxval-1; } \
    static type lowest() { return -maxval-1; } \
}; \
gen_fundamental(type) \
template<> \
struct is_integral<type> { \
    static const bool value = true; \
}; \
template<> \
struct is_floating_point<type> { \
    static const bool value = false; \
}; \
template<> \
struct is_signed<type> { \
    static const bool value = signed; \
}; \
template<> \
struct is_unsigned<type> { \
    static const bool value = !signed; \
}

gen_templates(__uint128_t, max128u, false);
gen_templates(__int128_t, max128i, true);

#undef max64u
#undef max128u
#undef max128i
#undef min128i
}
#endif

#endif // _SAFE_CAST_ANSICOMPAT_H_
