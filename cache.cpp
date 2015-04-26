#include <iostream>
#include <iomanip>
#include "cache.h"

#define DEBUG_CACHE 0
using namespace std;

cache::cache(unsigned int csize, unsigned int ways, unsigned int bsize, unsigned int htime,
         unsigned int mtime, unsigned int trantime, unsigned int bwidth, cache * lower)
{
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
    // Initialize all sets
    for (unsigned int i = 0; i < table_size; i++)
    {
        table[i].assoc = assoc;
        table[i].blocks = new block[assoc];

        // Initialize LRU stack for each set
        table[i].LRU = new LRU_stack(assoc);
    }
}

unsigned int log2(unsigned int x)
{
    unsigned int log2x = 0;
    unsigned int comp = 0x01;
    while((x & comp) == 0)
    {
        comp = comp << 1;
        log2x++;
    }
    return log2x;
}

unsigned long long int cache::read(unsigned long long int address)
{
    // Going to adjust the tag by shifting out low order bits by
    // block_offset and index_offset.
    unsigned long long int time = 0;
    unsigned long long int tag = address >> block_offset;
    tag = tag >> index_offset;

#if DEBUG_CACHE
    cout << "Handling read for address " << hex << address << " with tag " << tag << dec << endl;
#endif

    unsigned long long index; // effective index used for the request

    // We want the mask to be all 1's for the index
    unsigned long long int mask;
    mask = (unsigned long long int) table_size - 1;

    // Get the index
    index = (unsigned long long)((address >> block_offset) & mask);

    // Increment counter for requests
    requests++;

    // Check if valid at the effective index for any block in the set
    for(unsigned long long b = 0; b < table[index].assoc; ++b)
    {
        if(table[index].blocks[b].valid)
        {
            // Valid, so check the tag
            if(table[index].blocks[b].tag == tag)
            {
                // Tag matches, so we have a hit
                hit_count++;

                // Update the LRU stack
                table[index].LRU->update_stack_on_hit(b);

                // Update the execution time by adding on the hit time
                execution.exec_time += hit_time;
                time += hit_time;

#if DEBUG_CACHE
                if(lower_level != NULL)
                {
                    if(index == 0xFF)
                    {
                        cout << "Contents of L1" << endl;
                        printCache();
                    }
                }
                else
                {
                    if(index == 0x1FF)
                    {
                        cout << "Contents of L2" << endl;
                        printCache();
                    }
                }
#endif

                return time;
            }

        }
    }

    // Miss
    miss_count++;
    execution.exec_time += miss_time;
    time += miss_time;

    // None of the blocks in the set were a match, so determine which block to evict
    unsigned long long way_number = table[index].LRU->update_stack_on_miss();
    if(table[index].blocks[way_number].valid)
    {
        // Block that we are evicting is valid, so record a kickout
        kickouts++;

        // Check if the block that we are evicting is dirty
        if(table[index].blocks[way_number].dirty)
        {
            // We are performing a dirty kickout
            dirty_kickouts++;

            // Write the dirty block to a lower level in the hierarchy
            if(lower_level != NULL)
            {
                // create the effective address of the block that we are kicking out
                unsigned long long eff_address =
                (((table[index].blocks[way_number].tag) << index_offset) | index) << block_offset;
#if DEBUG_CACHE
                cout << "L1 dirty kickout sending address " << hex << eff_address << dec << " to L2 cache" << endl;
#endif

                // write to cache, not main memory, but also need to
                // handle transfer time from L1 to L2
                time += lower_level->write(eff_address);
                execution.exec_time += transfer_time * transfers_per_block;
                time += transfer_time * transfers_per_block;
            }
            else
            {
#if DEBUG_CACHE
                cout << "L2 dirty kickout" << endl;
                cout << "   Sending to memory" << endl;
#endif
                // write to main memory
                execution.exec_time += mem_sendaddr + mem_ready +
                (mem_chunktime * transfers_per_block);
                time += mem_sendaddr + mem_ready +
                (mem_chunktime * transfers_per_block);
            }
        }
    }

    // Generate a read to the next lower level
    if(lower_level != NULL)
    {
        // Read from cache, not main memory, but also need to
        // handle transfer time from L2 to L1
        time += lower_level->read(address);
        execution.exec_time += transfer_time * transfers_per_block;
        time += transfer_time * transfers_per_block;
        transfers++;
    }
    else
    {
        // Read from main memory
        execution.exec_time += mem_sendaddr + mem_ready +
        (mem_chunktime * transfers_per_block);
        time += mem_sendaddr + mem_ready +
        (mem_chunktime * transfers_per_block);
        transfers++;
    }

    // "Replay" the request i.e. add on the hit time
    execution.exec_time += hit_time;
    time += hit_time;

    // Update the actual cache entries
    table[index].blocks[way_number].tag = tag;
    table[index].blocks[way_number].dirty = false;
    table[index].blocks[way_number].valid = true;

#if DEBUG_CACHE
    if(lower_level != NULL)
    {
        if(index == 0xFF)
        {
            cout << "Contents of L1" << endl;
            printCache();
        }
    }
    else
    {
        if(index == 0x1FF)
        {
            cout << "Contents of L2" << endl;
            printCache();
        }
    }
#endif

    return time;
}

