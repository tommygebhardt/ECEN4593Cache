#ifndef CACHE_H
#define CACHE_H
struct stats{
  unsigned long long int exec_time;
  unsigned long long int flush_time;

  unsigned long long int L1I_hit_count;
  unsigned long long int L1D_hit_count;
  unsigned long long int L1I_miss_count;
  unsigned long long int L1D_miss_count;
  unsigned long long int L2_hit_count;
  unsigned long long int L2_miss_count;
  
  unsigned long long int kickouts;
  unsigned long long int dirty_kickouts;

  unsigned long long int transfers;

  unsigned long long int inst_count;
  unsigned long long int read_count;
  unsigned long long int write_count;
  unsigned long long int total_count;
  stats(){
    exec_time = 0;
    flush_time = 0;
    L1I_hit_count = 0;
    L1D_hit_count = 0;
    L1I_miss_count = 0;
    L1D_miss_count = 0;
    L2_hit_count = 0;
    L2_miss_count = 0;
    kickouts = 0;
    dirty_kickouts = 0;
    transfers = 0;
    inst_count = 0;
    read_count = 0;
    write_count = 0;
    total_count = 0;
  }
};

struct block{
  bool dirty;
  bool valid;
  unsigned long long tag;
  unsigned int block_size;
  block(){
    dirty = false;
    valid = false;
    tag = 0;
    block_size = 0;
  }
};

struct set{
  block * blocks;
  unsigned int assoc;
  int * LRU;
};

class cache{
 private:
  unsigned int cache_size;
  unsigned int table_size;
  unsigned int assoc;
  unsigned int block_size;
  int hit_time;
  int miss_time;
  int transfer_time;
  int bus_width;
  set * table;

 public:
  cache(unsigned int csize, unsigned int ways, unsigned int bsize, int htime, int mtime, int trantime, int bwidth);
  bool read(unsigned long long int address, unsigned int bytesize);
  bool write(unsigned long long int address, unsigned int bytesize);
  void flush();
  void printInfo();
};

#endif
