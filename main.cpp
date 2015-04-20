#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstdio>
#include "cache.h"

#define DEBUG 1
#define PRINT_FORMATTED_STATS 1
using namespace std;

stats execution; // Global statistics

// Main memory access times
int mem_sendaddr = 10;
int mem_ready = 30;
int mem_chunktime = 15;
int mem_chunksize = 8;

// Cache variables
unsigned int csize, ways, bsize;
unsigned int htime,  mtime, trantime, bwidth;
unsigned int csizeL2, waysL2, bsizeL2;
unsigned int htimeL2,  mtimeL2, trantimeL2, bwidthL2;

int main(int argc, char ** argv)
{
    // Trace file variables
    char op;
    unsigned long long int address;
    unsigned int bytesize;
    
    unsigned int flush_counter = 0; // Flush counter
    unsigned long long int eff_address; // effective address for request
    unsigned int num_requests; // number of requests for a single reference

#if DEBUG
    cout << "argc = " << argc << endl;
    for(int i = 0; i < argc; ++i)
    {
        cout << "argv[" << i << "] = " << argv[i] << endl;
    }
#endif

    // Take command line input
    if (argc == 2) 
    {
        string line;
        ifstream file(argv[1]);
        size_t position;
        getline(file, line);
        // Go to end of comments
        while(line[0] == '#')
        {
            getline(file, line);
        }
        // First line is L1 block size
        position = line.find(',');
        bsize = stoi(line.substr(position+1));

        // Second line is L1 cache size
        getline(file,line);
        position = line.find(',');
        csize = stoi(line.substr(position+1));

        // Third line is L1 associativity
        getline(file,line);
        position = line.find(',');
        ways = stoi(line.substr(position+1));

        // Fourth line is L1 hit time
        getline(file,line);
        position = line.find(',');
        htime = stoi(line.substr(position+1));

        // Fifth line is L1 miss time
        getline(file,line);
        position = line.find(',');
        mtime = stoi(line.substr(position+1));

        // Sixth line is L2 block size
        getline(file,line);
        position = line.find(',');
        bsizeL2 = stoi(line.substr(position+1));

        // Seventh line is L2 cache size
        getline(file,line);
        position = line.find(',');
        csizeL2 = stoi(line.substr(position+1));

        // Eighth line is L2 associativity
        getline(file,line);
        position = line.find(',');
        waysL2 = stoi(line.substr(position+1));

        // Ninth line is L2 hit time
        getline(file,line);
        position = line.find(',');
        htimeL2 = stoi(line.substr(position+1));

        // Tenth line is L2 miss time
        getline(file,line);
        position = line.find(',');
        mtimeL2 = stoi(line.substr(position+1));

        // Eleventh line is L2/L1 transfer time (stored as L1 cache member)
        getline(file,line);
        position = line.find(',');
        trantime = stoi(line.substr(position+1));

        // Twelfth line is L2/L1 bus width (stored as L1 cache member)
        getline(file,line);
        position = line.find(',');
        bwidth = stoi(line.substr(position+1));

    } 
    else if (argc == 1) 
    {
        // Default values
        bsize = 32;
        csize = 8192;
        ways = 1;
        htime = 1;
        mtime = 1;
        bsizeL2 = 64;
        csizeL2 = 32768;
        waysL2 = 1;
        htimeL2 = 5;
        mtimeL2 = 7;
        trantime = 5; // L1 to L2
        bwidth = 16; // L1 to L2
    } 
    else 
    {
        cout << "Incorrect usage: Only zero or one command line arguments accepted" << endl;
        return -1;
    }

    trantimeL2 = mem_chunktime; // L2 to main memory 
    bwidthL2 = mem_chunksize; // L2 to main memory

	// Instantiate caches (starting from lowest level)
    cache L2(csizeL2, waysL2, bsizeL2, htimeL2, mtimeL2, trantimeL2, bwidthL2);
    cache L1D(csize, ways, bsize, htime, mtime, trantime, bwidth, &L2);
    cache L1I(csize, ways, bsize, htime, mtime, trantime, bwidth, &L2);
    

#if (DEBUG == 1)
    cout << "L1 Instruction Cache" << endl;
    L1I.printInfo();
    cout << "L1 Data Cache" << endl;
    L1D.printInfo();
    cout << "L2 Cache" << endl;
    L2.printInfo();
    op = 'R';
    cout << op << endl;
    address = 0x815857077080;
    bytesize = 5;
    L1D.read(address);
    L1D.read(address);
    L1D.read(address + 0x04);
    L1D.read(address + 0x08);
    L1D.read(address + 0x0C);
    L1D.read(address + 0x10);
    L1D.read(address + 0x14);
    L1D.read(address + 0x18);
    L1D.read(address + 0x1C);
    L1D.read(address + 0x20);
    L1D.read(address + 0x24);
    cout << "L1 I:" << endl;
    L1I.printCounts();
    cout << "L1 D:" << endl;
    L1D.printCounts();
    cout << "L2:" << endl;
    L2.printCounts();
#endif
    

    cout << "Starting trace file..." << endl;    
    // Main loop to read trace data and start tracking statistics
    while(scanf("%c %Lx %d%*c", &op, &address, &bytesize) == 3)
    {
#if DEBUG
        cout << "Reading next line from trace file" << endl;
#endif
        execution.total_count++; // Increment total number of references
        
        if(flush_counter == 380000)
        {
            // flush each cache (must be in order)
            L1I.flush();
            L1D.flush();
            L2.flush();
            
            // Keep track of number of flushes
            execution.flushes++;
            
            // Reset the flush counter
            flush_counter = 0;
        }
        
        
        switch(op)
        {
            case 'I':
#if DEBUG
                cout << "Handling instruction reference: " << 
                    op << " " << hex << address << " " << dec << bytesize << endl; 
#endif
                execution.inst_count++;
                // compute the number of effective requests
                num_requests = 1 + (address%4 + bytesize - 1)/4;
                // For each request, generate the "effective address" i.e.
				// the address at the beginning of the L1 cache block, and
				// perform a read.
                for(unsigned int i = 0; i < num_requests; i++)
                {
					eff_address = (address + ((unsigned long long int)i<<2));
					L1I.read(eff_address);
				}
                break;
            case 'R':
#if DEBUG
                cout << "Handling data read reference: " << 
                    op << " " << hex << address << " " << dec << bytesize << endl; 
#endif
                execution.read_count++;
                // compute the number of effective requests
				num_requests = 1 + (address%4 + bytesize - 1)/4;
				// For each request, generate the "effective address" i.e.
				// the address at the beginning of the L1 cache block, and
				// perform a read.
                for(unsigned int i = 0; i < num_requests; i++)
                {
					eff_address = (address + ((unsigned long long int)i<<2));
					L1D.read(eff_address);
				}
                break;
            case 'W':
#if DEBUG
                cout << "Handling data write reference: " << 
                    op << " " << hex << address << " " << dec << bytesize << endl; 
#endif
                // compute the number of effective requests
				num_requests = 1 + (address%4 + bytesize - 1)/4;
				// For each request, generate the "effective address" i.e.
				// the address at the beginning of the L1 cache block, and
				// perform a write.
                for(unsigned int i = 0; i < num_requests; i++)
                {
					eff_address = (address + ((unsigned long long int)i<<2));
					L1D.write(eff_address);
				}
                execution.write_count++;
                
                break;
            default:
                cout << "Undefined op reference type: " << 
                    op << " " << hex << address << " " << dec << bytesize << endl; 
                break;
        }
        
        flush_counter++;
    }
    
#if PRINT_FORMATTED_STATS
    print_all_stats(L1I, L1D, L2);
#endif    
    
#if (DEBUG == 1)
    cout << "Exiting cache simulation..." << endl;
#endif
    
    return 0;
}


