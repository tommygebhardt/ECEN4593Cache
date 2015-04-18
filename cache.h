#ifndef CACHE_H
#define CACHE_H

struct block{
  bool dirty;
  bool valid;
  unsigned long long tag;
  unsigned int block_size;
  byte * data;
  block(unsigned int size){
    block_size = size;
    data = new byte[block_size];
    dirty = false;
    valid = false;
    tag = 0;
  }
};

struct set{
  block * blocks;
  unsigned int assoc;
  linkedlist LRU;
  set(unsigned int ways){
    blocks = new block[assoc];
    assoc = ways;
  }
}

class cache{
 private:
  unsigned int cache_size;
  unsigned int assoc;
  unsigned int block_size;
  unsigned int hit_time;
  unsigned int miss_time;
  unsigned int transfer_time;
  unsigned int bus_width;
  
}





#endif
