#include <stdio.h>
#include <stdint.h>
#include <vector>
#include <string>
#include "ssTableHeader.h"
#include "bloomfilter.h"
class ssTableUnit {
    ssTableHeader header;
    bloomfilter filter;
    std::vector<std::pair<uint64_t, uint32_t>> index;
    std::string dir;

    ssTableUnit(std::string);
    
};