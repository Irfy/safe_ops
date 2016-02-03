CXXFLAGS += -Wall -Wextra -pedantic

all: safe_test

check:
	@for stdyear in 03 11 14 1z; do \
	    for stdext in c gnu; do \
            stdflag=-std=$$stdext++$$stdyear; \
	        CXXFLAGS=$$stdflag $(MAKE) -sB; \
	        ./safe_test > output 2>/dev/null; \
	        diff output expected.$$stdext || { echo "stdflag[$$stdflag]: unexpected difference"; exit 1; }; \
            echo stdflag[$$stdflag]: output == expected; \
	        rm output; \
	    done; \
	done

clean:
	rm -f safe_test
