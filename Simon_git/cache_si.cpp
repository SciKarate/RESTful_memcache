#include <iostream>
#include <unordered_map>
#include <string>
#include <utility>
#include <cstring>
#include <algorithm>
#include <tuple>
#include <queue>
#include <functional>
#include <memory>
#include <assert.h>
#include "cache.hh"
// #include "evictor.cpp"
// #include "cache_value.cpp"

using namespace std;


// definetly see our README for clarification on this!


///////////////////////////////////////////////////////////////////////////////////////
// FKA: fifo.cpp
///////////////////////////////////////////////////////////////////////////////////////

struct Fifo {

    queue<string> evict_queue;

    string get_front_element() {
        string k = "";
        if (!this-> q_empty()) {

            k = this-> evict_queue.front();
            this-> dequeue();
        }
        return k;
    }

    // Tried to make this more generic, since not every potential
    // eviction policy will be a queue. 
    string get_eviction() {
        return this-> get_front_element();
    }

    void dequeue(){
        this-> evict_queue.pop();
    }

    // This function called when a new value is
    // inserted into the cache.
    void enqueue(string key){
        this-> evict_queue.push(key);
    }

    bool q_empty(){
        return this-> evict_queue.empty();
    }

    Fifo(){
        queue<string> q;
        this-> evict_queue = q;
    }

    ~Fifo(){}
};






///////////////////////////////////////////////////////////////////////////////////////
// FKA: evictor.cpp
///////////////////////////////////////////////////////////////////////////////////////

struct Evictor {

    string get_evicted(){
        string k = (this-> eviction_queue)-> get_front_element();
        return k;
    }

    Fifo *eviction_queue;

    void update_evict_func(string key){
        (this-> eviction_queue)-> enqueue(key);
    }

    Evictor(){
        Fifo *queue = new Fifo;
        this-> eviction_queue = queue;
    }

    ~Evictor() {
        delete this-> eviction_queue;
    }
};






///////////////////////////////////////////////////////////////////////////////////////
// FKA: cache_value.cpp
///////////////////////////////////////////////////////////////////////////////////////
/*
Need to keep track of the sizes of values in the cache.
There is no way to get the size of a void* without casting it
which is why we need a struct that stores the size.
*/

struct Cache_Value {
    void *value = 0;
    uint32_t value_size;

    Cache_Value(){}

    uint32_t get_size() {
        return this-> value_size;
    }

    /*
    we can use free() because free() knows how to
    deallocate void*. If we used delete, we would first have
    to cast this-> value to a char array.
    */
    ~Cache_Value() {
        free(this-> value);
        this-> value = NULL;
    }

};


///////////////////////////////////////////////////////////////////////////////////////
// cache.cpp
///////////////////////////////////////////////////////////////////////////////////////


/*
Allocate space for a Cache_Value object which is stored in the cache.
The Cache_Value object stores a void* and the size of whatever that void* is pointing to
so that it can be dereferenced and freed.
*/
Cache_Value*
get_new_obj(void *val, uint32_t value_size) {

    Cache_Value *cache_val = new Cache_Value;
    cache_val-> value_size = value_size;

    void *v = calloc(value_size, sizeof(char) + 1);

    if (!v) {

        cout << "Failed to allocate space for value" << endl;
        exit(-1);
    }

    memcpy(v, val, value_size);
    cache_val-> value = v;

    assert(cache_val);
    return cache_val;
}


struct Cache::Impl {
    index_type maxmem_;
    evictor_type evictor_;
    hash_func hasher_;
    unordered_map<string, Cache_Value*> table_;
    Cache::index_type table_size_;
    Evictor *evictor_obj;
    Impl(index_type maxmem, evictor_type evictor, hash_func hasher)
        :maxmem_(maxmem), evictor_(evictor), hasher_(hasher) {
        };

    void set(key_type key, val_type val, index_type size);
    Cache::val_type get(key_type key);
    void del(key_type key);
    index_type space_used() const;
};


