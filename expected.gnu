    If a 'safe' test were to fail, the assertion text would contain 'safe(' and would be easy to identify. E.g.:
failed assertion `safe(1) > 2'
    safe_arith test int/int: no asserts expected
    safe_arith test unsigned/unsigned: 5 asserts expected, unsigned lowest()
failed assertion `numeric_limits_compat<unsigned>::lowest() + (numeric_limits_compat<unsigned>::lowest()) != ((unsigned long)(numeric_limits_compat<unsigned>::lowest()) + numeric_limits_compat<unsigned>::lowest()): [0] isn't != [0]
failed assertion `numeric_limits_compat<unsigned>::lowest() + (numeric_limits_compat<unsigned>::lowest()) != ((unsigned long)(numeric_limits_compat<unsigned>::lowest()) + numeric_limits_compat<unsigned>::lowest()): [0] isn't != [0]
failed assertion `numeric_limits_compat<unsigned>::max() - (numeric_limits_compat<unsigned>::lowest()) != ((long) (numeric_limits_compat<unsigned>::max()) - numeric_limits_compat<unsigned>::lowest()): [4294967295] isn't != [4294967295]
failed assertion `numeric_limits_compat<unsigned>::lowest() * (numeric_limits_compat<unsigned>::lowest()) != ((unsigned long)(numeric_limits_compat<unsigned>::lowest()) * numeric_limits_compat<unsigned>::lowest()): [0] isn't != [0]
failed assertion `numeric_limits_compat<unsigned>::lowest() * (numeric_limits_compat<unsigned>::lowest()) != ((unsigned long)(numeric_limits_compat<unsigned>::lowest()) * numeric_limits_compat<unsigned>::lowest()): [0] isn't != [0]
    safe_arith test int/unsigned: 4 asserts expected, unsigned lowest() and negative division
failed assertion `numeric_limits_compat<int>::max() - (numeric_limits_compat<unsigned>::lowest()) != ((long) (numeric_limits_compat<int>::max()) - numeric_limits_compat<unsigned>::lowest()): [2147483647] isn't != [2147483647]
failed assertion `numeric_limits_compat<int>::lowest() * (numeric_limits_compat<unsigned>::lowest()) != ((long)(numeric_limits_compat<int>::lowest()) * numeric_limits_compat<unsigned>::lowest()): [0] isn't != [0]
failed assertion `numeric_limits_compat<unsigned>::lowest() * (numeric_limits_compat<int>::lowest()) != ((long)(numeric_limits_compat<int>::lowest()) * numeric_limits_compat<unsigned>::lowest()): [0] isn't != [0]
failed assertion `numeric_limits_compat<int>::lowest() / ((unsigned)1) == ((long) (numeric_limits_compat<int>::lowest()) / (unsigned)1): [2147483648] isn't == [-2147483648]
    safe_arith test unsigned/int: 3 asserts expected, unsigned lowest() and negative division
