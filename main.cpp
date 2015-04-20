#include <iostream>
#include <fstream>
#include <cstdio>
#include "cache.h"

#define DEBUG 1
using namespace std;

stats execution; // Global statistics

// Main memory access times
int mem_sendaddr = 10;
int mem_ready = 30;
int mem_chunktime = 15;
int mem_chunksize = 8;

int main(int argc, char ** argv)
{
    // Cache variables
    unsigned int csize, ways, bsize;
    unsigned int htime,  mtime, trantime, bwidth;
    unsigned int csizeL2, waysL2, bsizeL2;
    unsigned int htimeL2,  mtimeL2, trantimeL2, bwidthL2;
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
        trantime = 2; // L1 to L2
        bwidth = 16; // L1 to L2
    } 
    else 
    {
        cout << "Incorrect usage: Only zero or one command line argument accepted" << endl;
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
    
#if (DEBUG == 1)
    cout << "Exiting cache simulation..." << endl;
#endif
    
    return 0;
}
