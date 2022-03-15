#include "ssTableLevel.h"

ssTableLevel::ssTableLevel(uint32_t level, const std::string &dir):_dir(dir), _level(level), _time(0), _fileno(0) {
    std::vector<std::string> files;
    if (!utils::dirExists(_dir)) {
        utils::mkdir(_dir.c_str());
    }
    utils::scanDir(_dir, files);
    for (auto &file : files) {
        ssTableUnit* unit = new ssTableUnit(_dir + "/" + file);
        uint64_t fileno = std::stoi(file.substr(0, file.size() - 4));
        _fileno = std::max(_fileno, fileno);
        _time = std::max(_time, unit->getTimeStamp());
        
        if (_level == 0) {
            auto iter = _data.begin();
            for (; iter != _data.end(); ++iter) {
                if ((*iter)->getTimeStamp() < unit->getTimeStamp()) {
                    break;
                }
            }
            _data.insert(iter, unit);
        } else {
            auto iter = _data.begin();
            for (; iter != _data.end(); ++iter) {
                if ((*iter)->getMin() < unit->getMin()) {
                    break;
                }
            }
            _data.insert(iter, unit);
        }
    }
    _fileno += 1;
    _time += 1;
}

void ssTableLevel::compaction(std::vector<ssTableUnit*> old, ssTableUnit* now) {
    std::vector<ssTableLevel::kvItem> itemList = std::vector<ssTableLevel::kvItem>();
    for (auto iter = old.cbegin(); iter != old.cend(); ++iter) {
        ssTableUnit* unit = *iter;
        for (uint32_t i = 0; i < unit->getNum(); ++i) {
            itemList.push_back(kvItem(unit->_index[i].first, i, unit));
        }
    }
    auto oldIter = itemList.begin();
    uint32_t newIter = 0;
    while (oldIter != itemList.end() && newIter < now->getNum()) {
        std::pair<uint64_t, uint32_t> cur = now->_index[newIter];
        if ((*oldIter).key < cur.first) {
            ++oldIter;
        } else if ((*oldIter).key == cur.first) {
            *oldIter = kvItem(cur.first, cur.second, now);
            ++newIter;
            ++oldIter;
        } else {
            oldIter = itemList.insert(oldIter, kvItem(cur.first, newIter, now));
            ++oldIter;
            ++newIter;
        }
    }
    while (newIter < now->getNum()) {
        itemList.push_back(kvItem(now->_index[newIter].first, newIter, now));
        ++newIter;
    }

    std::vector<ssTableUnit*> allUnit = std::move(old);
    allUnit.push_back(now);
    std::map<ssTableUnit*, std::ifstream> unitStream;
    for (auto &unit : allUnit) {
        unitStream[unit] = std::ifstream();
        unitStream[unit].open(unit->_path, std::ios_base::in | std::ios_base::binary);
        if (!unitStream[unit].is_open()) {
            std::cerr << "ERROR: Fail to open file." << std::endl;
            exit(1);
        }
    }
    std::vector<ssTableUnit*> newUnits;
    std::vector<std::pair<uint64_t, std::string> > kvList;
    size_t size = 0;
    for (auto &item : itemList) {
        size_t valueSize = item.unit->getValueSize(item.index);
        uint32_t offset = item.unit->getOffset(item.index);
        char *tmp = new char[valueSize];
        unitStream[item.unit].seekg(offset, std::ios_base::beg);
        unitStream[item.unit].read(tmp, valueSize);
        if (size + valueSize + sizeof(uint64_t) + sizeof(uint32_t) > TABLE_SIZE) {
            newUnits.push_back(new ssTableUnit(_dir + "/" + std::to_string(_fileno++) + FILESUFFIX, _time++, kvList));
            kvList.clear();
            size = 0;
        }
        size += valueSize + sizeof(uint64_t) + sizeof(uint32_t);
        kvList.push_back(std::pair<uint64_t, std::string>(item.key, std::string(tmp, valueSize)));            
    }
    if (!kvList.empty()) {
        newUnits.push_back(new ssTableUnit(_dir + "/" + std::to_string(_fileno++) + FILESUFFIX, _time++, kvList));
    }
    for (auto &unit : allUnit) {
        unitStream[unit].close();
        unit->cleanFile();
        delete unit;
    }
    if (_data.empty()) {
        _data.insert(_data.end(), newUnits.cbegin(), newUnits.cend());
        return;
    }
    uint64_t minkey = newUnits.front()->getMin();
    for (auto iter = _data.begin(); iter != _data.end(); ++iter) {
        if ((*iter)->getMax() < minkey) {
            _data.insert(iter, newUnits.cbegin(), newUnits.cend());
            return;
        }
    }
}

bool ssTableLevel::insert(ssTableUnit *unit) {
    std::vector<ssTableUnit*> targetUnits;

    for (auto iter = _data.begin(); iter != _data.end();) {
        ssTableUnit* cur = *iter;
        if (cur->getMax() >= unit->getMin() && cur->getMin() <= unit->getMax()) {
            targetUnits.push_back(*iter);
            iter = _data.erase(iter);
        } else {
            ++iter;
        }
    }
    compaction(targetUnits, unit);
    return (_data.size() > (2 << _level));
}

bool ssTableLevel::insert(const std::vector<std::pair<uint64_t, std::string> >& kvList) {
    std::string path = _dir + "/" + std::to_string(_fileno) + FILESUFFIX;
    ssTableUnit* unit = new ssTableUnit(path, _time, kvList);
    ++_fileno;
    ++_time;
    _data.insert(_data.begin(), unit);
    return (_data.size() > (2 << _level));
}

std::vector<ssTableUnit*> ssTableLevel::getVictims(){
    std::vector<ssTableUnit*> ret;
    uint64_t minTime = INT64_MAX;
    size_t index;
    if (_level == 0) {
        ret = _data;
        _data.clear();
        return ret;
    }
    while (_data.size() > (2 << _level)) {
        for (size_t i = 0; i < _data.size(); ++i) {
            if (_data[i]->getTimeStamp() < minTime) {
                index = i;
                minTime = _data[i]->getTimeStamp();
            }
        }
        ret.push_back(_data[index]);
        _data.erase(_data.begin() + index);
        minTime = INT64_MAX;
    }
    return ret;
}

std::pair<bool, std::string> ssTableLevel::get(uint64_t key) {
    /* simply traverse through whole level here and do not differenciate level 0 and others */
    /* level-0 from new to old */
    for (auto iter = _data.cbegin(); iter < _data.cend(); ++iter){
        ssTableUnit* cur = *iter;
        if(cur->getMin() <= key && cur->getMax() >= key) {
            return cur->get(key);
        }
    }
    return std::pair<bool, std::string>(false, "");
}

void ssTableLevel::clear() {
    while (!_data.empty()) {
        _data.back()->cleanFile();
        delete _data.back();
        _data.pop_back();
    }
}

ssTableLevel::~ssTableLevel() {
    for (auto iter = _data.begin(); iter != _data.end(); ++iter) {
        delete *iter;
    }
}