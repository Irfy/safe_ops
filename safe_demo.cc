#include <sys/types.h>
#include <iostream>

#include "safe_ops.h"

using safe_ops::safe;
using std::cout;
using std::endl;

#define MAX(T) std::numeric_limits<T>::max()
#define MIN(T) std::numeric_limits<T>::min()

int main() {
    cout << "max int + 1 unsafe: " << (MAX(int) + 1) << endl;
    cout << "max int + 1   safe: " << (MAX(int) + safe(1)).value() << endl; // resulting type is larger than int

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsign-compare"
    assert(1u < -1); // unfortunate language rules: signed is reinterpreted as unsigned
#pragma GCC diagnostic pop
    assert(safe(1u) > -1); // types are promoted to the next larger signed type
    assert(1u > safe(-1));

    size_t size = MAX(size_t);
    off_t offset = safe(size).ptruncate(); // choose truncation policy
    assert(offset == MAX(off_t)); // don't worry about exact types
    assert(offset < safe(size));

    offset = -1;
    size = safe(offset).ptruncate();
    assert(size == 0);

    try {
        size = safe(offset); // default: policy_throw
        assert(0);
    } catch(std::bad_cast) {
        cout << "bad_cast caught as expected\n";
    } catch(...) {
        assert(0);
    }

    size = MAX(size_t);
    cout << "last 3 digits of max size_t: " << (size % 1000) << endl;;
#if __cplusplus >= 201103L
    auto result1 = safe(size).passert() + 1; // will yield a larger type to hold the result
#endif
    // __int128 is not printable, so take last 3 digits and safely cast down to int64_t
    cout << "max(size_t)+1 without casting: " << size_t((safe(size).passert() + 1).value() % 1000) << endl;
    cout << "max(size_t)+1 as size_t/ptruncate: " << ((size_t(safe(size).ptruncate() + 1)) % 1000) << " (1 less due to truncation)" << endl;
    cout << "### FOLLOWING ASSERT/ABORT IS EXPECTED ###\n";
    size_t result = safe(size).passert() + 1; // will end the program with an abort
    cout << (result % 1000) << endl;
    return 0;
}
