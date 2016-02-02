#ifndef _SAFE_CAST_CXX98COMPAT_H_
#define _SAFE_CAST_CXX98COMPAT_H_

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
    gen_signed(signed long long);
    gen_signed(float);
    gen_signed(double);
    gen_signed(long double);
#if !defined(__STRICT_ANSI__) && defined(_GLIBCXX_USE_INT128)
    gen_signed(__int128_t);
#endif

#undef gen_signed

template<typename T>
struct is_unsigned {
    static const bool value = !is_signed<T>::value;
};

}

#endif // _SAFE_CAST_CXX98COMPAT_H_
