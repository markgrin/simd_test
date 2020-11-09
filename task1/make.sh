#!/bin/sh
g++ -O3 -lpthread -g  -msse -msse2 -msse3 -mavx -mavx2 -mavx512f main.cpp && g++ -O3 -lpthread -g -o main.S -mavx -mavx2 -mavx512f -msse -msse2 -msse3  -S main.cpp

