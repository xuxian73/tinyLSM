#pragma once

#include <vector>
#include "kvstore_api.h"
#include "skipList.h"
#include "ssTable.h"

class KVStore : public KVStoreAPI {
	// You can add your implementation here
private:
	std::string _dir;

	skipList _memTable;

	ssTable _ssTable;
public:
	KVStore(const std::string &dir);

	~KVStore();

	void put(uint64_t key, const std::string &s) override;

	std::string get(uint64_t key) override;

	bool del(uint64_t key) override;

	void reset() override;

};
