#!/bin/bash
./build.sh
valgrind --leak-check=full --log-file="memcheck_results.txt" ./bin/main
