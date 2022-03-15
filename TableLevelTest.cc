#include "ssTableLevel.h"

int main() {
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

    ssTableLevel* level0 = new ssTableLevel(0, "./data/level-0");
    ssTableLevel* level1 = new ssTableLevel(1, "./data/level-1");
    std::vector<ssTableUnit*> victims = level0->getVictims();
    while (!victims.empty()) {
        ssTableUnit* victim = victims.back();
        level1->insert(victim);
        victims.pop_back();
    }
    for (int i = 0; i < 1000; ++i) {
        std::pair<bool, std::string> ret = level1->get(3 * i);
        std::cout << "key: " << i << " len: " << ret.second.length() << std::endl;
        ret = level1->get(3 * i + 1);
        std::cout << "key: " << i << " len: " << ret.second.length() << std::endl;
        ret = level1->get(3 * i + 2);
        std::cout << "key: " << i << " len: " << ret.second.length() << std::endl;
    }
    level1->clear();
    level0->clear();
    delete level0;
    delete level1;
    return 0;
}