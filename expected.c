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
failed assertion `(int)numeric_limits_compat<unsigned>::max() == (int)numeric_limits_compat<int>::max(): [-1] isn't == [2147483647]
failed assertion `(unsigned)numeric_limits_compat<int>::lowest() == (unsigned)numeric_limits_compat<unsigned>::lowest(): [2147483648] isn't == [0]
same size integral tests passed
failed assertion `(int32_t)numeric_limits_compat<int64_t>::max() == (int32_t)numeric_limits_compat<int32_t>::max(): [-1] isn't == [2147483647]
failed assertion `(int32_t)numeric_limits_compat<int64_t>::lowest() == (int32_t)numeric_limits_compat<int32_t>::lowest(): [0] isn't == [-2147483648]
failed assertion `(int32_t)numeric_limits_compat<uint64_t>::max() == (int32_t)numeric_limits_compat<int32_t>::max(): [-1] isn't == [2147483647]
failed assertion `(uint64_t)numeric_limits_compat<int32_t>::lowest() == (uint64_t)numeric_limits_compat<uint64_t>::lowest(): [18446744071562067968] isn't == [0]
different size integral tests passed
