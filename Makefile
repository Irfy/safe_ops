# Copyright (c) 2016 Irfan Adilovic
# This software is licensed under the terms of the MIT license.
# See LICENSE for the text of the license.

CXXFLAGS += -Wall -Wextra -pedantic

OBJ = safe_test

all: $(OBJ)

safe_test: CPPFLAGS += -DDEFAULT_SAFE_CAST_POLICY=policy_truncate

check:
	@test $$(grep -E 'safe\(' expected.{c,gnu} | grep -Ev "'safe\('|failed assertion .safe\(1\) > 2." | wc -l) -gt 0 && { echo "expected.* files contain safe failures"; exit 1; } || echo "expected.* files are clean"
	@for stdyear in 03 11 14 1z; do \
	    for stdext in c gnu; do \
            stdflag=-std=$$stdext++$$stdyear; \
	        CXXFLAGS=$$stdflag $(MAKE) -sB; \
	        ./safe_test > output 2>/dev/null; \
	        diff output expected.$$stdext || { echo "stdflag[$$stdflag]: unexpected difference"; exit 1; }; \
            echo stdflag[$$stdflag]: output == expected.$$stdext; \
	        rm output; \
	    done; \
	done

clean:
	rm -f safe_test
