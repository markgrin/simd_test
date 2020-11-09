#!/bin/sh
g++ -lpthread -g -std=c++17 -msse -msse2 -msse3 -mavx -mavx2 -mavx512f main.cpp && g++ -lpthread -g -o main.S -mavx -mavx2 -mavx512f -msse -msse2 -msse3  -S main.cpp

