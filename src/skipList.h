#pragma once

#include <string>
#include <vector>
#include <random>
#include <iostream>
#include "common.h"

class skipList {
private:
    struct skipListNode {
        skipListNode* right;
        skipListNode* down;
        uint64_t key;
        std::string value;

        skipListNode():right(nullptr), down(nullptr), key(0), value("")  {}
        skipListNode(skipListNode* r, skipListNode* d, uint64_t k, std::string v):
            right(r), down(d), key(k), value(v){}
    };
    skipListNode* _head;
    size_t _size;

    uint32_t getRandomLevel(size_t level) {
        std::random_device rd;
        std::default_random_engine eng(rd());
        std::uniform_real_distribution<uint8_t> dis(0, 1);
        uint32_t l = 1;
        while (dis(eng) && l <= level) {
            ++l;
        }
        return l;
    }

    /**
     *  @return first: success put? / second: key found?
     **/
    std::pair<bool, bool> putOrDel(uint64_t key, const std::string &s);

    std::vector<skipListNode*> getLeftNeighbor(uint64_t key);
    
public:
    skipList();

    ~skipList();

    bool put(uint64_t key, const std::string &s);

    bool get(uint64_t key, std::string* s);

    std::pair<bool, bool> del(uint64_t key);

    std::vector<std::pair<uint64_t, std::string> > toSortedList() const;

    void clear();
};