#pragma once

#include <cstdio>
#include <cstdint>
#include <fstream>
#include <iostream>

class ssTableHeader {
public:
    uint64_t timestamp;
    uint64_t num;
    int64_t min;
    int64_t max;

    explicit ssTableHeader(uint64_t t = 0, uint64_t n = 0, int64_t Min = 0, int64_t Max = 0):
        timestamp(t), num(n), min(Min), max(Max) {}

    void readHeader(const std::string& path);

    void writeHeader(const std::string& path);
};

