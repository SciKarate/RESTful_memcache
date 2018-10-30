#include <iostream>
#include <string>
#include <assert.h>
#include "cache.hh"
#include "unit_tests.hh"

using namespace std;

template <typename T>
bool
correct_value_for_key(string key, Cache &c, T expected) {

    T *v;
    v = (T*) c.get(key);
    //cout << "Returned value: " << *v << "| Expected value: " << expected << endl;
    return (*v == expected);
}

void
single_insertion_test() {

    Cache c(100);
    int32_t val = 10;
    string key = "ten";

    int32_t *val_ptr;
    val_ptr = &val;
    c.set(key, val_ptr, sizeof(val));

    bool passed = correct_value_for_key(key, c, 10);

    if (passed) {

        cout << "Insertion test " << "\u001b[32m" << " passed "<< "\u001b[37m" << " for single value." << endl;
    } else {

        cout << "Insertion test " << "\u001b[31m" << " failed "<< "\u001b[37m" << " for single value." << endl;
    }
}

void
multiple_insertion_test() {

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

    if (passed) {

        cout << "Insertion test " << "\u001b[32m" << " passed "<< "\u001b[37m" << " for multiple value." << endl;
    } else {

        cout << "Insertion test " << "\u001b[31m" << " failed "<< "\u001b[37m" << " for multiple value." << endl;
    }
}

void
multiple_type_insertion_test() {

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
    if (passed) {

        cout << "Insertion test " << "\u001b[32m" << " passed "<< "\u001b[37m" << " for multiple types." << endl;
    } else {

        cout << "Insertion test " << "\u001b[31m" << " failed "<< "\u001b[37m" << " for multiple types." << endl;
    }
}


void
insert_existing_key_test() {

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

        if (passed) {

        cout << "Existing key insertion test " << "\u001b[32m" << " passed "<< "\u001b[37m" << endl;
    } else {

        cout << "Existing key insertion test " << "\u001b[31m" << " failed "<< "\u001b[37m" << endl;
    }
}


void
delete_single_key_test() {

    Cache c(100);

    int32_t val = 10;
    string key = "ten";

    int32_t *val_ptr;
    val_ptr = &val;
    c.set(key, val_ptr, sizeof(val));
    assert(correct_value_for_key(key, c, 10));

    string deleted_key = "ten";
    c.del(deleted_key);

    bool passed = (!(c.get(deleted_key)));

    if (passed) {

        cout << "Deletion test for single deletion " << "\u001b[32m" << " passed "<< "\u001b[37m" << endl;
    } else {

        cout << "Deletion test for single deletion " << "\u001b[32m" << " failed "<< "\u001b[37m" << endl;
    }
}


void
delete_multiple_key_test() {

    Cache c(100);

    string deleted_key_1 = "twenty";
    string deleted_key_2 = "string key";
    string deleted_key_3 = "float key";

    c.del(deleted_key_1);
    c.del(deleted_key_2);
    c.del(deleted_key_3);

    bool passed = (!(c.get(deleted_key_1)));
    passed = (!(c.get(deleted_key_2))) && passed;
    passed = (!(c.get(deleted_key_3))) && passed;

    if (passed) {

        cout << "Deletion test for multiple deletions " << "\u001b[32m" << " passed "<< "\u001b[37m" << endl;
    } else {

        cout << "Deletion test for multiple deletions " << "\u001b[32m" << " failed "<< "\u001b[37m" << endl;
    }
}

void
evictor_test() {

    Cache c(10);

    string key_1 = "ten";
    int32_t val_1 = 10;

    string key_2 = "twenty";
    int32_t val_2 = 20;

    string key_3 = "thirty";
    int32_t val_3 = 30;

    c.set(key_1, &val_1, sizeof(int32_t));

    c.set(key_2, &val_2, sizeof(int32_t));

    c.set(key_3, &val_3, sizeof(int32_t));

    /*The default evictor is FIFO
    so only the first key should be evicted*/
    bool passed = c.get(key_2);
    passed = (!(c.get(key_1))) && passed;
    if (passed) {

        cout << "Evictor test " << "\u001b[32m" << " passed "<< "\u001b[37m" << endl;
    } else {

        cout << "Evictor test " << "\u001b[31m" << " failed "<< "\u001b[37m" << endl;
    }


}

int
a_terrible_hash(string key) {

    /*
    I know this does not make sense.
    I am trying to avoid an unused variable error.
    */
    if (!key.empty()) {

        return 1;
    } else {

        return 1;
    }
}

void
test_custom_hash() {

    function<uint32_t(string)> custom_hash = a_terrible_hash;
    Cache c(100, NULL, custom_hash);

    string key_1 = "ten";
    int32_t val_1 = 10;

    string key_2 = "twenty";
    int32_t val_2 = 20;

    string key_3 = "thirty";
    int32_t val_3 = 30;

    c.set(key_1, &val_1, sizeof(int32_t));

    c.set(key_2, &val_2, sizeof(int32_t));

    c.set(key_3, &val_3, sizeof(int32_t));

}