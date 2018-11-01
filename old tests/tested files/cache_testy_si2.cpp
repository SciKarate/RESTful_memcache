#define CATCH_CONFIG_MAIN

#include <iostream>
#include <string>
#include "cache.hh"
#include "catch.hpp"

using namespace std;



template <typename T>
bool
correct_value_for_key(string key, Cache &c, T expected) {

    T *v;
    Cache::index_type size;
    v = (T*) c.get(key, size);
    //cout << "Returned value: " << *v << "| Expected value: " << expected << endl;
    return (*v == expected);
}

TEST_CASE( "Insert single key") {
        Cache c(100);
        int32_t val = 10;
        string key = "ten";

        int32_t *val_ptr;
        val_ptr = &val;
        c.set(key, val_ptr, sizeof(val));

        bool passed = correct_value_for_key(key, c, 10);

        REQUIRE( passed );
    }

TEST_CASE( "Insert multiple keys") {
    Cache c(100);
    int val_1 = 20;
    string key_1 = "twenty";

    int val_2 = 30;
    string key_2 = "thirty";

    int val_3 = 40;
    string key_3 = "fourty";

    int *val_ptr;
    val_ptr = &val_1;
    c.set(key_1, val_ptr, sizeof(int));

    val_ptr = &val_2;
    c.set(key_2, val_ptr, sizeof(int));

    val_ptr = &val_3;
    c.set(key_3, val_ptr, sizeof(int));

    bool passed = correct_value_for_key(key_1, c, 20);
    passed = (correct_value_for_key(key_2, c, 30) && passed);
    passed = (correct_value_for_key(key_3, c, 40) && passed);

    REQUIRE( passed );
}

TEST_CASE( "Insert multiple keys of different types" ) {
    Cache c(100);

    string val_str = "string value";
    string key_1 = "string key";

    float val_flo = 3.5;
    string key_2 = "float key";

    string *str_ptr;
    str_ptr = &val_str;
    c.set(key_1, str_ptr, val_str.size());

    float *flo_ptr;
    flo_ptr = &val_flo;
    c.set(key_2, flo_ptr, sizeof(val_flo));

    bool passed = correct_value_for_key(key_1, c, val_str);
    passed = correct_value_for_key(key_2, c, val_flo) && passed;

    REQUIRE( passed );
}

TEST_CASE( "Insert existing key" ) {
    Cache c(100);

    int32_t val = 10;
    string key = "ten";

    int32_t *val_ptr;
    val_ptr = &val;
    c.set(key, val_ptr, sizeof(val));
    assert(correct_value_for_key(key, c, 10));

    int new_val = 11;
    int *new_val_ptr = &new_val;
    string existing_key = "ten";

    c.set(existing_key, new_val_ptr, sizeof(new_val));

    bool passed = correct_value_for_key(existing_key, c, new_val);

    REQUIRE ( passed );
}

TEST_CASE( "Delete single key" ) {
    Cache c(100);

    int32_t val = 10;
    string key = "ten";

    int32_t *val_ptr;
    val_ptr = &val;
    c.set(key, val_ptr, sizeof(val));
    assert(correct_value_for_key(key, c, 10));

    string deleted_key = "ten";
    Cache::index_type size;
    c.del(deleted_key);

    bool passed = (!(c.get(deleted_key, size)));

    REQUIRE ( passed );
}

TEST_CASE( "Delete multiple keys" ) {
    Cache c(100);

    string deleted_key_1 = "twenty";
    string deleted_key_2 = "string key";
    string deleted_key_3 = "float key";
    c.del(deleted_key_1);
    c.del(deleted_key_2);
    c.del(deleted_key_3);
    Cache::index_type size1;
    Cache::index_type size2;
    Cache::index_type size3;

    bool passed = (!(c.get(deleted_key_1, size1)));
    passed = (!(c.get(deleted_key_2, size2))) && passed;
    passed = (!(c.get(deleted_key_3, size3))) && passed;

    REQUIRE ( passed );
}

TEST_CASE( "FIFO evictor test" ) {
    Cache c(10);

    string key_1 = "ten";
    int32_t val_1 = 10;

    string key_2 = "twenty";
    int32_t val_2 = 20;

    string key_3 = "thirty";
    int32_t val_3 = 30;

    Cache::index_type size1;
    Cache::index_type size2;

    c.set(key_1, &val_1, sizeof(int32_t));
    c.set(key_2, &val_2, sizeof(int32_t));
    c.set(key_3, &val_3, sizeof(int32_t));

    /*The default evictor is FIFO
    so only the first key should be evicted*/
    bool passed = c.get(key_2, size2);
    passed = (!(c.get(key_1, size1))) && passed;

    REQUIRE ( passed );
}

TEST_CASE( "space_used() test" ) {
    Cache c(100);

    string key = "array";
    int32_t xs[10] = {1,2,3,4,5,6,7,8,9,10};

    c.set(key, &xs, sizeof(xs));

    bool passed = ( c.space_used() == sizeof(xs) );

    REQUIRE( passed );
}

TEST_CASE( "space_used() updates size afer deleting key" ) {
    Cache c(100);

    string key = "array";
    int32_t xs[10] = {1,2,3,4,5,6,7,8,9,10};

    c.set(key, &xs, sizeof(xs));
    int32_t full = c.space_used();

    c.del(key);
    int32_t empty = c.space_used();

    bool passed = ( c.space_used() == 0 );
    passed = ( full != empty ) && passed;

    REQUIRE( passed );
}