#!/bin/bash
clang -Wall -Wextra -g -Isrc src/main.c -lm -o bin/main 
./bin/main
