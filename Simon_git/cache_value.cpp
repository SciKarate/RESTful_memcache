#include <iostream>
#include "cache_value.hh"


Cache_Value::Cache_Value() {
}

uint32_t
Cache_Value::get_size() {

    return this-> value_size;
}

/*
we can use free() because free() knows how to
deallocate void*. If we used delete, we would first have
to cast this-> value to a char array.
*/
Cache_Value::~Cache_Value() {

    free(this-> value);
    this-> value = NULL;
}

