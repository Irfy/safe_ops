## Synopsis

Allow controlled overflow handling when converting integral arguments and
mathematically correct comparisons between all fundamental types.

There is no runtime performance penalty when the library would do the same
thing as the base language itself (e.g. comparing `int` and `long`, or
converting `int` to `long`).

## Code Example

    size_t my_size = std::numeric_limits<size_t>::max();
    
    off_t my_offset1 = static_cast<off_t>(my_size);
    // can you guarantee that this cannot overflow, on any platform?
    // can you guarantee that this shall overflow, on every platform?

    off_t my_offset2 = safe(my_size);
    // if no overflow, no runtime penalty for safe() invocation
    // if overflow, my_offset1 == std::numeric_limits<off_t>::max()

    off_t my_offset3 = safe(my_size).policy<policy_exec>([](int result){ ... });
    // the lambda is called on overflow/underflow with 1/-1 as argument

    // available policies: policy_truncate, policy_result, policy_exec,
    // policy_assert, policy_throw and policy_log

    assert(-1 < 1u); // fails
    assert(safe(-1) < 1u); // won't fail
    assert(safe(0) < 1u); // no runtime overhead

## Motivation

Have you ever had to handle converting values of library types like `size_t`
and `off_t` under strict warnings and for different platforms? If you wanted
to write warning-free code that will handle overflows/underflows gracefully,
you'll know it's a nightmare.

This C++ library header is here to help with that.

## Installation

Simply `#include` the provided header in your project.

## API Reference

For warming up, see the test suite `safe_test.cc`.

## Tests

`make check` will do the trick: it will test the code under all C++ standards,
combined with strict and gnu variants (as far as your compiler supports them).

## Contributors

You can report issues and contribute via github:
[github.com/Irfy/safe_ops](https://github.com/Irfy/safe_ops)

## License

Copyright (c) 2016 Irfan Adilovic

This software is licensed under the terms of the MIT license.

See `LICENSE` for the text of the license.