failed assertion `numeric_limits_compat<unsigned>::lowest() * (numeric_limits_compat<int>::lowest()) != ((long)(numeric_limits_compat<unsigned>::lowest()) * numeric_limits_compat<int>::lowest()): [0] isn't != [0]
failed assertion `numeric_limits_compat<int>::lowest() * (numeric_limits_compat<unsigned>::lowest()) != ((long)(numeric_limits_compat<unsigned>::lowest()) * numeric_limits_compat<int>::lowest()): [0] isn't != [0]
failed assertion `numeric_limits_compat<unsigned>::max() / ((int)-1) == ((long) (numeric_limits_compat<unsigned>::max()) / (int)-1): [1] isn't == [-4294967295]
    safe_arith test int/long: no asserts expected
    safe_arith test long/int: no asserts expected
    safe_arith test float/uint64_t: 8 asserts expected, int too small to make a difference and 0-multiplication
failed assertion `numeric_limits_compat<float>::max() + (numeric_limits_compat<uint64_t>::max()) != ((double)(numeric_limits_compat<float>::max()) + numeric_limits_compat<uint64_t>::max()): [3.40282e+38] isn't != [3.40282e+38]
failed assertion `numeric_limits_compat<uint64_t>::max() + (numeric_limits_compat<float>::max()) != ((double)(numeric_limits_compat<float>::max()) + numeric_limits_compat<uint64_t>::max()): [3.40282e+38] isn't != [3.40282e+38]
failed assertion `numeric_limits_compat<float>::lowest() + (numeric_limits_compat<uint64_t>::lowest()) != ((double)(numeric_limits_compat<float>::lowest()) + numeric_limits_compat<uint64_t>::lowest()): [-3.40282e+38] isn't != [-3.40282e+38]
failed assertion `numeric_limits_compat<uint64_t>::lowest() + (numeric_limits_compat<float>::lowest()) != ((double)(numeric_limits_compat<float>::lowest()) + numeric_limits_compat<uint64_t>::lowest()): [-3.40282e+38] isn't != [-3.40282e+38]
failed assertion `numeric_limits_compat<float>::lowest() - (numeric_limits_compat<uint64_t>::max()) != ((double) (numeric_limits_compat<float>::lowest()) - numeric_limits_compat<uint64_t>::max()): [-3.40282e+38] isn't != [-3.40282e+38]
failed assertion `numeric_limits_compat<float>::max() - (numeric_limits_compat<uint64_t>::lowest()) != ((double) (numeric_limits_compat<float>::max()) - numeric_limits_compat<uint64_t>::lowest()): [3.40282e+38] isn't != [3.40282e+38]
failed assertion `numeric_limits_compat<float>::lowest() * (numeric_limits_compat<uint64_t>::lowest()) != ((double)(numeric_limits_compat<float>::lowest()) * numeric_limits_compat<uint64_t>::lowest()): [-0] isn't != [-0]
failed assertion `numeric_limits_compat<uint64_t>::lowest() * (numeric_limits_compat<float>::lowest()) != ((double)(numeric_limits_compat<float>::lowest()) * numeric_limits_compat<uint64_t>::lowest()): [-0] isn't != [-0]
    safe_arith test uint64_t/float: 9 asserts expected, int too small to make a difference, 0-multiplication and non-zero division by small number
failed assertion `numeric_limits_compat<uint64_t>::max() + (numeric_limits_compat<float>::max()) != ((double)(numeric_limits_compat<uint64_t>::max()) + numeric_limits_compat<float>::max()): [3.40282e+38] isn't != [3.40282e+38]
failed assertion `numeric_limits_compat<float>::max() + (numeric_limits_compat<uint64_t>::max()) != ((double)(numeric_limits_compat<uint64_t>::max()) + numeric_limits_compat<float>::max()): [3.40282e+38] isn't != [3.40282e+38]
failed assertion `numeric_limits_compat<uint64_t>::lowest() + (numeric_limits_compat<float>::lowest()) != ((double)(numeric_limits_compat<uint64_t>::lowest()) + numeric_limits_compat<float>::lowest()): [-3.40282e+38] isn't != [-3.40282e+38]
failed assertion `numeric_limits_compat<float>::lowest() + (numeric_limits_compat<uint64_t>::lowest()) != ((double)(numeric_limits_compat<uint64_t>::lowest()) + numeric_limits_compat<float>::lowest()): [-3.40282e+38] isn't != [-3.40282e+38]
failed assertion `numeric_limits_compat<uint64_t>::lowest() - (numeric_limits_compat<float>::max()) != ((double) (numeric_limits_compat<uint64_t>::lowest()) - numeric_limits_compat<float>::max()): [-3.40282e+38] isn't != [-3.40282e+38]
failed assertion `numeric_limits_compat<uint64_t>::max() - (numeric_limits_compat<float>::lowest()) != ((double) (numeric_limits_compat<uint64_t>::max()) - numeric_limits_compat<float>::lowest()): [3.40282e+38] isn't != [3.40282e+38]
failed assertion `numeric_limits_compat<uint64_t>::lowest() * (numeric_limits_compat<float>::lowest()) != ((double)(numeric_limits_compat<uint64_t>::lowest()) * numeric_limits_compat<float>::lowest()): [-0] isn't != [-0]
failed assertion `numeric_limits_compat<float>::lowest() * (numeric_limits_compat<uint64_t>::lowest()) != ((double)(numeric_limits_compat<uint64_t>::lowest()) * numeric_limits_compat<float>::lowest()): [-0] isn't != [-0]
failed assertion `numeric_limits_compat<uint64_t>::max() / ((float)-1e-30) == ((double) (numeric_limits_compat<uint64_t>::max()) / (float)-1e-30): [-inf] isn't == [-1.84467e+49]
    safe_arith test float/float: 2 asserts expected, non-zero division by small number
