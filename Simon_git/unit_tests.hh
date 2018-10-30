#include <string>

template <typename T>
bool
correct_value_for_key(std::string key, Cache &c, T expected);

void
single_insertion_test();

void
multiple_insertion_test();

void
multiple_type_insertion_test();

void
insert_existing_key_test();

void
delete_single_key_test();

void
delete_multiple_key_test();

void
evictor_test();

int
a_terrible_hash(std::string key);

void
test_custom_hash();