unsigned long long int cache::write(unsigned long long int address)
{
    // Going to adjust the tag by shifting out low order bits by
    // block_offset and index_offset.
    unsigned long long int time = 0;
    unsigned long long int tag = address >> block_offset;
    tag = tag >> index_offset;

#if DEBUG_CACHE
    cout << "Handling write for address " << hex << address << " with tag " << tag << dec << endl;
#endif

    unsigned long long index; // effective index used for the request

    // We want the mask to be all 1's for the index
    unsigned long long int mask;
    mask = (unsigned long long int) table_size - 1;

    // Get the index
    index = (unsigned long long)((address >> block_offset) & mask);

    // Increment counter for requests
    requests++;

    // Check if valid at the effective index for any block in the set
    for(unsigned long long b = 0; b < table[index].assoc; ++b)
    {
        if(table[index].blocks[b].valid)
        {
            // Valid, so check the tag
            if(table[index].blocks[b].tag == tag)
            {
                // Tag matches, so we have a hit
                hit_count++;

                // Update the LRU stack
                table[index].LRU->update_stack_on_hit(b);

                // Update the execution time by adding on the hit time
                execution.exec_time += hit_time;
                time += hit_time;

                // Difference between read and write: on a write hit, we need
                // to set the dirty bit
                table[index].blocks[b].dirty = true;

#if DEBUG_CACHE
                if(lower_level != NULL)
                {
                    if(index == 0xFF)
                    {
                        cout << "Contents of L1" << endl;
                        printCache();
                    }
                }
                else
                {
                    if(index == 0x1FF)
                    {
                        cout << "Contents of L2" << endl;
                        printCache();
                    }
                }
#endif
                return time;
            }
        }
    }

    // Miss
    miss_count++;
    execution.exec_time += miss_time;
    time += miss_time;

    // None of the blocks in the set were a match, so determine which block to evict
    unsigned long long way_number = table[index].LRU->update_stack_on_miss();
    if(table[index].blocks[way_number].valid)
    {
        // Block that we are evicting is valid, so record a kickout
        kickouts++;

        // Check if the block that we are evicting is dirty
        if(table[index].blocks[way_number].dirty)
        {
            // We are performing a dirty kickout
            dirty_kickouts++;

            // Write the dirty block to a lower level in the hierarchy
            if(lower_level != NULL)
            {
                // create the effective address of the block that we are kicking out
                unsigned long long eff_address =
                (((table[index].blocks[way_number].tag) << index_offset) | index) << block_offset;
#if DEBUG_CACHE
                cout << "L1 dirty kickout" << endl;
                cout << "   Sending address " << hex << eff_address << dec << " to L2 cache" << endl;
#endif

                // write to cache, not main memory, but also need to
                // handle transfer time from L1 to L2
                time += lower_level->write(eff_address);
                execution.exec_time += transfer_time * transfers_per_block;
                time += transfer_time * transfers_per_block;
            }
            else
            {
#if DEBUG_CACHE
                cout << "L2 dirty kickout" << endl;
                cout << "   Sending to memory" << endl;
#endif
                // write to main memory
                execution.exec_time += mem_sendaddr + mem_ready +
                (mem_chunktime * transfers_per_block);
                time += mem_sendaddr + mem_ready +
                (mem_chunktime * transfers_per_block);
            }
        }
    }

    // Generate a read to the next lower level
    if(lower_level != NULL)
    {
        // Read from cache, not main memory, but also need to
        // handle transfer time from L2 to L1
        time += lower_level->read(address);
        execution.exec_time += transfer_time * transfers_per_block;
        time += transfer_time * transfers_per_block;
        transfers++;
    }
    else
    {
        // Read from main memory
        execution.exec_time += mem_sendaddr + mem_ready +
        (mem_chunktime * transfers_per_block);
        time += mem_sendaddr + mem_ready +
        (mem_chunktime * transfers_per_block);
        transfers++;
    }

    // "Replay" the request i.e. add on the hit time
    execution.exec_time += hit_time;
    time += hit_time;

    // Update the actual cache entries
    table[index].blocks[way_number].tag = tag;
    table[index].blocks[way_number].dirty = true; // write sets the dirty bit
    table[index].blocks[way_number].valid = true;

#if DEBUG_CACHE
    if(lower_level != NULL)
    {
        if(index == 0xFF)
        {
            cout << "Contents of L1" << endl;
            printCache();
        }
    }
    else
    {
        if(index == 0x1FF)
        {
            cout << "Contents of L2" << endl;
            printCache();
        }
    }
#endif

    return time;

}

