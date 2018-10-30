/*
A simple API for an eviction policy based on FIFO.
*/

#pragma once
#include <queue>
#include <functional>
#include <memory>
#include <string>

using namespace std;

struct Fifo {

    queue<string> evict_queue;
    string get_front_element();
    string get_eviction();
    void dequeue();
    void enqueue(string);
    bool q_empty();
    Fifo();
    ~Fifo();
};