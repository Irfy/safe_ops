## Motivation

Have you ever had to handle converting values of library types like `size_t`
and `off_t` under strict warnings and for different platforms? If you wanted
to write warning-free code that will handle overflows/underflows gracefully,
you'll know it's a nightmare.

This C++ library header is here to help with that.

## Synopsis

This template header library provides syntactic sugar for graceful handling of
various arithmetic overflow situations when performing operations on variables
of different numeric types.

The library handles:

- Assignment/casting with overflow handling policies
- Comparison, completely avoiding overflows
- Arithmetic operations, completely avoiding overflows

There are several built-in policies for handling overflows.

There is no runtime performance penalty when the library would do the same
thing as the base language itself (e.g. comparing `int` and `long`, or
converting `int` to `long`).

## Code Example

    int64_t sec...
    sleep(safe(sec)); // truncated to the range of target type

    void foo(size_t size, off_t off) {
        if(safe(size) > off_t) { ... } // comparison is always correct
    }

    void try_append(... T *obj) {
        size_t new_size = safe(current_size).pthrow() + sizeof(T);
        // the addition will result in a type larger than size_t, and the
        // assignment will throw std::bad_cast if the result is too large for size_t
        ...
    }

See also `safe_demo.cc` for further examples.

## Installation

Simply `#include "safe_ops.h"`.

## API Reference

There are multiple ways to use the library in increasing order of complexity:

- by using the `safe` helper and chaining policy helpers
- by creating `safe_t<>` objects manually, and
- by invoking the `safe_{cast/cmp/arith}*` implementation helpers.

The first option is the recommended one and is the least verbose: template
parameters may be fully omitted. The result is a `safe_t` struct that
encapsulates the overflow handling and type enlargement for all operations.

The second option is using the `safe_t` struct constructor directly -- template
parameters must be explicitly given in this case. The declaration is:

    template<
        typename T,
        template<typename, typename> class CastPolicy = policy_truncate,
        typename PolicyArg = void*
    > struct safe_t;

The `safe_t` struct relies on various SFINAE-based structs to implement its
functionality, and these magic structs can be invoked directly -- this is the
third option.

The built-in policies will be mentioned at the end.

The following subsections assume

    Source s;
    Target t;

to be given.

### Casting

The simplest way to cast `s` to `t` is:

    t = safe(s);

or as a temporary:

    static_cast<Target>(safe(s));

The default cast policy is to truncate the value at the point of overflow or
underflow, meaning the result will be the max or min value of the target type,
respectively.

To change the policy to `policy_exec` (see Overflow handling policies later)

    t = safe(s).pexec([](int result){ ... });

To change to a user-supplied policy:

    t = safe(s).policy<MyPolicy>(); // policy takes no arguments
    t = safe(s).policy<MyPolicy, MyPolicyArg>(my_policy_arg); // policy takes an argument

Using the `safe_t` struct directly:

    auto lambda = [](int result){ ... };
    t = safe_t<Source, policy_exec, decltype(lambda)>(lambda); // ugly!

Using the magic helpers:

    t = safe_cast_trunc<Target>(s);
    t = safe_cast_exec<Target, policy_exec, decltype(lambda)>(s, lambda);

To use user-supplied policies with the helpers, the `safe_cast_impl` struct
must be used directly:

    t = safe_cast_impl<Target, Source, MyPolicy, MyPolicyArg>(s, my policy_arg);

Note how the methods of invocation have increasing number of template
arguments that *must* be given. The simplest method above utilizes templated
conversion operator to deduce the target type, so that no template/type
parameter needs to--but still can--be given.

Runtime performance-wise, due to tempalte deduction and basic compiler
optimizations, there is no difference between the invocation styles, and thus
the first style should be preferred.

### Comparison

There is no overflow or underflow possible when performing comparison: the
arguments of comparison are cast to a larger type if necessary to avoid
overflow and underflow.

The simplest way to compare `s` to `t` is:

    safe(s) < t; // <=, >, >=, == and != are also implemented

With the `safe_t` directly:

    safe_t<Source>(s) < t

Using the helper struct:

    safe_cmp<Source, Target>::lt(s, t); // le, gt, ge, eq and ne are implemented

Since overflows and underflows are avoided by casting the arguments to an
appropriate type when necessary, no policies make sense and no additional
helpers are provided.

### Arithmetic operations

Arithmetic operations can result in overflow and underflow in both obvious and
obscure manners. For example -5 / 3u yields 1431655763 and `INT_MIN / DBL_MIN`
needs a `long double` to represent. The simplest way to fix
this is:

    safe(s) / t; // +, -, *, / and %[1] are provided

Using the `safe_t` struct:

    safe_t<Source>(s) / t;

And the helper struct:

    safe_arith<Source, Target>::div(s, t); // add, sub, mul and mod[1] are provided

Since overflows and underflows are avoided by choosing an appropriate result
type, no policies make sense and no additional helpers are provided.

[1] The modulus operator is only implemented for integral types. If either
operand is a floating type, the compilation will fail.

This may be obsucred by the fact that `safe` arithmetic operations on a
variable with the largest integral type will yield a `safe` float or double to
contain the result. To use the modulus operator, the result may be cast down to
an integral type with an appropriate overflow policy to detect errors.

### Overflow handling policies

Overflow/underflow policies provide a means to react to an overflow/underflow.
Several policies are built-in, and it's easy to provide your own.

#### Built-in policies

- `policy_trunc`: the default policy, truncates its result to the maximum
  target value in case of overflow or the minimum (lowest) target value in
  case of underflow.
- `policy_result`: accepts an `int *result` argument and sets the
  int to 1 or -1 in case of overflow or underflow, respectively. The value
  of the pointed-to variable is unchanged when no overflow/underflow occurs.
- `policy_exec`: accepts a generic executable argument, could be a function
  pointer or a function object or a lambda expression -- as long as it accepts
  one argument of type int. This executable object will be called with an
  argument of 1 or -1 in case of overflow or underflow, respectively.
- `policy_assert`: calls `assert` with a suitable error message.
- `policy_throw`: throws `std::bad_cast` with a suitable error message.
- `policy_log`: accepts a pointer to a generic type and invokes
  `ptr->log("E", "<message>");` with a suitable message text for overflow and
  underflow conditions.

#### User-supplied policies

Every policy has two template arguments:

    Policy<TargetType, PolicyArg>

In case the policy doesn't accept arguments, the second template argument will
be passed `void*` -- the template parameter may not be omitted.

The policy has to implement two static member functions:

    ::handle_overflow(PolicyArg policy_arg);
    ::handle_underflow(PolicyArg policy_arg);

In case the policy doesn't accept arguments, the type PolicyArg may not be
omitted from the signature -- use it without a variable name to avoid unused
parameter warnings.

## Limitations

The library is only tested under GCC, so compiler quirks may apply.

The library uses GCC's `[unsigned] __int128` as the largest integral types,
when available. STL specializations are partly missing for these types, e.g.
`cout << my128int` won't compile, and `numeric_limits<__int128>` are empty
unless GNU extensions are enabled. If this poses a problem, it is best to
cast down to `[u]int64_t` if no overflow is expected, or to `double` if an
overflow is anticipated (`unsigned __int128` does not fit in `float`!).

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
