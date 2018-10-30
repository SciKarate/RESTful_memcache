#pragma once



/*
Need to keep track of the sizes of values in the cache.
There is no way to get the size of a void* without casting it
which is why we need a struct that stores the size.
*/

struct Cache_Value {
    void *value = 0;
    uint32_t value_size;

    Cache_Value();
    uint32_t get_size();
    ~Cache_Value();
};
