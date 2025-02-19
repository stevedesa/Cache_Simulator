/**
 * Steve de Sa [101124071] + Samarth Naik [101125670]
 */

#include "cache_simulator.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <stdlib.h>
#include <cmath>
#include <time.h>
#include <iomanip>
using namespace std;

void directMappedCache(Cache &c, MemoryAddress address, string operation)
{
    c.increaseMemRefs();
    int indexToSearch = bin2Dec(address.getAddressIndex()) % c.getLineNumbers();
    string st = "s";
    Block cur_block = c.blocks[indexToSearch];

    if (cur_block.getTag() == address.getAddressTag() && cur_block.getValid() == 1)
    {
        // Hit case
        if (st.compare(operation) < 0)
        {
            c.increaseStoreHit();
            if (c.getWriteType() == 1) // Write Allocate
            {
                cur_block.setTag(address.getAddressTag());
                cur_block.setDirty(1);
            }
        }
        else
        {
            c.increaseLoadHit();
        }
        c.increaseHit();
    }
    else
    {
        // Miss case
        if (st.compare(operation) < 0)
        {
            c.increaseStoreMiss();
            if (c.getWriteType() == 1) // Write Allocate
            {
                cur_block.setTag(address.getAddressTag());
                cur_block.setDirty(1);
                cur_block.setValid(1);
            }
            else // No Write Allocate
            {
                // Do nothing - simulate direct memory write
                return;
            }
        }
        else
        {
            c.increaseLoadMiss();
            cur_block.setTag(address.getAddressTag());
            cur_block.setValid(1);
        }
        c.increaseMiss();
    }
    c.blocks[indexToSearch] = cur_block;
}

void fullyAssociativeCache(Cache &c, MemoryAddress address, string operation)
{
    c.increaseMemRefs();
    int lineNumber = c.getLineNumbers();
    bool hit = false;
    string st = "s";

    Block cur_block(0, 0, " ");
    for (int i = 0; i < lineNumber; i++)
    {
        cur_block = c.blocks[i];
        if (cur_block.getTag() == address.getAddressTag() && cur_block.getValid() == 1)
        {
            c.increaseHit();
            if (st.compare(operation) < 0)
            {
                c.increaseStoreHit();
                if (c.getWriteType() == 1) // Write Allocate
                {
                    cur_block.setTag(address.getAddressTag());
                    cur_block.setDirty(1);
                    cur_block.setValid(1);
                    cur_block.setTimeStamp(c.getMemRefs());
                }
            }
            else
            {
                c.increaseLoadHit();
            }
            c.blocks[i] = cur_block;
            hit = true;
            break;
        }
    }

    if (!hit)
    {
        c.increaseMiss();
        if (st.compare(operation) < 0) // Store operation
        {
            if (c.getWriteType() == 1) // Write Allocate
            {
                // Replacement logic for write allocation
                if (c.getReplacementPolicy() == 0)
                {
                    // Random replacement
                    int randIndex = rand() % lineNumber;
                    cur_block = c.blocks[randIndex];
                    cur_block.setTag(address.getAddressTag());
                    cur_block.setDirty(1);
                    cur_block.setValid(1);
                    cur_block.setTimeStamp(c.getMemRefs());
                    c.blocks[randIndex] = cur_block;
                }
                else
                {
                    // LRU replacement
                    int lruIndex = 0;
                    int minTimestamp = c.blocks[0].getTimeStamp();
                    for (int i = 1; i < lineNumber; i++)
                    {
                        if (c.blocks[i].getTimeStamp() < minTimestamp)
                        {
                            minTimestamp = c.blocks[i].getTimeStamp();
                            lruIndex = i;
                        }
                    }
                    cur_block = c.blocks[lruIndex];
                    cur_block.setTag(address.getAddressTag());
                    cur_block.setDirty(1);
                    cur_block.setValid(1);
                    cur_block.setTimeStamp(c.getMemRefs());
                    c.blocks[lruIndex] = cur_block;
                }
            }
            else // No Write Allocate
            {
                // Do nothing - simulate direct memory write
                return;
            }
        }
        else // Load operation
        {
            // Existing load miss replacement logic
            if (c.getReplacementPolicy() == 0)
            {
                // Random replacement
                int randIndex = rand() % lineNumber;
                cur_block = c.blocks[randIndex];
                cur_block.setTag(address.getAddressTag());
                cur_block.setDirty(0);
                cur_block.setValid(1);
                cur_block.setTimeStamp(c.getMemRefs());
                c.blocks[randIndex] = cur_block;
            }
            else
            {
                // LRU replacement
                int lruIndex = 0;
                int minTimestamp = c.blocks[0].getTimeStamp();
                for (int i = 1; i < lineNumber; i++)
                {
                    if (c.blocks[i].getTimeStamp() < minTimestamp)
                    {
                        minTimestamp = c.blocks[i].getTimeStamp();
                        lruIndex = i;
                    }
                }
                cur_block = c.blocks[lruIndex];
                cur_block.setTag(address.getAddressTag());
                cur_block.setDirty(0);
                cur_block.setValid(1);
                cur_block.setTimeStamp(c.getMemRefs());
                c.blocks[lruIndex] = cur_block;
            }
        }
    }
}

