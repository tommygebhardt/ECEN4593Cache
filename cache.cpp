#include "cache.h"

cache::cache(unsigned int csize, unsigned int ways, unsigned int bsize, int htime, int mtime, int trantime, int bwidth){
  cache_size = csize;
  assoc = ways;
  block_size = bsize;
  hit_time = htime;
  miss_time = mtime;
  transfer_time = trantime;
  bus_width = bwidth;
  table_size = (cache_size/block_size)/assoc;
  table = new set[table_size];
  //Initialize all sets
  for (int i = 0; i < table_size; i++){
    table[i].assoc = assoc;
    table[i].blocks = new block[assoc];

    //Initialize LRU

    //I believe this is unneccessary. It can be added back in if it becomes necessary
    /*for (int j = 0; j < assoc; j++){
      table[i].blocks[j].block_size = block_size;
      }*/
  }
}
