## KVStore using Log-structured Merge Tree

A trivial implementation of LSM tree.
### Feature
- bloomfilter
- skipList(memtable)

- [ ] WAL
- [ ] compaction in daemon

### Project structrue:

```text
.
├── Makefile  // Makefile if you use GNU Make
├── README.md // This readme file
├── correctness.cc // Correctness test, you should not modify this file
├── data      // Data directory used in our test
├── kvstore.cc     // your implementation
├── kvstore.h      // your implementation
├── skipList.cc    // memTable implemented using skipList
├── skipList.h
├── bloomfilter.cc // bloomfilter used to improve searching
├── bloomfilter.h
├── ssTableHeader.cc // header of each ssTable, storing each sstable's metadata
├── ssTableHeader.h
├── ssTableUnit.cc // each ssTable data structure, including header, key-offset pair and value
├── ssTableUnit.h
├── ssTableLevel.cc // level of ssTable, grouped by ssTableUnit in the same layer
├── ssTableLevel.h
├── ssTable.cc // ssTable, grouped by different layer of ssTableLevel
├── ssTable.h
├── kvstore_api.h  // KVStoreAPI, you should not modify this file
├── persistence.cc // Persistence test, you should not modify this file
├── utils.h         // Provides some cross-platform file/directory interface
├── MurmurHash3.h  // Provides murmur3 hash function
└── test.h         // Base class for testing, you should not modify this file
```

