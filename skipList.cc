#include "skipList.h"

skipList::skipList(): _size(0) {
    _head = new skipListNode();
}

skipList::~skipList() {
    this->clear();
    delete _head;
}

std::vector<skipList::skipListNode*> skipList::getLeftNeighbor(uint64_t key) {
    std::vector<skipListNode*> neighbor;
    skipListNode* cur = _head;
    while (cur) {
        while (cur->right && cur->right->key < key) {
            cur = cur->right;
        }
        neighbor.push_back(cur);
        cur = cur->down;
    }
    return neighbor;
}

std::pair<bool, bool> skipList::putOrDel(uint64_t key, const std::string &s) {
    std::vector<skipListNode*> leftNeighbor = this->getLeftNeighbor(key);
    skipListNode* cur = leftNeighbor.back();
    std::pair<bool, bool> ret;
    if (cur->right && cur->right->key == key) {
        cur = cur->right;
        ret.second = cur->value != TOMBSTONE;
        size_t size = _size + s.length() * sizeof(char) - cur->value.length() * sizeof(char);
        if (size > MEMTABLE_SIZE) {
            ret.first = false;
            return ret;
        }
        _size = size;
        cur->key = key;
        cur->value = s;
        ret.first = true;
        return ret;
    } else {
        ret.second = false;
        size_t size = _size + s.length() * sizeof(char) + sizeof(uint64_t) + sizeof(uint32_t);
        if (size > MEMTABLE_SIZE) {
            ret.first = false;
            return ret;
        }
        _size = size;
        uint32_t level = getRandomLevel(leftNeighbor.size());
        skipListNode* child = nullptr;
        while (level > 0 && !leftNeighbor.empty()) {
            skipListNode* p = leftNeighbor.back();
            p->right = new skipListNode(p->right, child, key, s);
            leftNeighbor.pop_back();
            child = p->right;
        }
        if (level > 0) {
            _head = new skipListNode(new skipListNode(nullptr, child, key, s), _head, 0, "");
        }
        ret.first = true;
        return ret;
    }
}

bool skipList::put(uint64_t key, const std::string &s) {
    return putOrDel(key, s).first;
}

bool skipList::get(uint64_t key, std::string* s) {
    std::vector<skipListNode*> leftNeighbor = getLeftNeighbor(key);
    skipListNode* cur = leftNeighbor.back();
    if (cur->right && cur->right->key == key) {
        if (cur->right->value == TOMBSTONE) {
            return false;
        }
        *s = cur->right->value;
        return true;
    }    
    return false;
}

std::pair<bool, bool> skipList::del(uint64_t key){
    return putOrDel(key, TOMBSTONE);
}

void skipList::clear() {
    skipListNode* cur = _head;
    while (cur) {
        skipListNode* p = cur;
        cur = cur->down;
        while (p) {
            skipListNode* oldp = p;
            p = p->right;
            delete oldp;
        }
    }
    _head = new skipListNode();
    _size = 0;
}