void nWaySetAssociative(Cache &c, MemoryAddress address, string operation, Instruction ins)
{
    c.increaseMemRefs();
    string st = "s";
    bool hit = false;
    int ways = c.getAssociativity();
    int startIndex = ways * bin2Dec(address.getAddressIndex());
    int endIndex = startIndex + ways;

    Block cur_block(0, 0, " ");
    for (int i = startIndex; i < endIndex; i++)
    {
        cur_block = c.blocks[i];
        if (cur_block.getTag() == address.getAddressTag() && cur_block.getValid() == 1)
        {
            c.increaseHit();
            if (st.compare(operation) < 0)
            {
                c.increaseStoreHit();
                if (c.getWriteType() == 1) // Write Allocate
                {
                    cur_block.setDirty(1);
                    cur_block.setValid(1);
                    cur_block.setTimeStamp(c.getMemRefs());
                    c.blocks[i] = cur_block;
                }
            }
            else
            {
                c.increaseLoadHit();
            }
            hit = true;
            break;
        }
    }

    if (!hit)
    {
        c.increaseMiss();
        if (st.compare(operation) < 0) // Store operation
        {
            if (c.getWriteType() == 1) // Write Allocate
            {
                // Replacement logic for write allocation
                if (c.getReplacementPolicy() == 0)
                {
                    // Random replacement
                    int randIndex = startIndex + rand() % ways;
                    cur_block = c.blocks[randIndex];
                    cur_block.setTag(address.getAddressTag());
                    cur_block.setDirty(1);
                    cur_block.setValid(1);
                    cur_block.setTimeStamp(c.getMemRefs());
                    c.blocks[randIndex] = cur_block;
                }
                else
                {
                    // LRU replacement
                    int lruIndex = startIndex;
                    int minTimestamp = c.blocks[startIndex].getTimeStamp();
                    for (int i = startIndex + 1; i < endIndex; i++)
                    {
                        if (c.blocks[i].getTimeStamp() < minTimestamp)
                        {
                            minTimestamp = c.blocks[i].getTimeStamp();
                            lruIndex = i;
                        }
                    }
                    cur_block = c.blocks[lruIndex];
                    cur_block.setTag(address.getAddressTag());
                    cur_block.setDirty(1);
                    cur_block.setValid(1);
                    cur_block.setTimeStamp(c.getMemRefs());
                    c.blocks[lruIndex] = cur_block;
                }
            }
            else // No Write Allocate
            {
                // Do nothing - simulate direct memory write
                return;
            }
        }
        else // Load operation
        {
            // Existing load miss replacement logic
            if (c.getReplacementPolicy() == 0)
            {
                // Random replacement
                int randIndex = startIndex + rand() % ways;
                cur_block = c.blocks[randIndex];
                cur_block.setTag(address.getAddressTag());
                cur_block.setDirty(0);
                cur_block.setValid(1);
                cur_block.setTimeStamp(c.getMemRefs());
                c.blocks[randIndex] = cur_block;
            }
            else
            {
                // LRU replacement
                int lruIndex = startIndex;
                int minTimestamp = c.blocks[startIndex].getTimeStamp();
                for (int i = startIndex + 1; i < endIndex; i++)
                {
                    if (c.blocks[i].getTimeStamp() < minTimestamp)
                    {
                        minTimestamp = c.blocks[i].getTimeStamp();
                        lruIndex = i;
                    }
                }
                cur_block = c.blocks[lruIndex];
                cur_block.setTag(address.getAddressTag());
                cur_block.setDirty(0);
                cur_block.setValid(1);
                cur_block.setTimeStamp(c.getMemRefs());
                c.blocks[lruIndex] = cur_block;
            }
        }
    }
}

