#include <cstdint>
#include <string>
#include <vector>
#include <map>
#include "ssTableUnit.h"
#include "utils.h"

class ssTableLevel {
private:
    std::string _dir;

    uint32_t _level;

    uint64_t _time;

    uint64_t _fileno;

    std::vector<ssTableUnit*> _data;

    struct kvItem {
        uint64_t key;
        uint32_t index;
        ssTableUnit* unit;

        explicit kvItem(uint64_t k = 0, uint32_t i = 0, ssTableUnit *u = nullptr): key(k), index(i), unit(u) {}

        bool operator == (const ssTableLevel::kvItem& other) const {
            return key == other.key;
        }
        bool operator < (const ssTableLevel::kvItem& other) const {
            return key < other.key;
        }
    };

public:
    /**
     * construct from dir level-*
     */ 
    ssTableLevel(uint32_t level, const std::string& dir);

    /**
     * @return is overflow
     */
    bool insert(ssTableUnit* unit);

    /**
     * For level 0, using a kvList construct a ssTableUnit, then insert
     * @return if level 0 is overflow
     */
    bool insert(const std::vector<std::pair<uint64_t, std::string> >& kvList);

    void compaction(std::vector<ssTableUnit*> old, ssTableUnit* now);
    
    /**
     * @return the selected victim for compaction
     */
    std::vector<ssTableUnit*> getVictims();

    /**
     * Given key get the value
     * @return <if find the key, the value(can be TOMBSTONE)>
     */
    std::pair<bool, std::string> get(uint64_t key);

    void clear();

    ~ssTableLevel();
};