// Function to print formatted summary of all statistics
void print_all_stats(cache& L1I, cache& L1D, cache& L2)
{
    // TODO: Acquire the trace file name and process config file to determine these
    string tracefile = "sjeng";
    string cacheconfig = "Default";
    
    // Make all decimals round to the tenths place
    cout << fixed << setprecision(1);
    
    cout << "--------------------------------------------------------------------------------" << endl;
    cout << "      " << tracefile << ", " << cacheconfig << "          " <<
        "Simulation Results" << endl;
    cout << "--------------------------------------------------------------------------------" << endl;
    cout << endl;
    cout << " Memory system:" << endl;
    cout << "   Dcache size = " << csize << " : ways = " << ways << " : block size = " << bsize << endl;
    cout << "   Icache size = " << csize << " : ways = " << ways << " : block size = " << bsize << endl;
    cout << "   L2-cache size = " << csizeL2 << " : ways = " << waysL2 << " : block size = " << bsizeL2 << endl;
    cout << "   Memory ready time = " << mem_ready << " : chunksize = " << mem_chunksize << " : chunktime = " << mem_chunktime << endl;
    cout << endl;
    cout << " Execute time = " << execution.exec_time << ";  Total refs = " << execution.total_count << endl;
    cout << " Flush time = " << execution.flush_time << endl;
    cout << " Inst refs = " << execution.inst_count << ";  " << "Data refs = " << (execution.read_count + execution.write_count) << endl;
    cout << endl;
    cout << " Number of reference types:  [Percentage]" << endl;
    cout << "   Reads  = " << execution.read_count << "     [" << 100.0*(float)(execution.read_count)/(float)(execution.total_count) << "%]" << endl;
    cout << "   Writes = " << execution.write_count << "     [" << 100.0*(float)(execution.write_count)/(float)(execution.total_count) << "%]" << endl;
    cout << "   Inst.  = " << execution.inst_count << "     [" << 100.0*(float)(execution.inst_count)/(float)(execution.total_count) << "%]" << endl;
    cout << "   Total  = " << execution.total_count << endl;
    cout << endl;
    
    // Total cycles information
    
    // Average cycles information
    
    cout << " Memory Level:  L1i" << endl;
    cout << "   Hit Count = " << L1I.hit_count << "  Miss Count = " << L1I.miss_count << endl;
    cout << "   Total Requests = " << L1I.requests << endl;
    cout << "   Hit Rate = " << 100.0*(float)(L1I.hit_count)/(float)(L1I.requests) << "%   Miss Rate = " << 100.0*(float)(L1I.miss_count)/(float)(L1I.requests) << "%" << endl;
    cout << "   Kickouts = " << L1I.kickouts << "; Dirty kickouts = " << L1I.dirty_kickouts << "; Transfers = " << L1I.transfers << endl;
    cout << "   Flush Kickouts = " << L1I.flush_kickouts << endl;
    cout << endl;
    
    cout << " Memory Level:  L1d" << endl;
    cout << "   Hit Count = " << L1D.hit_count << "  Miss Count = " << L1D.miss_count << endl;
    cout << "   Total Requests = " << L1D.requests << endl;
    cout << "   Hit Rate = " << 100.0*(float)(L1D.hit_count)/(float)(L1D.requests) << "%   Miss Rate = " << 100.0*(float)(L1D.miss_count)/(float)(L1D.requests) << endl;
    cout << "   Kickouts = " << L1D.kickouts << "; Dirty kickouts = " << L1D.dirty_kickouts << "; Transfers = " << L1D.transfers << endl;
    cout << "   Flush Kickouts = " << L1D.flush_kickouts << endl;
    cout << endl; 
    
    cout << " Memory Level:  L2" << endl;
    cout << "   Hit Count = " << L2.hit_count << "  Miss Count = " << L2.miss_count << endl;
    cout << "   Total Requests = " << L2.requests << endl;
    cout << "   Hit Rate = " << 100.0*(float)(L2.hit_count)/(float)(L2.requests) << "%   Miss Rate = " << 100.0*(float)(L2.miss_count)/(float)(L2.requests) << endl;
    cout << "   Kickouts = " << L2.kickouts << "; Dirty kickouts = " << L2.dirty_kickouts << "; Transfers = " << L2.transfers << endl;
    cout << "   Flush Kickouts = " << L2.flush_kickouts << endl;
    cout << endl;
    
    // Cache costs
    
    cout << " Flushes = " << execution.flushes << " : Invalidated = " << execution.flushes << endl;
    
    return;
}
