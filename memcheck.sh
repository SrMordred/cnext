#!/bin/bash
make
valgrind --leak-check=full --log-file="memcheck_results.txt" ./bin/main
