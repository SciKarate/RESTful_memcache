Simon Walker-Kahne and Sam Zofkie.

Hey! So we initally did our #include headers wrong and we ran into tons of problems trying to link stuff, but now everything Eitan's cache.hh API specifies is implemented in our cache.cpp file, so that should be everyting y'all need to test our code.

Originally the fifo.cpp, evictor.cpp, and cache_value.cpp were separate files that we had incorrectly linked, but now they are all just sections of a now monstrously large cache.cpp file.

See the Makefile for compilation, although you'll want to exclude "unit_tests.cpp" and include your own main and or testing files at your own discretion. We did all of our testing and compiling in Linux.

This is the README for hw3, the testing version.