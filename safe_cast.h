#ifndef _SAFE_CAST_H_
#define _SAFE_CAST_H_

#include <typeinfo> // std::bad_cast for policy_throw
#include <limits>
#include <stdint.h> // <cstdint> is not c++03 conformant

#ifndef assert // use user-provided assert macro -- useful for testing
#include <cassert> // for policy_assert
#endif

#define IF(expr)            typename std::enable_if<(expr), void>::type
#define IS(T,U)             std::is_same<T,U>::value
#define ISNT(T,U)           !std::is_same<T,U>::value
#define SIGNED(T)           std::is_signed<T>::value
#define UNSIGNED(T)         std::is_unsigned<T>::value
#define INTEGRAL(T)         std::is_integral<T>::value
#define FLOATING(T)         std::is_floating_point<T>::value
#define SIZE(T,cmp,U)       (sizeof(T) cmp sizeof(U))

#define BOTH_SIGNED         (SIGNED(Target) && SIGNED(Source))
#define SAME_INTEGRALITY    (INTEGRAL(Target) == INTEGRAL(Source))
#define TARGET_LT           SIZE(Target, <, Source)
#define TARGET_LE           SIZE(Target, <=, Source)
#define TARGET_GT           SIZE(Target, >, Source)
#define TARGET_GE           SIZE(Target, >=, Source)
#define INT_GT_FLOAT(T,U)   (IS(U,float) && INTEGRAL(T) && sizeof(T) > 16 && SIGNED(T)) // __int128_t fits in float completely. so must be larger than that
#define UINT_GT_FLOAT(T,U)  (IS(U,float) && INTEGRAL(T) && sizeof(T) >= 16 && UNSIGNED(T)) // __uint128_t doesn't fit in float. so greater-or-equal
#define INTEGRAL_GT_FLOAT(T,U)  (INT_GT_FLOAT(T,U) || UINT_GT_FLOAT(T,U))

#define MAX(T) numeric_limits_compat<T>::max()
#define MIN(T) numeric_limits_compat<T>::lowest()

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

template<typename Target, typename Source, typename Policy, typename PolicyArg, typename Enable = void>
struct safe_cast_impl {
#if __cplusplus >= 201103L
    static_assert(std::is_fundamental<Target>::value, "Target type of overflow checking is not supported");
    static_assert(std::is_fundamental<Source>::value, "Source type for overflow checking is not supported");
#endif // for c++98, the call to cast() will simply fail to bind, instead of getting a nice error message
};

template<typename Target, typename Source, typename Policy, typename PolicyArg>
struct safe_cast_impl<Target, Source, Policy, PolicyArg, CASTABLE_ALWAYS(Source,Target)> {
    static Target cast(Source x, PolicyArg *policy_arg = NULL) {
        return static_cast<Target>(x);
    }
};

template<typename Target, typename Source, typename Policy, typename PolicyArg>
struct safe_cast_impl<Target, Source, Policy, PolicyArg, CASTABLE_WHEN_POSITIVE(Source,Target)> {
    static Target cast(Source x, PolicyArg *policy_arg = NULL) {
        if (x < 0)
            return Policy::handle_underflow(policy_arg);
        else
            return static_cast<Target>(x);
    }
};

template<typename Target, typename Source, typename Policy, typename PolicyArg>
struct safe_cast_impl<Target, Source, Policy, PolicyArg, CASTABLE_FROM_UNSIGNED_WHEN_SMALL_ENOUGH(Source,Target)> {
    static Target cast(Source x, PolicyArg *policy_arg = NULL) {
        return static_cast<Source>(MAX(Target)) < x ? Policy::handle_overflow(policy_arg) : static_cast<Target>(x);
    }
};

template<typename Target, typename Source, typename Policy, typename PolicyArg>
struct safe_cast_impl<Target, Source, Policy, PolicyArg, CASTABLE_WHEN_POSITIVE_AND_SMALL_ENOUGH(Source,Target)> {
    static Target cast(Source x, PolicyArg *policy_arg = NULL) {
        if (x < 0)
            return Policy::handle_underflow(policy_arg);
        else
            return static_cast<Source>(MAX(Target)) < x ? Policy::handle_overflow(policy_arg) : static_cast<Target>(x);
    }
};

