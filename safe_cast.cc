#include <iostream>
#include <cassert>
#include <limits>
#include <stdint.h>
#include <cstdio>


#undef assert // for testability
#define assert(e) ((e) ? (void)0 : ((void)printf ("%s:%u: failed assertion `%s'\n", __FILE__, __LINE__, #e)))
#include "safe_cast.h"

const char *progname = "";

struct FakeLogger {
    void log(const char *level, const char *str) { printf("[log]\t%s\t%s\t%s\n", progname, level, str); }
};

int main(int argc, char**argv) {
    progname = argv[0];
    FakeLogger logger;

    int x = safe_cast_trunc2(2.5);
    assert(x == 2);

printf("expecting -1 < 0u to fail, but Safe(-1) < 0u and -1 < Safe(0u) to succeed...\n");
    assert(-1 < 0u);
    assert(safe(-1) < 0u);
    //assert(-1 < safe(0u));

printf("ad-hoc tests passed\n");

printf("expecting two asserts...\n");
    safe_cast_assert<int>(numeric_limits_compat<long>::max());
    safe_cast_assert<int>(numeric_limits_compat<long>::min());

printf("expecting two log entries...\n");
    safe_cast_log<int>(numeric_limits_compat<long>::max(), &logger);
    safe_cast_log<int>(numeric_limits_compat<long>::min(), &logger);

    try {
        safe_cast_throw<int>(numeric_limits_compat<long>::max());
        assert(0);
    } catch (std::bad_cast &) {
        printf ("expected bad_cast due to overflow\n");
    }

    try {
        safe_cast_throw<int>(numeric_limits_compat<long>::min());
        assert(0);
    } catch (std::bad_cast &) {
        printf ("expected bad_cast due to underflow\n");
    }

printf("non-truncating tests passed\n");
    // long double -> float
    assert(safe_cast_trunc<float>(static_cast<long double>(0)) == 0.0);
    assert(safe_cast_trunc<float>(numeric_limits_compat<long double>::max()) == numeric_limits_compat<float>::max());
    assert(safe_cast_trunc<float>(-numeric_limits_compat<long double>::max()) == -numeric_limits_compat<float>::max());

    // float -> long double
    assert(safe_cast_trunc<long double>(static_cast<float>(0)) == 0.0);
    assert(safe_cast_trunc<long double>(numeric_limits_compat<float>::max()) == numeric_limits_compat<float>::max());
    assert(safe_cast_trunc<long double>(-numeric_limits_compat<float>::max()) == -numeric_limits_compat<float>::max());

    // double -> int
    assert(safe_cast_trunc<int>(0.0) == 0);
    assert(safe_cast_trunc<int>(numeric_limits_compat<double>::max()) == numeric_limits_compat<int>::max());
    assert(safe_cast_trunc<int>(-numeric_limits_compat<double>::max()) == numeric_limits_compat<int>::min());

    // int -> double
    assert(safe_cast_trunc<double>(0) == 0.0);
    assert(safe_cast_trunc<double>(numeric_limits_compat<int>::max()) == numeric_limits_compat<int>::max());
    assert(safe_cast_trunc<double>(numeric_limits_compat<int>::min()) == numeric_limits_compat<int>::min());

    // double -> unsigned
    assert(safe_cast_trunc<unsigned>(0.0) == 0);
    assert(safe_cast_trunc<unsigned>(numeric_limits_compat<double>::max()) == numeric_limits_compat<unsigned>::max());
    assert(safe_cast_trunc<unsigned>(-numeric_limits_compat<double>::max()) == numeric_limits_compat<unsigned>::min());

    // unsigned -> double
    assert(safe_cast_trunc<double>(0) == 0.0);
    assert(safe_cast_trunc<double>(numeric_limits_compat<unsigned>::max()) == numeric_limits_compat<unsigned>::max());
    assert(safe_cast_trunc<double>(numeric_limits_compat<unsigned>::min()) == numeric_limits_compat<unsigned>::min());

printf("floating point tests passed\n");

/// naive size comparison is "wrong"
    // float -> uint64_t
    assert(safe_cast_trunc<uint64_t>(0.0f) == 0);
    assert(safe_cast_trunc<uint64_t>(numeric_limits_compat<float>::max()) == numeric_limits_compat<uint64_t>::max());
    assert(safe_cast_trunc<uint64_t>(-numeric_limits_compat<float>::max()) == numeric_limits_compat<uint64_t>::min());

    // uint64_t -> float
    assert(safe_cast_trunc<float>(static_cast<uint64_t>(0)) == 0.0f);
    assert(safe_cast_trunc<float>(numeric_limits_compat<uint64_t>::max()) == numeric_limits_compat<uint64_t>::max());
    assert(safe_cast_trunc<float>(numeric_limits_compat<uint64_t>::min()) == numeric_limits_compat<uint64_t>::min());

    // float -> int64_t
    assert(safe_cast_trunc<int64_t>(0.0f) == 0);
    assert(safe_cast_trunc<int64_t>(numeric_limits_compat<float>::max()) == numeric_limits_compat<int64_t>::max());
    assert(safe_cast_trunc<int64_t>(-numeric_limits_compat<float>::max()) == numeric_limits_compat<int64_t>::min());

    // int64_t -> float
    assert(safe_cast_trunc<float>(static_cast<int64_t>(0)) == 0.0f);
    assert(safe_cast_trunc<float>(numeric_limits_compat<int64_t>::max()) == numeric_limits_compat<int64_t>::max());
    assert(safe_cast_trunc<float>(numeric_limits_compat<int64_t>::min()) == numeric_limits_compat<int64_t>::min());

/// naive size comparison is 'equal'
    // float -> uint32_t
    assert(safe_cast_trunc<uint32_t>(0.0f) == 0);
    assert(safe_cast_trunc<uint32_t>(numeric_limits_compat<float>::max()) == numeric_limits_compat<uint32_t>::max());
    assert(safe_cast_trunc<uint32_t>(-numeric_limits_compat<float>::max()) == numeric_limits_compat<uint32_t>::min());

    // uint32_t -> float
    assert(safe_cast_trunc<float>(static_cast<uint32_t>(0)) == 0.0f);
    assert(safe_cast_trunc<float>(numeric_limits_compat<uint32_t>::max()) == numeric_limits_compat<uint32_t>::max());
    assert(safe_cast_trunc<float>(numeric_limits_compat<uint32_t>::min()) == numeric_limits_compat<uint32_t>::min());

    // float -> int32_t
    assert(safe_cast_trunc<int32_t>(0.0f) == 0);
    assert(safe_cast_trunc<int32_t>(numeric_limits_compat<float>::max()) == numeric_limits_compat<int32_t>::max());
    assert(safe_cast_trunc<int32_t>(-numeric_limits_compat<float>::max()) == numeric_limits_compat<int32_t>::min());

    // int32_t -> float
    assert(safe_cast_trunc<float>(static_cast<int32_t>(0)) == 0.0f);
    assert(safe_cast_trunc<float>(numeric_limits_compat<int32_t>::max()) == numeric_limits_compat<int32_t>::max());
    assert(safe_cast_trunc<float>(numeric_limits_compat<int32_t>::min()) == numeric_limits_compat<int32_t>::min());

printf("naive sizeof tests passed (float <-> int64/32_t)\n");

/// ints greater than float
#if defined(_GLIBCXX_USE_INT128)
    // float -> __uint128_t
    assert(safe_cast_trunc<__uint128_t>(0.0f) == 0);
    assert(safe_cast_trunc<__uint128_t>(numeric_limits_compat<float>::max()) == numeric_limits_compat<float>::max());
    assert(safe_cast_trunc<__uint128_t>(-numeric_limits_compat<float>::max()) == numeric_limits_compat<__uint128_t>::min());

    // __uint128_t -> float
    assert(safe_cast_trunc<float>(static_cast<__uint128_t>(0)) == 0.0f);
    assert(safe_cast_trunc<float>(numeric_limits_compat<__uint128_t>::max()) == numeric_limits_compat<float>::max());
    assert(safe_cast_trunc<float>(numeric_limits_compat<__uint128_t>::min()) == numeric_limits_compat<__uint128_t>::min());

/// __int128_t is less than float, but test special-case handling regardless
    // float -> __int128_t
    assert(safe_cast_trunc<__int128_t>(0.0f) == 0);
    assert(safe_cast_trunc<__int128_t>(numeric_limits_compat<float>::max()) == numeric_limits_compat<__int128_t>::max());
    assert(safe_cast_trunc<__int128_t>(-numeric_limits_compat<float>::max()) == numeric_limits_compat<__int128_t>::min());

    // __int128_t -> float
    assert(safe_cast_trunc<float>(static_cast<__int128_t>(0)) == 0.0f);
    assert(safe_cast_trunc<float>(numeric_limits_compat<__int128_t>::max()) == numeric_limits_compat<__int128_t>::max());
    assert(safe_cast_trunc<float>(numeric_limits_compat<__int128_t>::min()) == numeric_limits_compat<__int128_t>::min());

printf("extreme sizeof tests passed (float <-> int128_t)\n");

#endif

/// integers
    // same size, both signed
    assert(safe_cast_trunc<int>(0) == 0);
    assert(safe_cast_trunc<int>(numeric_limits_compat<int>::max()) == numeric_limits_compat<int>::max());
    assert(safe_cast_trunc<int>(numeric_limits_compat<int>::min()) == numeric_limits_compat<int>::min());

    // same size, both unsigned
    assert(safe_cast_trunc<unsigned>(0) == 0);
    assert(safe_cast_trunc<unsigned>(numeric_limits_compat<unsigned>::max()) == numeric_limits_compat<unsigned>::max());

    // same size, unsigned to signed
    assert(safe_cast_trunc<int>(static_cast<unsigned>(0)) == 0);
    assert(safe_cast_trunc<int>(numeric_limits_compat<unsigned>::max()) == numeric_limits_compat<int>::max()); // overflow fixed!

    // same size, signed to unsigned
    assert(safe_cast_trunc<unsigned>(0) == 0);
    assert(safe_cast_trunc<unsigned>(numeric_limits_compat<int>::max()) == static_cast<unsigned>(numeric_limits_compat<int>::max()));
    assert(safe_cast_trunc<unsigned>(numeric_limits_compat<int>::min()) == static_cast<unsigned>(0)); // underflow fixed!

printf("same size integral tests passed\n");

    // Target > Source, both signed
    assert(safe_cast_trunc<int64_t>(static_cast<int32_t>(0)) == 0);
    assert(safe_cast_trunc<int64_t>(numeric_limits_compat<int32_t>::max()) == static_cast<int64_t>(numeric_limits_compat<int32_t>::max()));
    assert(safe_cast_trunc<int64_t>(numeric_limits_compat<int32_t>::min()) == static_cast<int64_t>(numeric_limits_compat<int32_t>::min()));

    // Target > Source, both unsigned
    assert(safe_cast_trunc<uint64_t>(static_cast<uint32_t>(0)) == 0);
    assert(safe_cast_trunc<uint64_t>(numeric_limits_compat<uint32_t>::max()) == static_cast<uint64_t>(numeric_limits_compat<uint32_t>::max()));

    // Target > Source, unsigned to signed
    assert(safe_cast_trunc<int64_t>(static_cast<uint32_t>(0)) == 0);
    assert(safe_cast_trunc<int64_t>(numeric_limits_compat<uint32_t>::max()) == static_cast<int64_t>(numeric_limits_compat<uint32_t>::max()));

    // Target > Source, signed to unsigned
    assert(safe_cast_trunc<uint64_t>(static_cast<int32_t>(0)) == 0);
    assert(safe_cast_trunc<uint64_t>(numeric_limits_compat<int32_t>::max()) == static_cast<uint64_t>(numeric_limits_compat<int32_t>::max()));
    assert(safe_cast_trunc<uint64_t>(numeric_limits_compat<int32_t>::min()) == 0); // underflow fixed!

printf("Target > Source integral tests passed\n");

    // Target < Source, both signed
    assert(safe_cast_trunc<int32_t>(static_cast<int64_t>(0)) == 0);
    assert(safe_cast_trunc<int32_t>(numeric_limits_compat<int64_t>::max()) == numeric_limits_compat<int32_t>::max()); // overflow fixed!
    assert(safe_cast_trunc<int32_t>(numeric_limits_compat<int64_t>::min()) == numeric_limits_compat<int32_t>::min()); // underflow fixed!

    // Target < Source, both unsigned
    assert(safe_cast_trunc<int32_t>(static_cast<int64_t>(0)) == 0);
    assert(safe_cast_trunc<int32_t>(numeric_limits_compat<int64_t>::max()) == numeric_limits_compat<int32_t>::max()); // overflow fixed!

    // Target < Source, unsigned to signed
    assert(safe_cast_trunc<int32_t>(static_cast<uint64_t>(0)) == 0);
    assert(safe_cast_trunc<int32_t>(numeric_limits_compat<uint64_t>::max()) == numeric_limits_compat<int32_t>::max()); // overflow fixed!

    // Target < Source, signed to unsigned
    assert(safe_cast_trunc<uint32_t>(static_cast<int64_t>(0)) == 0);
    assert(safe_cast_trunc<uint32_t>(numeric_limits_compat<int64_t>::max()) == numeric_limits_compat<uint32_t>::max()); // overflow fixed!
    assert(safe_cast_trunc<uint32_t>(numeric_limits_compat<int64_t>::min()) == 0); // underflow fixed!

printf("Target < Source integral tests passed\n");

    return 0;
}
