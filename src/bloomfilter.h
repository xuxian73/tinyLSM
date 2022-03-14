#include <iostream>
#include <cstdint>
#include <cstdlib>
#include <string>
#include <fstream>
#include "MurmurHash3.h"
#include "common.h"

class bloomfilter {
private:
    void *_filter;

public:
    bloomfilter();

    ~bloomfilter();

    void clean();

    bool isFiltered(uint64_t key) const;

    void addKey(uint64_t key);

    void readFilter(const std::string& path);

    void writeFilter(const std::string& path);
};