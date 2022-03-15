#include "ssTableUnit.h"

int main() {
    std::vector<std::pair<uint64_t, std::string> > kvList;
    for (int i = 0; i < 100; ++i) {
        kvList.push_back(std::pair<uint64_t, std::string>(i, std::string(i, 's')) );
    }
    ssTableUnit* unit = new ssTableUnit("./data/unit.sst", 0, kvList);
    ssTableUnit* unit2 = new ssTableUnit("./data/unit.sst");
    for (int i = 0; i < 100; ++i) {
        std::cout << unit2->get(i).second << std::endl;
    }
    kvList = unit2->toSortedList();
    for (int i = 0; i < 100; ++i) {
        std::cout << kvList[i].second << std::endl;
    }
    unit2->cleanFile();
    return 0;
}