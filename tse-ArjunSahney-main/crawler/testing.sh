#!/bin/bash

# Below I have included tests that check boundary cases as well as different seeds and depths. I will include more comprehensive regression and integration testing on the final assignment and have already completed them but for the sake of brevity as stated in the specifications I've only included 5 tests here.


# Too few arguments:
./crawler https://www.chelseafc.com/en

# Too many arguments:
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html test1 33 man

# Invalid link
./crawler http://yuh.com test2 3

# Accurate test:
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html h1 10

# Accurate test with different seed and depth:
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/C.html h2 4


