    in the following tests, if a 'safe' test were to fail, it would look like this:
failed assertion `safe(1) > 2'
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
ad-hoc tests passed
    expecting two asserts...
failed assertion `"Overflow detected" == NULL'
failed assertion `"Underflow detected" == NULL'
    expecting two log entries...
[log]	./safe_test	E	Overflow detected
[log]	./safe_test	E	Underflow detected
expected bad_cast due to overflow
expected bad_cast due to underflow
non-truncating tests passed
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
failed assertion `(float)numeric_limits_compat<safe_uint128_t>::max() == (float)numeric_limits_compat<float>::max(): [inf] isn't == [3.40282e+38]
failed assertion `(safe_uint128_t)numeric_limits_compat<float>::lowest() == (safe_uint128_t)numeric_limits_compat<safe_uint128_t>::lowest(): [0x80000000000000008000000000000000] isn't == [0x00000000000000000000000000000000]
failed assertion `(safe_int128_t)numeric_limits_compat<float>::max() == (safe_int128_t)numeric_limits_compat<safe_int128_t>::max(): [0xffffff00000000000000000000000000] isn't == [0x7fffffffffffffffffffffffffffffff]
failed assertion `(safe_int128_t)numeric_limits_compat<float>::lowest() == (safe_int128_t)numeric_limits_compat<safe_int128_t>::lowest(): [0x00000100000000000000000000000000] isn't == [0x80000000000000000000000000000000]
extreme sizeof tests passed (float <-> safe_[u]int128_t)
failed assertion `(int)numeric_limits_compat<unsigned>::max() == (int)numeric_limits_compat<int>::max(): [-1] isn't == [2147483647]
failed assertion `(unsigned)numeric_limits_compat<int>::lowest() == (unsigned)numeric_limits_compat<unsigned>::lowest(): [2147483648] isn't == [0]
same size integral tests passed
failed assertion `(int32_t)numeric_limits_compat<int64_t>::max() == (int32_t)numeric_limits_compat<int32_t>::max(): [-1] isn't == [2147483647]
failed assertion `(int32_t)numeric_limits_compat<int64_t>::lowest() == (int32_t)numeric_limits_compat<int32_t>::lowest(): [0] isn't == [-2147483648]
failed assertion `(int32_t)numeric_limits_compat<uint64_t>::max() == (int32_t)numeric_limits_compat<int32_t>::max(): [-1] isn't == [2147483647]
failed assertion `(uint64_t)numeric_limits_compat<int32_t>::lowest() == (uint64_t)numeric_limits_compat<uint64_t>::lowest(): [18446744071562067968] isn't == [0]
different size integral tests passed
