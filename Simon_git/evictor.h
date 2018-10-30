#pragma once
#include <functional>
#include <memory>
#include <string>
#include "fifo.cpp"

struct Evictor {

    std::string get_evicted();
    Fifo *eviction_queue;
    void update_evict_func(std::string key);
    Evictor();
    ~Evictor();
};