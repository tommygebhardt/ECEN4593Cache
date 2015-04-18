#include "cache.h"
#include <iostream>
using namespace std;

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
    for (unsigned int i = 0; i < table_size; i++){
	table[i].assoc = assoc;
	table[i].blocks = new block[assoc];

	//Initialize LRU
	table[i].LRU = new LRU_stack(assoc);

	//I believe this is unneccessary. It can be added back in if it becomes necessary
	/*for (int j = 0; j < assoc; j++){
	  table[i].blocks[j].block_size = block_size;
	  }*/
    }
}

void cache::printInfo(){
    cout << "Cache Size: " << cache_size << endl;
    cout << "Block Size: " << block_size << " Associativity: " << assoc << endl;
    cout << "Table Size: " << table_size << endl;
    cout << "Timing== " ;
    cout << "Hit: " << hit_time << "   Miss: " << miss_time << endl;
    cout << "Transfer Time: " << transfer_time << "   Bus Width: " << bus_width << endl;
}


