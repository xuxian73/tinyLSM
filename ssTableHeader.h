#pragma once

#include <stdio.h>
#include <stdint.h>

class ssTableHeader {
    uint64_t timestamp;
    uint64_t num;
    int64_t min;
    int64_t max;

    explicit ssTableHeader(uint64_t t, uint64_t n, int64_t Min, int64_t Max):
        timestamp(t), num(n), min(Min), max(Max) {}

};