/*construct Cache object*/
Cache::Cache(index_type maxmem,
        evictor_type evictor,
        hash_func hasher)
    :pImpl_(new Impl(maxmem, evictor, hasher)) {
        unordered_map<string, Cache_Value*> t;
        pImpl_-> table_ = t;
        index_type table_size = 0;
        pImpl_-> table_size_ = table_size;
        Evictor *e = new Evictor;
        pImpl_-> evictor_obj = e;
    }


/*
Cache::set first checks if there is enough space for the new value.
If memory is full, items are evicted until there is enough space. If
every item is evicted and there is still not enough space, the new item is not
inserted.
*/
void
Cache::set(key_type key, val_type val, index_type size) {

    index_type new_size = this-> space_used() + size;
    bool should_evict = (new_size > pImpl_-> maxmem_);
    bool insert = (size < pImpl_-> maxmem_);

    if (!insert) {

        /*
        We don't want to evict any items if the new item
        is larger than the entire cache, so we skip the while loop by
        setting should_evict to false.
        */
        should_evict = false;
    }

    while (should_evict) {

        string removed_key = (pImpl_-> evictor_obj)-> get_evicted();
        Cache::val_type val = this-> get(removed_key);

        /*Make sure that the key returned by the evictor is still valid.
        It is possible that the key was deleted/written over, and the evictor never found out.*/
        if (val) {

            this-> del(removed_key);
            new_size = this-> space_used() + size;
            should_evict = (new_size > pImpl_-> maxmem_);

        }
    }

    if (insert) {

        pImpl_->table_size_ += size;

        string k = key;
        Cache_Value *v = get_new_obj((void *)val, size);

        /*
        unordered_map has unique keys, so count(key)
        always returns 1 or 0.
        */
        bool key_exists = pImpl_-> table_.count(key);
        if (key_exists) {

            this-> del(key);
        }

        pair<string, Cache_Value*> new_element = make_pair(k, v);
        pImpl_-> table_.insert(new_element);

        /*
        Make sure to add the new key to the evictor. If the evictor
        is the default, this means that the new key is enqueued to 
        a FIFO queue.
        */
        (pImpl_-> evictor_obj)-> update_evict_func(k);
    }

}

/*
Simple function that queries the hash table for a value.
The value is recieved by this function as a pointer to Cache_Value
object. This function returns a pointer to the value member of the
Cache_Value object.
*/
Cache::val_type
Cache::get(key_type key) const {

    Cache::val_type ret_val = NULL;
    unordered_map<string, Cache_Value*>::const_iterator element;
    element = pImpl_-> table_.find(key);

    if (element != pImpl_-> table_.end()) {

        ret_val = (element-> second)-> value;
    }

    return ret_val;
}

void
Cache::del(key_type key) {

    Cache::val_type val = NULL;

    unordered_map<string, Cache_Value*>::const_iterator element;
    element = pImpl_-> table_.find(key);

    if (element != pImpl_-> table_.end()) {

        val = (element-> second);
    }

    if (!val) {

        //cout << "Key: " << key << " does not exist." << endl;
    } else {

        Cache_Value *v;
        v = (Cache_Value *)val;
        pImpl_-> table_size_ -= (v-> value_size);
        delete v;
        pImpl_-> table_.erase(key);
    }

}


Cache::index_type
Cache::space_used() const {

    return pImpl_-> table_size_;
}

//free memory of elements that were not already deleted
Cache::~Cache() {

    string key;
    bool keys_left = true;
    /*
    I am making the assumption here that the evictor stores all the keys that may
    be in the cache. This may be incorrect for some policies, but I had the evictor_obj update the 
    eviction structure each time a new value was inserted.
    */
    while (keys_left) {

        key = (pImpl_-> evictor_obj)-> get_evicted();
        keys_left = !(key.empty());
        this-> del(key);
    }

    delete pImpl_-> evictor_obj;
}