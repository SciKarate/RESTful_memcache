//Josh Reiss and Ryan Neumann
# CS389_HW3

Part 1: Tests in cache_testy.cc

# CS389_HW2

Part 1: Stored in cache.cc
Part 2: Stored in cache_tester.cc

Part 3: Hash function written in cache_tester.cc; cache.cc implements said function.

Part 4: cache.cc uses an unordered_map, which uses bucket hashing to resolve collisions. No further implementation is necessary.

Part 5: cache.cc uses an unordered_map, which includes the built-in function void rehash(n), and
	"Rehashes are automatically performed by the container whenever its load factor is going to surpass its max_load_factor in an operation." -cplusplus.com
The max_load_factor is set to 0.5 in cache.cc, on line 18, as per instruction.

Part 6: Eviction policy is written in cache.cc, within "set".
		Our evictor uses a queue structure that stores key pointers and value sizes in order to implement FIFO





Things I can't figure out:

	* How is get supposed to work, exactly? Should it be returning void pointers or values? Is my current solution (returning void pointers and then casting them manually in main) correct? 

	* What is index_type& val_size for?

	* Is it normal that I need an individual function for each cast, or is there a universal method I just haven't found after a week of searching?

	* How can I get my Cache to have a default hash function without having to declare a new function within Cache?