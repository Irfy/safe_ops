#!/bin/bash -e

for stdflag in '' -ansi -std=c++11 -std=gnu++11; do
    CXXFLAGS=$stdflag make -B
    echo
    echo "Testing standards flag '$stdflag'"
    ./safe_cast
done