failed assertion `numeric_limits_compat<float>::lowest() / ((float)1e-30) == ((double) (numeric_limits_compat<float>::lowest()) / (float)1e-30): [-inf] isn't == [-3.40282e+68]
failed assertion `numeric_limits_compat<float>::max() / ((float)-1e-30) == ((double) (numeric_limits_compat<float>::max()) / (float)-1e-30): [-inf] isn't == [-3.40282e+68]
    safe_arith test double/double: 2 asserts expected, non-zero division by small number
failed assertion `numeric_limits_compat<double>::lowest() / ((double)1e-30) == ((long double) (numeric_limits_compat<double>::lowest()) / (double)1e-30): [-inf] isn't == [-1.79769e+338]
failed assertion `numeric_limits_compat<double>::max() / ((double)-1e-30) == ((long double) (numeric_limits_compat<double>::max()) / (double)-1e-30): [-inf] isn't == [-1.79769e+338]
    safe_arith test float/int128_t: no asserts expected
    safe_arith test int128_t/float: 2 asserts expected, non-zero division by small number
failed assertion `numeric_limits_compat<int128_t>::lowest() / ((float)1e-30) == ((double) (numeric_limits_compat<int128_t>::lowest()) / (float)1e-30): [-inf] isn't == [-1.70141e+68]
failed assertion `numeric_limits_compat<int128_t>::max() / ((float)-1e-30) == ((double) (numeric_limits_compat<int128_t>::max()) / (float)-1e-30): [-inf] isn't == [-1.70141e+68]
    safe_arith test float/uint128_t: 6 asserts expected, unsigned lowest ops and unsigned -1 division
failed assertion `numeric_limits_compat<float>::lowest() + (numeric_limits_compat<uint128_t>::lowest()) != ((double)(numeric_limits_compat<float>::lowest()) + numeric_limits_compat<uint128_t>::lowest()): [-3.40282e+38] isn't != [-3.40282e+38]
failed assertion `numeric_limits_compat<uint128_t>::lowest() + (numeric_limits_compat<float>::lowest()) != ((double)(numeric_limits_compat<float>::lowest()) + numeric_limits_compat<uint128_t>::lowest()): [-3.40282e+38] isn't != [-3.40282e+38]
failed assertion `numeric_limits_compat<float>::max() - (numeric_limits_compat<uint128_t>::lowest()) != ((double) (numeric_limits_compat<float>::max()) - numeric_limits_compat<uint128_t>::lowest()): [3.40282e+38] isn't != [3.40282e+38]
failed assertion `numeric_limits_compat<float>::lowest() * (numeric_limits_compat<uint128_t>::lowest()) != ((double)(numeric_limits_compat<float>::lowest()) * numeric_limits_compat<uint128_t>::lowest()): [-0] isn't != [-0]
failed assertion `numeric_limits_compat<uint128_t>::lowest() * (numeric_limits_compat<float>::lowest()) != ((double)(numeric_limits_compat<float>::lowest()) * numeric_limits_compat<uint128_t>::lowest()): [-0] isn't != [-0]
failed assertion `numeric_limits_compat<float>::max() / ((uint128_t)-1) == ((double) (numeric_limits_compat<float>::max()) / (uint128_t)-1): [0] isn't == [1]
    safe_arith test uint128_t/float: 7 asserts expected, unsigned lowest ops and divisions
