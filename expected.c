    in the following tests, if a 'safe' test were to fail, it would look like this:
safe_test.cc:61: failed assertion `safe(1) > 2'
    expecting 1u >/>= int8_t(-1) to fail, but safe variants to succeed...
safe_test.cc:64: failed assertion `1u > int8_t(-1)'
safe_test.cc:64: failed assertion `int8_t(-1) < 1u'
safe_test.cc:64: failed assertion `1u >= int8_t(-1)'
safe_test.cc:64: failed assertion `int8_t(-1) <= 1u'
    expecting 0u >/>= -1 to fail, but safe variants to succeed...
safe_test.cc:67: failed assertion `0u > -1'
safe_test.cc:67: failed assertion `-1 < 0u'
safe_test.cc:67: failed assertion `0u >= -1'
safe_test.cc:67: failed assertion `-1 <= 0u'
    expecting 1ul >/>= -1 to fail, but safe variants to succeed...
safe_test.cc:70: failed assertion `1ul > -1'
safe_test.cc:70: failed assertion `-1 < 1ul'
safe_test.cc:70: failed assertion `1ul >= -1'
safe_test.cc:70: failed assertion `-1 <= 1ul'
    expecting 4294967296ul >/>= -1 to fail, but safe variants to succeed...
safe_test.cc:73: failed assertion `4294967296ul > -1'
safe_test.cc:73: failed assertion `-1 < 4294967296ul'
safe_test.cc:73: failed assertion `4294967296ul >= -1'
safe_test.cc:73: failed assertion `-1 <= 4294967296ul'
    expecting 1ul >/>= -1l to fail, but safe variants to succeed...
safe_test.cc:76: failed assertion `1ul > -1l'
safe_test.cc:76: failed assertion `-1l < 1ul'
safe_test.cc:76: failed assertion `1ul >= -1l'
safe_test.cc:76: failed assertion `-1l <= 1ul'
    expecting 2147483648u >/!= -2147483648 to fail, but safe variants to succeed...
safe_test.cc:79: failed assertion `2147483648u > (-2147483647-1)'
safe_test.cc:79: failed assertion `(-2147483647-1) < 2147483648u'
safe_test.cc:79: failed assertion `2147483648u != (-2147483647-1)'
safe_test.cc:79: failed assertion `(-2147483647-1) != 2147483648u'
    expecting 4294967295u >/!= -1 to fail, but safe variants to succeed...
safe_test.cc:83: failed assertion `4294967295u > -1'
safe_test.cc:83: failed assertion `-1 < 4294967295u'
safe_test.cc:83: failed assertion `4294967295u != -1'
safe_test.cc:83: failed assertion `-1 != 4294967295u'
    no failures expected in any of the floating point comparisons (neither native nor 'safe')...
ad-hoc tests passed
    expecting two asserts...
safe_cast.h:157: failed assertion `"Overflow detected" == NULL'
safe_cast.h:158: failed assertion `"Underflow detected" == NULL'
    expecting two log entries...
[log]	./safe_test	E	Overflow detected
[log]	./safe_test	E	Underflow detected
expected bad_cast due to overflow
expected bad_cast due to underflow
non-truncating tests passed
floating point tests passed
naive sizeof tests passed (float <-> int64/32_t)
same size integral tests passed
Target > Source integral tests passed
Target < Source integral tests passed
