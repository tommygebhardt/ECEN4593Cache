#ifndef CACHE_H
#define CACHE_H

#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstdio>
#include <cstring>
#include "lru_stack.h"

extern int mem_sendaddr;
extern int mem_ready;
extern int mem_chunktime;
extern int mem_chunksize;

struct stats{
    unsigned long long int exec_time;
    unsigned long long int flush_time;
    unsigned long long int inst_count;
    unsigned long long int read_count;
    unsigned long long int write_count;
    unsigned long long int total_count;
    unsigned long long int req_count;
    unsigned int flushes;
    unsigned long long int read_time;
    unsigned long long int write_time;
    unsigned long long int inst_time;
    stats()
    {
    exec_time = 0;
    flush_time = 0;
    inst_count = 0;
    read_count = 0;
    write_count = 0;
    total_count = 0;
    req_count = 0;
    flushes = 0;
    read_time = 0;
    write_time = 0;
    inst_time = 0;
    }
};

extern stats execution;

struct block
{
    bool dirty;
    bool valid;
    unsigned long long int tag;
    block()
    {
    dirty = false;
    valid = false;
    tag = 0;
    }
};

struct set
{
    block * blocks;
    unsigned int assoc;
    LRU_stack * LRU;
};

class cache
{
 private:
    unsigned int cache_size;
    unsigned int table_size;
    unsigned int block_size;

    unsigned int block_offset;
    unsigned int index_offset;

    unsigned int assoc;
    unsigned int hit_time;
    unsigned int miss_time;
    unsigned int transfer_time;
    unsigned int bus_width;
    set * table;
    cache * lower_level;
    unsigned int transfers_per_block;

    unsigned long long int hit_count;
    unsigned long long int miss_count;
    unsigned long long int requests;
    unsigned long long int kickouts;
    unsigned long long int dirty_kickouts;
    unsigned long long int transfers;
    unsigned long long int flush_kickouts;

 public:
    cache(unsigned int csize, unsigned int ways, unsigned int bsize, unsigned int htime,
      unsigned int mtime, unsigned int trantime, unsigned int bwidth, cache * lower = NULL);
    unsigned long long int read(unsigned long long int address);
    unsigned long long int write(unsigned long long int address);
    unsigned long long int flush();
    void printInfo();
    void printCounts();
    void printCache();
    // Function to print formatted summary of all statistics
    friend void print_all_stats(cache& L1I, cache& L1D, cache& L2);
};

// Computes log base 2 of x
unsigned int log2(unsigned int x);


#endif
