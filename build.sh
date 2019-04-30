#!/bin/bash
clang -Wall -Wextra -std=c11 -g -Isrc src/main.c -lm -o bin/main 
./bin/main
