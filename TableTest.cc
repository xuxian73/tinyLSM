#include "ssTable.h"

int main() {
    ssTable table = ssTable("./data");
    std::vector<std::pair<uint64_t, std::string> > kvList;
    for (int i = 0; i < 1000; ++i) {
        kvList.push_back(std::pair<uint64_t, std::string>(3 * i, std::string(i, 's')) );
    }
    ssTableUnit* unit = new ssTableUnit("./data/level-0/1.sst", 0, kvList);
    
    kvList.clear();
    for (int i = 0; i < 1000; ++i) {
        kvList.push_back(std::pair<uint64_t, std::string>(3 * i + 1, std::string(2 * i, 's')) );
    }
    delete unit;
    unit = new ssTableUnit("./data/level-0/2.sst", 1, kvList);

    kvList.clear();
    for (int i = 0; i < 1000; ++i) {
        kvList.push_back(std::pair<uint64_t, std::string>(3 * i + 2, std::string(2 * i, 's')) );
    }
    delete unit;
    unit = new ssTableUnit("./data/level-0/3.sst", 2, kvList);

    delete unit;
}