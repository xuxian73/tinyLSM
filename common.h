#pragma once

#define HEADER_SIZE (32)
#define FILTER_SIZE (10240)
#define MEMTABLE_SIZE ((1 << 21) - HEADER_SIZE - FILTER_SIZE)
#define TOMBSTONE "~DELETED~"