unsigned long long int cache::flush()
{
    unsigned long long int eff_address;
    unsigned long long int time = 0;
    unsigned long long zero = 0;
    // Loop through the entire table, invalidating all valid bits. When
    // a dirty block is encountered, write to the next level below
    // before invalidating.
    for(unsigned int i = 0; i < table_size; ++i)
    {
        // loop through each block in the set
        for(unsigned int j = 0; j < assoc; ++j)
        {
            // If the block is valid, then we need to check if it is dirty
            if(table[i].blocks[j].valid)
            {
                // If the block is dirty, write it out to the next level
                if(table[i].blocks[j].dirty)
                {
                    // Perform a flush kickout
                    flush_kickouts++;

                    if(lower_level != NULL)
                    {
                        // Reconstruct the effective address of the block
                        eff_address = (((table[i].blocks[j].tag) << index_offset) | (zero + i)) << block_offset;

                        // Write to the next level
                        time += lower_level->write(eff_address);

                        // Add time for transfer from L1 to L2
                        execution.exec_time += transfer_time * transfers_per_block;
                        time += transfer_time * transfers_per_block;
                        transfers++;
                    }
                    else
                    {
                        // Write to main memory
                        execution.exec_time += mem_sendaddr + mem_ready +
                        (mem_chunktime * transfers_per_block);
                        time += mem_sendaddr + mem_ready +
                        (mem_chunktime * transfers_per_block);
                        transfers++;
                    }
                }

                // Regardless of whether the block was dirty, we need to
                // clear the valid bit
                table[i].blocks[j].valid = false;
            }

        }
    }

    return time;
}

void cache::printInfo()
{
    cout << "Cache Size: " << cache_size << endl;
    cout << "Block Size: " << block_size << "   Associativity: " << assoc << endl;
    cout << "Table Size: " << table_size << endl;
    cout << "Hit Time: " << hit_time << "   Miss Time: " << miss_time << endl;
    cout << "Transfer Time: " << transfer_time << "   Bus Width: " << bus_width << endl;
}


void cache::printCounts()
{
    cout << "Hit count: " << hit_count << endl;
    cout << "Miss count: " << miss_count << endl;
    cout << "Requests: " << requests << endl;
    cout << "Kickouts: " << kickouts << endl;
    cout << "Dirty kickouts: " << dirty_kickouts << endl;
    cout << "Transfers: " << transfers << endl;
    cout << "Flush Kickouts: " << flush_kickouts << endl;
}

void cache::printCache()
{
    bool print_index = false;
    for (unsigned int index = 0; index < table_size; index++){
    for (unsigned int b = 0; b < assoc; b++){
        if (table[index].blocks[b].valid == true){
        print_index = true;
        break;
        }
    }
    if(print_index){
        cout << "Index: " << setw(4) << hex << index << " |";
        for (unsigned int b = assoc; b > 0; --b){
        cout << " V: " << dec << table[index].blocks[b-1].valid;
        if (table[index].blocks[b-1].valid){
            cout << " D: " << dec << table[index].blocks[b-1].dirty;
            cout << " Tag: " << setw(12) << hex << table[index].blocks[b-1].tag << " |";
        } else {
            cout << " D: 0 Tag: - |";
        }
        cout << (b+1 < assoc);
        if( b%2 == 1 && b-1 > 0 ){
            cout << endl << "            |";
        }
        }
        cout << endl;
        print_index = false;
    }
    }

}
