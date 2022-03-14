#include "bloomfilter.h"

bloomfilter::bloomfilter() {
    _filter = malloc(FILTER_SIZE);
    if (!_filter) {
        std::cerr << "Fatal: unable to allocate filter" << std::endl;
        exit(1);
    }
    clean();
}

bloomfilter::~bloomfilter() {
    free(_filter);
}

void bloomfilter::clean() {
    memset(_filter, 0, FILTER_SIZE);
}

bool bloomfilter::isFiltered(uint64_t key) const {
    unsigned int hash[4] = {0};
    MurmurHash3_x64_128(&key, sizeof(key), 1, hash);
    for (int i = 0; i < 4; ++i) {
        uint32_t bit = hash[i] % (FILTER_SIZE << 3);
        if (((char*)_filter)[bit >> 3] & (1 << (bit % 8))) {
            return true;
        }
    }
    return false;
}

void bloomfilter::addKey(uint64_t key) {
    unsigned int hash[4] = {0};
    MurmurHash3_x64_128(&key, sizeof(key), 1, hash);
    for (int i = 0; i < 4; ++i) {
        uint32_t bit = hash[i] % (FILTER_SIZE << 3);
        ((char*)_filter)[bit >> 3] &= 1 << (bit % 8);
    }
}

void bloomfilter::readFilter(const std::string& path) {
    std::ifstream in;
    in.open(path, std::ios_base::in | std::ios_base::binary);
    if (!in.is_open()) {
        std::cerr << "Fatal: failed to open file " << path << std::endl; 
        exit(1);
    }
    
    in.seekg(HEADER_SIZE, std::ios_base::beg);
    in.read((char*)_filter, FILTER_SIZE);

    if (!in.good()) {
        std::cerr << "Fatal: Read filter error on " << path << std::endl; 
        exit(1);
    }
    in.close();
}

void bloomfilter::writeFilter(const std::string& path) {
    std::ofstream out;
    out.open(path, std::ios_base::out | std::ios_base::binary);
    if (!out.is_open()) {
        std::cerr << "Fatal: failed to open file " << path << std::endl; 
        exit(1);
    }

    out.seekp(HEADER_SIZE, std::ios_base::beg);
    out.write((char*)_filter, FILTER_SIZE);

    if (!out.good()) {
        std::cerr << "Fatal: Write filter error on" << path << std::endl;
        exit(1);
    }
    out.close();
}