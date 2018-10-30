#include "evictor.h"

using namespace std;


Evictor::Evictor() {

    Fifo *queue = new Fifo;
    this-> eviction_queue = queue;
}


string
Evictor::get_evicted() {
    string k = (this-> eviction_queue)-> get_front_element();
    return k;
}


void 
Evictor::update_evict_func(string key) {

    (this-> eviction_queue)-> enqueue(key);
}


Evictor::~Evictor() {

    delete this-> eviction_queue;
}