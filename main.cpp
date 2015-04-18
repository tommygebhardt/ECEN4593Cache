#include <iostream>
#include <fstream>
#include <cstdio>
#include "cache.h"

#define DEBUG 1
using namespace std;

stats execution; // Stats for execution

// Main memory access times
int mem_sendaddr = 10;
int mem_ready = 30;
int mem_chunktime = 15;
int mem_chunksize = 8;

int main(int argc, char ** argv)
{
    // Cache variables
    unsigned int csize, ways, bsize;
    int htime,  mtime, trantime, bwidth;
    unsigned int csizeL2, waysL2, bsizeL2;
    int htimeL2,  mtimeL2, trantimeL2, bwidthL2;
    // Trace file variables
    char op;
    unsigned long long int address;
    unsigned int bytesize;

    // Take command line input
    if (argc == 2) {
    string line;
    ifstream file(argv[1]);
    size_t position;
    getline(file, line);
    // Go to end of comments
    while(line[0] == '#'){
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

    // Eleventh line is L2 transfer time (put in L1 cache)
    getline(file,line);
    position = line.find(',');
    trantime = stoi(line.substr(position+1));

    // Twelveth line is L2 bus width (put in L1 cache)
    getline(file,line);
    position = line.find(',');
    bwidth = stoi(line.substr(position+1));

    } else if (argc == 1) {
    // Default values
    csize = 8192;
    bsize = 32;
    ways = 1;
    htime = 1;
    mtime = 1;
    trantime = 2; // L1 to L2
    bwidth = 16; // L1 to L2
    csizeL2 = 32768;
    bsizeL2 = 64;
    waysL2 = 1;
    htimeL2 = 5;
    mtimeL2 = 7;
    } else {
    cout << "Incorrect usage: Only zero or one command line argument accepted" << endl;
    return -1;
    }

    trantimeL2 = mem_chunktime; // L2 to main memory 
    bwidthL2 = mem_chunksize; // L2 to main memory

    cache L1D(csize, ways, bsize, htime, mtime, trantime, bwidth);
    cache L1I(csize, ways, bsize, htime, mtime, trantime, bwidth);
    cache L2(csizeL2, waysL2, bsizeL2, htimeL2, mtimeL2, trantimeL2, bwidthL2);

#if (DEBUG == 1)
    cout << "L1 Data Cache" << endl;
    L1D.printInfo();
    cout << "L1 Instruction Cache" << endl;
    L1I.printInfo();
    cout << "L2 Cache" << endl;
    L2.printInfo();
#endif

    // Main loop to read trace data and start tracking statistics
    while(scanf("%c %Lx %d\n", &op, &address, &bytesize) == 3)
    {

    }

    return 0;
}