failed assertion `numeric_limits_compat<uint128_t>::lowest() + (numeric_limits_compat<float>::lowest()) != ((double)(numeric_limits_compat<uint128_t>::lowest()) + numeric_limits_compat<float>::lowest()): [-3.40282e+38] isn't != [-3.40282e+38]
failed assertion `numeric_limits_compat<float>::lowest() + (numeric_limits_compat<uint128_t>::lowest()) != ((double)(numeric_limits_compat<uint128_t>::lowest()) + numeric_limits_compat<float>::lowest()): [-3.40282e+38] isn't != [-3.40282e+38]
failed assertion `numeric_limits_compat<uint128_t>::lowest() - (numeric_limits_compat<float>::max()) != ((double) (numeric_limits_compat<uint128_t>::lowest()) - numeric_limits_compat<float>::max()): [-3.40282e+38] isn't != [-3.40282e+38]
failed assertion `numeric_limits_compat<uint128_t>::lowest() * (numeric_limits_compat<float>::lowest()) != ((double)(numeric_limits_compat<uint128_t>::lowest()) * numeric_limits_compat<float>::lowest()): [-0] isn't != [-0]
failed assertion `numeric_limits_compat<float>::lowest() * (numeric_limits_compat<uint128_t>::lowest()) != ((double)(numeric_limits_compat<uint128_t>::lowest()) * numeric_limits_compat<float>::lowest()): [-0] isn't != [-0]
failed assertion `numeric_limits_compat<uint128_t>::max() / ((float)-1) == ((double) (numeric_limits_compat<uint128_t>::max()) / (float)-1): [-inf] isn't == [-3.40282e+38]
failed assertion `numeric_limits_compat<uint128_t>::max() / ((float)-1e-30) == ((double) (numeric_limits_compat<uint128_t>::max()) / (float)-1e-30): [-inf] isn't == [-3.40282e+68]
ad-hoc test 'policy vs operator+': caught expected bad_cast: MAX(int) + 1 is no longer an int
    expecting 1u >/>= int8_t(-1) to fail, but safe variants to succeed...
failed assertion `1u > int8_t(-1)'
failed assertion `int8_t(-1) < 1u'
failed assertion `1u >= int8_t(-1)'
failed assertion `int8_t(-1) <= 1u'
    expecting 0u >/>= -1 to fail, but safe variants to succeed...
failed assertion `0u > -1'
failed assertion `-1 < 0u'
failed assertion `0u >= -1'
failed assertion `-1 <= 0u'
    expecting 1ul >/>= -1 to fail, but safe variants to succeed...
failed assertion `1ul > -1'
failed assertion `-1 < 1ul'
failed assertion `1ul >= -1'
failed assertion `-1 <= 1ul'
    expecting 4294967296ul >/>= -1 to fail, but safe variants to succeed...
failed assertion `4294967296ul > -1'
failed assertion `-1 < 4294967296ul'
failed assertion `4294967296ul >= -1'
failed assertion `-1 <= 4294967296ul'
    expecting 1ul >/>= -1l to fail, but safe variants to succeed...
failed assertion `1ul > -1l'
failed assertion `-1l < 1ul'
failed assertion `1ul >= -1l'
failed assertion `-1l <= 1ul'
    expecting 2147483648u >/!= -2147483648 to fail, but safe variants to succeed...
failed assertion `2147483648u > (-2147483647-1)'
failed assertion `(-2147483647-1) < 2147483648u'
failed assertion `2147483648u != (-2147483647-1)'
failed assertion `(-2147483647-1) != 2147483648u'
    expecting 4294967295u >/!= -1 to fail, but safe variants to succeed...
failed assertion `4294967295u > -1'
failed assertion `-1 < 4294967295u'
failed assertion `4294967295u != -1'
failed assertion `-1 != 4294967295u'
    no failures expected in any of the floating point comparisons (neither native nor 'safe')...
safe_cmp tests passed
    safe_cast_assert: expecting two asserts...
failed assertion `"Overflow detected" == NULL'
failed assertion `"Underflow detected" == NULL'
    safe_cast_result: expecting no asserts...
    safe_cast_lambda: expecting two 'lambda: ...' messages...
lambda: overflow detected
lambda: underflow detected
    safe_cast_log: expecting two log entries...
[log]	./safe_test	E	Overflow detected
[log]	./safe_test	E	Underflow detected
    safe_cast_throw: expecting two bad_casts...
bad_cast caught due to overflow
bad_cast caught due to underflow
non-truncating safe_cast functional tests passed
truncating full-coverage tests following:
failed assertion `(float)numeric_limits_compat<long double>::max() == (float)numeric_limits_compat<float>::max(): [inf] isn't == [3.40282e+38]
failed assertion `(float)numeric_limits_compat<long double>::lowest() == (float)numeric_limits_compat<float>::lowest(): [-inf] isn't == [-3.40282e+38]
failed assertion `(int)numeric_limits_compat<double>::max() == (int)numeric_limits_compat<int>::max(): [-2147483648] isn't == [2147483647]
failed assertion `(unsigned)numeric_limits_compat<double>::max() == (unsigned)numeric_limits_compat<unsigned>::max(): [0] isn't == [4294967295]
floating point tests passed
failed assertion `(uint64_t)numeric_limits_compat<float>::max() == (uint64_t)numeric_limits_compat<uint64_t>::max(): [0] isn't == [18446744073709551615]
failed assertion `(uint64_t)numeric_limits_compat<float>::lowest() == (uint64_t)numeric_limits_compat<uint64_t>::lowest(): [9223372036854775808] isn't == [0]
failed assertion `(int64_t)numeric_limits_compat<float>::max() == (int64_t)numeric_limits_compat<int64_t>::max(): [-9223372036854775808] isn't == [9223372036854775807]
failed assertion `(uint32_t)numeric_limits_compat<float>::max() == (uint32_t)numeric_limits_compat<uint32_t>::max(): [0] isn't == [4294967295]
failed assertion `(int32_t)numeric_limits_compat<float>::max() == (int32_t)numeric_limits_compat<int32_t>::max(): [-2147483648] isn't == [2147483647]
naive sizeof tests passed (float <-> int64/32_t)
failed assertion `(float)numeric_limits_compat<uint128_t>::max() == (float)numeric_limits_compat<float>::max(): [inf] isn't == [3.40282e+38]
failed assertion `(uint128_t)numeric_limits_compat<float>::lowest() == (uint128_t)numeric_limits_compat<uint128_t>::lowest(): [0x80000000000000008000000000000000] isn't == [0x00000000000000000000000000000000]
failed assertion `(int128_t)numeric_limits_compat<float>::max() == (int128_t)numeric_limits_compat<int128_t>::max(): [0xffffff00000000000000000000000000] isn't == [0x7fffffffffffffffffffffffffffffff]
failed assertion `(int128_t)numeric_limits_compat<float>::lowest() == (int128_t)numeric_limits_compat<int128_t>::lowest(): [0x00000100000000000000000000000000] isn't == [0x80000000000000000000000000000000]
extreme sizeof tests passed (float <-> safe_[u]int128_t)
failed assertion `(int)numeric_limits_compat<unsigned>::max() == (int)numeric_limits_compat<int>::max(): [-1] isn't == [2147483647]
failed assertion `(unsigned)numeric_limits_compat<int>::lowest() == (unsigned)numeric_limits_compat<unsigned>::lowest(): [2147483648] isn't == [0]
same size integral tests passed
failed assertion `(int32_t)numeric_limits_compat<int64_t>::max() == (int32_t)numeric_limits_compat<int32_t>::max(): [-1] isn't == [2147483647]
failed assertion `(int32_t)numeric_limits_compat<int64_t>::lowest() == (int32_t)numeric_limits_compat<int32_t>::lowest(): [0] isn't == [-2147483648]
failed assertion `(int32_t)numeric_limits_compat<uint64_t>::max() == (int32_t)numeric_limits_compat<int32_t>::max(): [-1] isn't == [2147483647]
failed assertion `(uint64_t)numeric_limits_compat<int32_t>::lowest() == (uint64_t)numeric_limits_compat<uint64_t>::lowest(): [18446744071562067968] isn't == [0]
different size integral tests passed
