#include <cstdint>
#include <vector>
#include <string>
#include "ssTableHeader.h"
#include "bloomfilter.h"
#include "skipList.h"
#include "utils.h"

class ssTableUnit {
friend class ssTableLevel;

    ssTableHeader _header;
    bloomfilter _filter;
    std::vector<std::pair<uint64_t, uint32_t> > _index;
    std::string _path;

public:
    /**
     * Construct a new ssTableUnit from memTable
     * @param path file path to save
     * @param timestamp
     * @param data pointer to the sorted linklist of memTable
     */
    ssTableUnit(std::string path, uint64_t timestamp, const std::vector<std::pair<uint64_t, std::string> >& kvList);
    
    /**
     * Construct a ssTableUnit from file system
     * @param path file path to read from
     */ 
    ssTableUnit(const std::string& path);

    /**
     * Given key get the value
     * @return <if find the key, the value(can be TOMBSTONE)>
     */
    std::pair<bool, std::string> get(uint64_t key) const;

    /**
     * clean the file and ssTable, used when compaction and reset
     */
    void cleanFile();

    int64_t getMin() { return _header.min; }

    int64_t getMax() { return _header.max; }

    uint64_t getNum() { return _header.num; }

    uint64_t getTimeStamp() { return _header.timestamp; }

    uint32_t getValueSize(uint32_t ind) { return _index[ind + 1].second - _index[ind].second; }

    uint32_t getOffset(uint32_t ind) { return _index[ind].second; }

    std::vector<std::pair<uint64_t, std::string> > toSortedList();
    
    ~ssTableUnit() {}
};