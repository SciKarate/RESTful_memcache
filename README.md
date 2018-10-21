# CS389_HW2

Part 1: Stored in cache.cc
Part 2: Stored in cache_tester.cc

Part 3: Hash function written in cache_tester.cc; cache.cc implements said function.

Part 4: cache.cc uses an unordered_map, which uses bucket hashing to resolve collisions. No further implementation is necessary.

Part 5: cache.cc uses an unordered_map, which includes the built-in function void rehash(n), and
	"Rehashes are automatically performed by the container whenever its load factor is going to surpass its max_load_factor in an operation." -cplusplus.com
The max_load_factor is set to 0.5 in cache.cc, on line 18, as per instruction.

Part 6: Eviction policy is written in cache.cc