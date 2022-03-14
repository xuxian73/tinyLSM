#include <cstdint>
#include <string>
#include <vector>
#include <fstream>
#include "ssTableLevel.h"
#include "skipList.h"
#include "utils.h"

class ssTable {
private:
    std::string _dir;

    std::vector<ssTableLevel*> _table;
public:
    ssTable(const std::string &dir);

    ~ssTable();

    /**
     * Given key get the value
     * @return <if find the key, the value(can be TOMBSTONE)>
     */
    std::pair<bool, std::string> get(uint64_t key);

    void insert(const skipList& memtable);

    void makeTableUnit();

    void clear();
};