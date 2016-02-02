#!/bin/bash -e

for stdflag in '' -ansi -std=c++11 -std=gnu++11; do
    echo
    echo "Testing standards flag '$stdflag'"
    CXXFLAGS=$stdflag make -B
    echo
    ./safe_test
done
