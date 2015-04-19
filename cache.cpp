#include "cache.h"
#include <iostream>

#define DEBUG_CACHE 1
using namespace std;

cache::cache(unsigned int csize, unsigned int ways, unsigned int bsize, unsigned int htime, 
	unsigned int mtime, unsigned int trantime, unsigned int bwidth, cache * lower){
    cache_size = csize;
    assoc = ways;
    block_size = bsize;
    table_size = (cache_size/block_size)/assoc;
    lower_level = lower; // Pointer to cache at level below current cache, if any
    
    block_offset = log2(block_size);
    index_offset = log2(table_size);

    hit_time = htime;
    miss_time = mtime;
    transfer_time = trantime;
    bus_width = bwidth;
    transfers_per_block = block_size / bus_width;
    
    hit_count = 0;
    miss_count = 0;
    requests = 0;
    kickouts = 0;
    dirty_kickouts = 0;
    transfers = 0;
    flush_kickouts = 0;

    table = new set[table_size];
    //Initialize all sets
    for (unsigned int i = 0; i < table_size; i++){
		table[i].assoc = assoc;
		table[i].blocks = new block[assoc];

		//Initialize LRU
		table[i].LRU = new LRU_stack(assoc);
    }
}

unsigned int cache::log2(unsigned int x){
    unsigned int log2x = 0;
    unsigned int comp = 0x01;
    while((x & comp) == 0){
	comp = comp << 1;
	log2x++;
    }
    return log2x;
}
void cache::read(unsigned long long int address){
    // Going to adjust the tag by shifting out low order bits by
    // block_offset and index_offset.
    unsigned long long int tag = address >> block_offset;
    tag = tag >> index_offset;

    unsigned int index; // effective index used for the request

    // We want the mask to be all 1's for the index
    unsigned long long int mask;
    mask = (unsigned long long int) table_size - 1;

	// Get the index
	index = (unsigned int)((address >> block_offset) & mask);

	// Increment counter for requests
	requests++;

	// Check if valid at the effective index
	if(table[index].blocks[0].valid)
	{
		// Valid, so check the tag
		if(table[index].blocks[0].tag == tag)
		{
			// Tag matches, so we have a hit
			hit_count++;
			
			// Update the execution time by adding on the hit time
			execution.exec_time += hit_time;
			
			return;
		}
		else
		{
			// Tag did not match, so evict the block
			kickouts++;
			
			// Check if the block that we are evicting is dirty
			if(table[index].blocks[0].dirty)
			{
				// We are performing a dirty kickout
				dirty_kickouts++;
				
				// Write the dirty block to a lower level in the hierarchy
				if(lower_level != NULL)
				{
					// write to cache, not main memory, but also need to
					// handle transfer time from L1 to L2
					lower_level->write(address);
					execution.exec_time += transfer_time * transfers_per_block;
				}
				else
				{
					// write to main memory
					execution.exec_time += mem_sendaddr + mem_ready + 
					(mem_chunktime * transfers_per_block);
				}
			}
		}
		
	}
	
	// Miss
	miss_count++;
	execution.exec_time += miss_time;

	// Generate a read to the next lower level
	if(lower_level != NULL)
	{
		// Read from cache, not main memory, but also need to
		// handle transfer time from L1 to L2
		lower_level->read(address);
		execution.exec_time += transfer_time * transfers_per_block;
		transfers++;
	}
	else
	{
		// Read from main memory
		execution.exec_time += mem_sendaddr + mem_ready + 
		(mem_chunktime * transfers_per_block);
		transfers++;
	}
	
	// "Replay" the request i.e. add on the hit time
	execution.exec_time += hit_time;
	
	// Update the actual cache entries
	table[index].blocks[0].tag = tag;
	table[index].blocks[0].dirty = false;
	table[index].blocks[0].valid = true;
		 
    
    return;
}

void cache::write(unsigned long long int address){
	
	cout << address << endl;
	
}

void cache::printInfo(){
    cout << "Cache Size: " << cache_size << endl;
    cout << "Block Size: " << block_size << " Associativity: " << assoc << endl;
    cout << "Table Size: " << table_size << endl;
    cout << "Timing== " ;
    cout << "Hit: " << hit_time << "   Miss: " << miss_time << endl;
    cout << "Transfer Time: " << transfer_time << "   Bus Width: " << bus_width << endl;
}


void cache::printCounts(){
    cout << "Hit count," << hit_count << endl;
    cout << "Miss count," << miss_count << endl;
    cout << "Requests," << requests << endl;
    cout << "Kickouts," << kickouts << endl;
    cout << "Dirty kickouts," << dirty_kickouts << endl;
    cout << "Transfers," << transfers << endl;
    cout << "Flush Kickouts" << flush_kickouts << endl;
}
