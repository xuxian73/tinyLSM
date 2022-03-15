#pragma once

#define HEADER_SIZE (32)
#define FILTER_SIZE (10240)
#define TABLE_SIZE ((uint32_t)(1 << 21) - HEADER_SIZE - FILTER_SIZE)
#define TOMBSTONE "~DELETED~"
#define FILESUFFIX ".sst"
#define TABLELEVEL (4)