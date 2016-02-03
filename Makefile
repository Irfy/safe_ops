CXXFLAGS += -Wall -Wextra -pedantic -Wno-long-long

all: safe_test

check:
	@for stdflag in '' -ansi -std=c++11 -std=gnu++11 -std=c++14 -std=gnu++14 -std=c++1z -std=gnu++1z; do \
	    CXXFLAGS=$$stdflag $(MAKE) -sB; \
	    ./safe_test > output 2>/dev/null; \
	    diff output expected || { echo "stdflag[$$stdflag]: unexpected difference"; exit 1; }; \
        echo stdflag[$$stdflag]: output == expected; \
	    rm output; \
	done

clean:
	rm -f safe_test
