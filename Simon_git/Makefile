CC=g++
CFLAGS=-std=c++11 -Wall -Werror -Wextra -pedantic




cache: main.cpp cache.cpp evictor.cpp fifo.cpp cache_value.cpp
	$(CC) $(CFLAGS) -o cache cache.cpp unit_tests.cpp main.cpp