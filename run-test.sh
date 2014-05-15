#!/bin/bash

rm -f *.log

valgrind --leak-check=full --track-origins=yes --log-file=valgrind.log ./test_graphdb > test.log