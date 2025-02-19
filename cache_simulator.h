/**
 * Steve de Sa [101124071] + Samarth Naik [101125670]
 */

#ifndef CACHE_SIMULATOR_H
#define CACHE_SIMULATOR_H

#include <string>
#include <vector>
#include <iostream>
#include <cmath>
#include <sstream>
#include <fstream>
using namespace std;

// Utility Function Declarations
int bin2Dec(string str);
int hextoDec(string str);
string hextoBinary(string str);

class MemoryAddress
{
public:
    string tag;
    string offset;
    string index;

    MemoryAddress(string t = "", string o = "", string i = "");

    void setAddressTag(string addressTag);
    void setAddressOffset(string addressOffset);
    void setAddressIndex(string addressIndex);
    string getAddressTag();
    string getAddressOffset();
    string getAddressIndex();
};

class Block
{
public:
    int valid;
    int dirty;
    string tag;
    int timeStamp;

    Block(int v = 0, int d = 0, string s = "");

    void setValid(int v);
    void setDirty(int d);
    void setTag(string t);
    void setTimeStamp(int time);
    int getValid();
    int getDirty();
    string getTag();
    int getTimeStamp();
};

class Instruction
{
public:
    string type;
    string rawAddress;
    int count;
    vector<MemoryAddress> binaryAddress;

    Instruction(string s);

    string getInstructionType();
    int getInstructionCount();
    MemoryAddress getInstructionAddress();
    string getRawAddress();
    void setInstructionType(char type);
    void setInstructionCount(int c);
    void setInstructionAddress(int tagSize, int setIndexSize);
    void setRawAddress(string a);
    string parseAddress();
};

class Cache
{
public:
    int blockSize;
    int associativity;
    int size;
    int replacement_policy;
    int miss_penalty;
    int write_type;
    int miss;
    int hit;
    int loadmiss;
    int loadhit;
    int storemiss;
    int storehit;
    int load;
    int store;
    int memRefs;
    vector<Block> blocks;

    Cache(int b = 0, int a = 0, int s = 0, int r = 0, int m = 0, int w = 0);

    void increaseHit();
    void increaseLoadHit();
    void increaseStoreHit();
    void increaseStoreMiss();
    void increaseLoadMiss();
    void increaseMiss();
    void increaseMemRefs();
    void increaseStoreCount();
    void increaseLoadCount();
    int getLoadHits();
    int getStoreHits();
    int getStoreMiss();
    int getLoadMiss();
    int getMemRefs();
    int getLoad();
    int getStore();
    int getHits();
    int getMiss();
    int getAssociativity();
    int getBlockOffsetSize();
    int getSetIndexSize();
    int getLineNumbers();
    int getWriteType();
    int getReplacementPolicy();
    int getMissPenalty();
};

#endif