void cacheResults(Cache &c, string fileName, int insCount)
{
    int ins = insCount;

    float hitRate = (float)(c.getHits() * 100 / (float)c.getMemRefs());
    float missRate = (float)(c.getMiss() / (float)c.getMemRefs());
    float loadHitRate = (float)c.getLoadHits() * 100 / (float)c.getLoad();
    float storeHitRate = (float)c.getStoreHits() * 100 / (float)c.getStore();
    float memRefs = (float)c.getMemRefs();
    int miss_penalty = c.getMissPenalty();
    int totalRunTime = memRefs + (c.getMiss() * miss_penalty) + insCount;
    float AMAL = (1) + (missRate * miss_penalty);

    cout << fixed << setprecision(4);
    cout << "Total Hits => " << c.getHits() << endl;
    cout << "Total Misses => " << c.getMiss() << endl;
    cout << endl;
    cout << "Total Hit Rate =>" << hitRate << endl;
    cout << "Load Hit Rate =>" << loadHitRate << endl;
    cout << "Store Hit Rate =>" << storeHitRate << endl;
    cout << "Total Run Rime => " << totalRunTime << " cycles" << endl;
    cout << "Average Memory Access Latency => " << AMAL << " cycles" << endl;

    ofstream opfile;
    string output_file = string(fileName) + ".txt";
    opfile.open(output_file.c_str());

    opfile << fixed << setprecision(4);
    opfile << "Total Hit Rate: " << hitRate << "\n";
    opfile << "Load Hit Rate: " << loadHitRate << "\n";
    opfile << "Store Hit Rate: " << storeHitRate << "\n";
    opfile << "Total Run Time: " << totalRunTime << "\n";
    opfile << "Average Memory Access Latency: " << AMAL << "\n";

    opfile.close();
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        cerr << "Usage: " << argv[0] << " <config file> <trace file>" << endl;
        return 1;
    }

    int totalCount = 0;

    string inputfile = argv[1];
    fstream ipfile;
    ipfile.open(inputfile.c_str());

    string cacheConfig;
    int a[6], i = 0;
    while (getline(ipfile, cacheConfig))
    {
        stringstream convert(cacheConfig);
        convert >> a[i];
        i++;
    }

    Cache cache(a[0], a[1], a[2], a[3], a[4], a[5]);
    ipfile.close();

    int mapWays = cache.getAssociativity();
    int setIndexSize = cache.getSetIndexSize(), blockOffsetSize = cache.getBlockOffsetSize();
    int tagSize = 32 - setIndexSize - blockOffsetSize, index;

    string cacheType = mapWays == 0 ? "Fully Associative Cache" : mapWays == 1 ? "Direct Mapped Cache"
                                                                               : "Set Associative Cache";
    string replacementPolicy = cache.getReplacementPolicy() == 0 ? "Random Replacement" : "LRU Replacement";
    string writeAllocation = cache.getWriteType() == 0 ? "No Write Allocate" : "Write Allocate";

    cout << "CACHE SIMULATOR" << endl;
    cout << "Config: " << cacheType << " : " << replacementPolicy << " : " << writeAllocation << endl;
    cout << endl;

    string outputfile = argv[2];
    fstream opfile;
    opfile.open(outputfile.c_str());

    string instr;
    while (getline(opfile, instr))
    {

        Instruction ins(instr);
        totalCount += ins.getInstructionCount();
        ins.setInstructionAddress(tagSize, setIndexSize);

        string st = "s";
        if (st.compare(ins.getInstructionType()) < 0)
        {
            cache.increaseStoreCount();
        }
        else
        {
            cache.increaseLoadCount();
        }
        if (mapWays == 0)
        {
            fullyAssociativeCache(cache, ins.getInstructionAddress(), ins.getInstructionType());
        }
        else if (mapWays == 1)
        {
            directMappedCache(cache, ins.getInstructionAddress(), ins.getInstructionType());
        }
        else
        {
            nWaySetAssociative(cache, ins.getInstructionAddress(), ins.getInstructionType(), ins);
        }
    }

    cacheResults(cache, outputfile, totalCount);
    opfile.close();

    return 0;
}