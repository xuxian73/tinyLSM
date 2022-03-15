#include "ssTableUnit.h"

ssTableUnit::ssTableUnit(std::string path, uint64_t timestamp, const std::vector<std::pair<uint64_t, std::string> >& kvList) {
    _header = ssTableHeader(timestamp, kvList.size(), kvList.front().first, kvList.back().first);
    this->_path = path;
    
    size_t offset = HEADER_SIZE + FILTER_SIZE + kvList.size() * (sizeof(uint64_t) + sizeof(uint32_t));
    for (auto iter = kvList.cbegin(); iter != kvList.cend(); ++iter) {
        _index.push_back(std::pair<uint64_t, uint32_t>(iter->first, offset));
        offset += iter->second.size() * sizeof(char);
        _filter.addKey(iter->first);
    }
    _index.push_back(std::pair<uint64_t, uint32_t>(0, offset));

    _header.writeHeader(path);
    _filter.writeFilter(path);
    std::ofstream out;
    out.open(path, std::ios_base::out | std::ios_base::binary | std::ios_base::app);
    if (!out.is_open()) {
        std::cerr << "Fatal: Failed to create ssTableUnit file " << std::endl;
    }

    for (auto iter = _index.cbegin(); iter != _index.cend() - 1; ++iter) {
        out.write((char*)&iter->first, sizeof(uint64_t));
        out.write((char*)&iter->second, sizeof(uint32_t));
    }
    for (auto iter = kvList.cbegin(); iter != kvList.cend(); ++iter) {
        out.write((char*)iter->second.c_str(), iter->second.size() * sizeof(char));
    }
    if (!out.good()) {
        std::cerr << "ERROR: Fail to write filter." << std::endl;
        exit(1);
    }
    out.close();
}

ssTableUnit::ssTableUnit(const std::string& path) {
    std::ifstream in;
    in.open(path, std::ios_base::in | std::ios_base::binary);
    _header.readHeader(path);
    _filter.readFilter(path);
    _path = path;
    if (!in.is_open()) {
        std::cerr << "Fatal: Failed to load ssTabelUnit " << path << std::endl;
        exit(1);
    }
    in.seekg(HEADER_SIZE + FILTER_SIZE, std::ios_base::beg);
    for (uint64_t i = 0; i < _header.num; ++i) {
        std::pair<uint64_t, uint32_t> keyOffset;
        in.read((char*)&(keyOffset.first), sizeof(uint64_t));
        in.read((char*)&(keyOffset.second), sizeof(uint32_t));
        _index.push_back(keyOffset);
    }
    in.seekg(0, std::ios_base::end);
    _index.push_back(std::pair<uint64_t, uint32_t>(0, in.tellg()));
    in.close();
}

std::pair<bool, std::string> ssTableUnit::get(uint64_t key) const {
    uint32_t l = 0, r = _header.num - 1, m;
    uint32_t offset;
    /* the ssTableLevel has checked min <= key <= max */
    if (_filter.isFiltered(key)) {
        return std::pair<bool, std::string>(false, std::string());
    }
    while (l <= r) {
        m = (l + r) / 2;
        if (_index[m].first < key) {
            l = m + 1;
        } else if (_index[m].first > key) {
            r = m - 1;
        } else {
            std::ifstream in;
            in.open(_path, std::ios_base::in | std::ios_base::binary);
            offset = _index[m].second;
            if (!in.is_open()) {
                std::cerr << "Fatal: Failed to load ssTabelUnit " << _path << std::endl;
                exit(1);
            }
            size_t size = _index[m + 1].second - _index[m].second;
            char* tmp = new char [size];
            in.seekg(offset, std::ios_base::beg);
            in.read(tmp, size);
            if (!in.good()) {
                std::cerr << "Fatal: Failed to load ssTableUnit" << std::endl;
            }
            in.close();
            return std::pair<bool, std::string>(true, std::string(tmp, size));
        }
    }
    return std::pair<bool, std::string>(false, std::string());
}

void ssTableUnit::cleanFile() {
    if (utils::rmfile(_path.c_str()) != 0){
        std::cerr << "Fatal: Failed to cleanFile" << _path << std::endl;
        exit(1);
    }
}

std::vector<std::pair<uint64_t, std::string> > ssTableUnit::toSortedList() {
    std::ifstream in;
    std::vector<std::pair<uint64_t, std::string> > ret;
    in.open(_path, std::ios_base::in | std::ios_base::binary);
    uint64_t offset = HEADER_SIZE + FILTER_SIZE + _header.num * (sizeof(uint64_t) + sizeof(uint32_t));
    in.seekg(offset, std::ios_base::beg);
    for (auto iter = _index.begin(); iter < _index.end() - 1; ++iter) { 
        size_t size = (*(iter + 1)).second - (*iter).second; 
        char str[size];
        in.read(str, size);
        std::string tmp(str, size);
        ret.push_back(std::pair<uint64_t, std::string>((*iter).first, tmp));
    }
    if (!in.good()) {
        std::cerr << "Fatal: failed to read ssTable " << _path << std::endl;
    }
    in.close();
    return ret;
}