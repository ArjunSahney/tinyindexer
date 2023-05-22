#!/bin/bash

# Too many arguments:
./querier test1 test2 test3


# Too few arguments:
./querier test1


# Incorrect Page Directory
./querier ../crawler ../../shared/tse/indices/toscrape-1.ndx

# Incorrect Index File 
./querier ../../shared/tse/output/toscrape-1 test1.txt

# Fuzz Testing - Regression Testing of Valid Output: 
./fuzzquery ../../shared/tse/indices/toscrape-1.ndx 10 15 | ./querier ../../shared/tse/output/toscrape-1 ../../shared/tse/indices/toscrape-1.ndx

# Direct STDIN Testing:
echo friedman | ./querier ../../shared/tse/output/toscrape-1 ../../shared/tse/indices/toscrape-1.ndx

# Empty STDIN Testing:
echo | ./querier ../../shared/tse/output/toscrape-1 ../../shared/tse/indices/toscrape-1.ndx
