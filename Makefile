CC=g++
CFLAGS=-std=c++17 -Wall -Werror -Wextra -pedantic

cache: cache.cpp cache_testy.cpp
	$(CC) $(CFLAGS) -o cache.cpp cache_testy.cpp