template<typename Target, typename Source, typename Policy, typename PolicyArg>
struct safe_cast_impl<Target, Source, Policy, PolicyArg, CASTABLE_WHEN_SMALL_ENOUGH(Source,Target)> {
    static Target cast(Source x, PolicyArg *policy_arg = NULL) {
        if (x < 0)
            return static_cast<Source>(MIN(Target)) > x ? Policy::handle_underflow(policy_arg) : static_cast<Target>(x);
        else
            return static_cast<Source>(MAX(Target)) < x ? Policy::handle_overflow(policy_arg) : static_cast<Target>(x);
    }
};

#undef CASTABLE_ALWAYS
#undef CASTABLE_WHEN_POSITIVE
#undef CASTABLE_FROM_UNSIGNED_WHEN_SMALL_ENOUGH
#undef CASTABLE_WHEN_POSITIVE_AND_SMALL_ENOUGH
#undef CASTABLE_WHEN_SMALL_ENOUGH

#include "safe_common_undef.h"

#undef BOTH_SIGNED
#undef SAME_INTEGRALITY
#undef TARGET_LT
#undef TARGET_LE
#undef TARGET_GT
#undef TARGET_GE
#undef INT_GT_FLOAT
#undef UINT_GT_FLOAT
#undef INTEGRAL_GT_FLOAT

template<typename Target, typename PolicyArg = void>
struct policy_truncate {
    static Target handle_overflow(PolicyArg*) { return MAX(Target); }
    static Target handle_underflow(PolicyArg*) { return MIN(Target); }
};

template<typename Target, typename PolicyArg = void>
struct policy_assert {
    static Target handle_overflow(PolicyArg*) { assert("Overflow detected" == NULL); return MAX(Target); }
    static Target handle_underflow(PolicyArg*) { assert("Underflow detected" == NULL); return MIN(Target); }
};

template<typename Target, typename PolicyArg = void>
struct policy_throw {
    static Target handle_overflow(PolicyArg*) { throw std::bad_cast(); }
    static Target handle_underflow(PolicyArg*) { throw std::bad_cast(); }
};

template<typename Target, typename Logger>
struct policy_log {
    static Target handle_overflow(PolicyArg *logger) { logger->log ("E", "Overflow detected"); return MAX(Target); }
    static Target handle_underflow(PolicyArg *logger) { logger->log ("E", "Underflow detected"); return MIN(Target); }
};

#undef MAX
#undef MIN

// main entry point for casting

template<typename Target, typename Source>
Target safe_cast_trunc(Source x) {
    return safe_cast_impl<Target,Source,policy_truncate<Target>,void>::cast(x);
}

template<typename Target, typename Source>
Target safe_cast_assert(Source x) {
    return safe_cast_impl<Target,Source,policy_assert<Target>,void>::cast(x);
}

template<typename Target, typename Source>
Target safe_cast_throw(Source x) {
    return safe_cast_impl<Target,Source,policy_throw<Target>,void>::cast(x);
}

template<typename Target, typename Source, typename Logger>
Target safe_cast_log(Source x, Logger *logger) {
    return safe_cast_impl<Target,Source,policy_log<Target, Logger>, Logger>::cast(x, logger);
}

// experiments

template<typename Source>
struct safe_cast_trunc_struct {
    safe_cast_trunc_struct(Source x_) : x(x_) {}
    template<typename Target>
    operator Target() {
        return safe_cast_impl<Target,Source,policy_truncate<Target>,void>::cast(x);
    }
    Source x;
};

template<typename Source>
safe_cast_trunc_struct<Source> safe_cast_trunc2(Source x) {
    return safe_cast_trunc_struct<Source>(x);
}

#endif // _SAFE_CAST_H_
