/**
 * Steve de Sa [101124071] + Samarth Naik [101125670]
 */

#include "cache_simulator.h"

int bin2Dec(string str)
{

    int n = 0;
    int size = str.size() - 1;
    int count = 0;
    for (int i = 0; i <= size; i++)
    {
        if (str[i] == '1')
            n = n + pow(2, size - count);
        count++;
    }
    return n;
}

int hextoDec(string str)
{
    unsigned int result = 0;
    int i = (str[0] == '0' && str[1] == 'x') ? 2 : 0;

    while (str[i] != '\0')
    {
        result *= 16;
        if (str[i] >= '0' && str[i] <= '9')
            result += (str[i] - '0');
        else if (tolower(str[i]) >= 'a' && tolower(str[i]) <= 'f')
            result += (tolower(str[i]) - 'a') + 10;
        i++;
    }
    return result;
}

string hextoBinary(string str)
{
    str = str.substr(2, str.size() - 3);
    int size = 4 * (str.size());
    int num = hextoDec(str);

    string bstring;

    for (int i = size - 1; i >= 0; i--)
    {
        bstring += (num == ((1 << i) | num)) ? '1' : '0';
    }

    return bstring;
}

// MemoryAddress Method Implementations
MemoryAddress::MemoryAddress(string t, string o, string i)
{
    tag = t;
    offset = o;
    index = i;
}

void MemoryAddress::setAddressTag(string addressTag) { tag = addressTag; }
void MemoryAddress::setAddressOffset(string addressOffset) { offset = addressOffset; }
void MemoryAddress::setAddressIndex(string addressIndex) { index = addressIndex; }
string MemoryAddress::getAddressTag() { return tag; }
string MemoryAddress::getAddressOffset() { return offset; }
string MemoryAddress::getAddressIndex() { return index; }

// Block Method Implementations
Block::Block(int v, int d, string s)
{
    valid = v;
    dirty = d;
    tag = s;
    timeStamp = 0;
}

void Block::setValid(int v) { valid = v; }
void Block::setDirty(int d) { dirty = d; }
void Block::setTag(string t) { tag = t; }
void Block::setTimeStamp(int time) { timeStamp = time; }
int Block::getValid() { return valid; }
int Block::getDirty() { return dirty; }
string Block::getTag() { return tag; }
int Block::getTimeStamp() { return timeStamp; }

// Instruction Method Implementations
Instruction::Instruction(string s)
{
    string a[3];
    int k = 0;
    for (int i = 0; i < s.size(); i++)
    {
        a[k] += s[i];
        if (s[i] == ' ')
            k++;
    }
    type = a[0];
    rawAddress = a[1];
    stringstream convert(a[2]);
    convert >> count;
    MemoryAddress m("s", "s", "s");
    binaryAddress.push_back(m);
}

string Instruction::getInstructionType() { return type; }
int Instruction::getInstructionCount() { return count; }
MemoryAddress Instruction::getInstructionAddress() { return binaryAddress[0]; }
string Instruction::getRawAddress() { return rawAddress; }
void Instruction::setInstructionType(char type) { type = type; }
void Instruction::setInstructionCount(int c) { count = c; }
void Instruction::setInstructionAddress(int tagSize, int setIndexSize)
{
    string binaryString = hextoBinary(rawAddress);
    string tag, setIndex, blockOffset;
    for (int i = 0; i < 32; i++)
    {
        if (i < tagSize)
            tag += binaryString[i];
        else if (i < tagSize + setIndexSize)
            setIndex += binaryString[i];
        else
            blockOffset += binaryString[i];
    }
    binaryAddress[0].setAddressTag(tag);
    binaryAddress[0].setAddressOffset(blockOffset);
    binaryAddress[0].setAddressIndex(setIndex);
}
void Instruction::setRawAddress(string a) { rawAddress = a; }
string Instruction::parseAddress()
{
    string binaryString = hextoBinary(rawAddress);
    return binaryString;
}

// Cache Method Implementations
Cache::Cache(int b, int a, int s, int r, int m, int w)
{
    blockSize = b;
    associativity = a;
    size = s;
    replacement_policy = r;
    miss_penalty = m;
    write_type = w;
    miss = 0;
    hit = 0;
    loadmiss = 0;
    loadhit = 0;
    storemiss = 0;
    storehit = 0;
    load = 0;
    store = 0;
    memRefs = 0;
    for (int i = 0; i < getLineNumbers(); i++)
    {
        Block b(0, 0, "");
        blocks.push_back(b);
    }
}

void Cache::increaseHit() { hit++; }
void Cache::increaseLoadHit() { loadhit++; }
void Cache::increaseStoreHit() { storehit++; }
void Cache::increaseStoreMiss() { storemiss++; }
void Cache::increaseLoadMiss() { loadmiss++; }
void Cache::increaseMiss() { miss++; }
void Cache::increaseMemRefs() { memRefs++; }
void Cache::increaseStoreCount() { store++; }
void Cache::increaseLoadCount() { load++; }
int Cache::getLoadHits() { return loadhit; }
int Cache::getStoreHits() { return storehit; }
int Cache::getStoreMiss() { return storemiss; }
int Cache::getLoadMiss() { return loadmiss; }
int Cache::getMemRefs() { return memRefs; }
int Cache::getLoad() { return load; }
int Cache::getStore() { return store; }
int Cache::getHits() { return hit; }
int Cache::getMiss() { return miss; }
int Cache::getAssociativity() { return associativity; }
int Cache::getBlockOffsetSize() { return log2(blockSize); }
int Cache::getSetIndexSize() { return associativity == 0 ? 0 : log2((size * 1024) / (blockSize) / associativity); }
int Cache::getLineNumbers() { return (size * 1024) / (blockSize); }
int Cache::getWriteType() { return write_type; }
int Cache::getReplacementPolicy() { return replacement_policy; }
int Cache::getMissPenalty() { return miss_penalty; }