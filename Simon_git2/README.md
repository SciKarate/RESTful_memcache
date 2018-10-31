Simon Walker-Kahne and Sam Zofkie

October 30th, 2018

Math 389 HW 3 
===================================

We ran into some problems with our cache from HW 2, so we had to make a few changes. We ended up putting previous separate .cpp and .hh files into our cache.cpp for the convenience of our testers, and to avoid some linking errors from improperly linked files from HW 2. We also realized we had a strange error in the cache.hh file that we had been using-- our copy specified Cache::get took only the key_type key argument, not the index_type val_size argument. So we fixed that (albeit scrappily) and implemented Catch2 into our main.cpp and set about testing everyone elses code.

Sierra and Mercy's cache.cc file was the first one we tried, and reading the compiler errors from it is what prompted us to realize that our cache.hh was wrong. After redoing our main.cpp (where all of our tests are), their code compiled and passed all of our tests on the first try! It also is like half the size of our cache.cpp which is a little embarrassing for us.

| get_key | get_keys | get_different_type_keys | insert_replacement | delete_key | delete_keys | space_allocated | space_deallocated |
|---|---|---|---|---|---|---|---|
| **PASS** | **PASS** | **PASS** | **PASS** | **PASS** | **PASS** | **PASS** | **PASS** |

Mike and David's testinglab.cpp gave us a compilation error saying it did not recognize an unordered_map function called "insert_or_assign", which apparently only works with c++17 compilers. So we changed the -std flag and it compiled immeadiately, but failed the space_deallocated test, which verifies that the get_space() method updates the availible space when Cache::del deletes keys from the cache.

| get_key | get_keys | get_different_type_keys | insert_replacement | delete_key | delete_keys | space_allocated | space_deallocated |
|---|---|---|---|---|---|---|---|
| **PASS** | **PASS** | **PASS** | **PASS** | **PASS** | **PASS** | **PASS** | **FAIL** |

Betsy and Robert's cache.cpp needed to be linked with their evictor.cpp file so I added evictor.hh to the directory and added evictor.cpp to the makefile and it compiled, although with the make warning: "Clock skew detected. Your build may be incomplete." The executable failed the get_different_type_keys, which Cache::sets a float value and a string value into a cache and tests that they come out the same.

| get_key | get_keys | get_different_type_keys | insert_replacement | delete_key | delete_keys | space_allocated | space_deallocated |
|---|---|---|---|---|---|---|---|
| **PASS** | **PASS** | **FAIL** | **PASS** | **PASS** | **PASS** | **PASS** | **PASS** |
