/*
Implementation of the API for Fifo class.
*/
#include <string>
#include "fifo.h"

using namespace std;

Fifo::Fifo() {

    queue<string> q;
    this-> evict_queue = q;
}

string
Fifo::get_front_element() {

    string k = "";
    if (!this-> q_empty()) {

        k = this-> evict_queue.front();
        this-> dequeue();
    }

    return k;
}

/*
Tried to make this more generic, since not every potential
eviction policy will be a queue. 
*/
string
Fifo::get_eviction() {

    return this-> get_front_element();
}

void
Fifo::dequeue() {

    this-> evict_queue.pop();
}

/*
This function called when a new value is
inserted into the cache.
*/
void
Fifo::enqueue(string key) {

    this-> evict_queue.push(key);
}

bool
Fifo::q_empty() {

    return this-> evict_queue.empty();
}

Fifo::~Fifo() {
}