#include "kvstore.h"
#include <string>

KVStore::KVStore(const std::string &dir): KVStoreAPI(dir), _ssTable(dir)
{
	_dir = dir;
}

KVStore::~KVStore()
{
}

/**
 * Insert/Update the key-value pair.
 * No return values for simplicity.
 */
void KVStore::put(uint64_t key, const std::string &s)
{
	bool res = _memTable.put(key, s);
	if (!res) {
		_ssTable.insert(_memTable);
		_memTable.clear();
	}
	_memTable.put(key, s);
}
/**
 * Returns the (string) value of the given key.
 * An empty string indicates not found.
 */
std::string KVStore::get(uint64_t key)
{
	std::string ret;
	bool res = _memTable.get(key, &ret);
	if (!res) {
		std::pair<bool, std::string> res = _ssTable.get(key);
		if (!res.first || res.second == "~DELETED~") {
			return "";
		}
		return res.second;
	}
	return ret;
}

/**
 * Delete the given key-value pair if it exists.
 * Returns false iff the key is not found.
 */
bool KVStore::del(uint64_t key)
{
	std::pair<bool, bool> res = _memTable.del(key);
	bool ret = false;
	if (res.second) {
		ret = true;
	} else {
		std::pair<bool, std::string> value = _ssTable.get(key);
		if (value.first && value.second != TOMBSTONE) {
			ret = true;
		}
	}
	if (!res.first) {
		_ssTable.insert(_memTable);
		_memTable.clear();
	} 
	return ret;
}

/**
 * This resets the kvstore. All key-value pairs should be removed,
 * including memtable and all sstables files.
 */
void KVStore::reset()
{
	_memTable.clear();
	_ssTable.clear();
}
