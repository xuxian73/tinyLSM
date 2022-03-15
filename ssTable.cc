#include "ssTable.h"

ssTable::ssTable(const std::string &dir):_dir(dir), _table(std::vector<ssTableLevel*>()) {
    uint32_t level = 0;
    std::string level_dir = dir + "/level-" + std::to_string(level);
    while (utils::dirExists(level_dir)) {
        ssTableLevel* tableLevel = new ssTableLevel(level, level_dir);
        _table.push_back(tableLevel);
        ++level;
        level_dir = dir + "/level-" + std::to_string(level);
    }
    if (_table.empty()) {
        utils::mkdir(level_dir.c_str());
        ssTableLevel* tableLevel = new ssTableLevel(0, level_dir);
        _table.push_back(tableLevel);
    }
}

ssTable::~ssTable() {
    for (size_t i = 0; i < _table.size(); ++i) {
        delete _table[i];
    }
}

std::pair<bool, std::string> ssTable::get(uint64_t key) {
    std::pair<bool, std::string> ret;
    for (size_t i = 0; i < _table.size(); ++i) {
        ret = _table[i]->get(key);
        if (ret.first) {
            break;
        }
    }
    return ret;
}

void ssTable::insert(const skipList& memtable){
    std::vector<std::pair<uint64_t, std::string> > kvList = memtable.toSortedList();
    uint32_t cur = 1;
    
    if ( _table[0]->insert(kvList)) {
        std::vector<ssTableUnit*> victims = _table[0]->getVictims();
        while (!victims.empty()) {
            bool needCompaction = false;
            if (cur >= _table.size()) {
                std::string level_dir = _dir + "/level-" + std::to_string(cur);
                if (!utils::dirExists(level_dir)) {
                    utils::mkdir(level_dir.c_str());
                }
                _table.push_back(new ssTableLevel(cur, level_dir));
            }
            while (!victims.empty()) {
                ssTableUnit* victim = victims.back();
                needCompaction = _table[cur]->insert(victim);
                victims.pop_back();
            }
            if (needCompaction) {
                victims = _table[cur]->getVictims();
                ++cur;
            }
        }
    }
}

void ssTable::clear() {
    while (!_table.empty()) {
        _table.back()->clear();
        delete _table.back();
        _table.pop_back();
    }
    for (size_t i = 0; i < _table.size(); ++i) {
        ssTableLevel* tableLevel = new ssTableLevel(i, _dir);
        _table.push_back(tableLevel);
    }
}