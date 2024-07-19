#include <iostream>
#include <vector>
#include <bitset>
#include <iomanip>
#include <cmath>
using namespace std;

#define DBG 1
#define DRAM_SIZE (64 * 1024 * 1024)
#define CACHE_SIZE (64 * 1024)

enum cacheResType
{
    MISS = 0,
    HIT = 1
};

std::vector<std::bitset<16 * 8 + 1 + 16>> cache16(CACHE_SIZE / 16, 0);
std::vector<std::bitset<32 * 8 + 1 + 16>> cache32(CACHE_SIZE / 32, 0);
std::vector<std::bitset<64 * 8 + 1 + 16>> cache64(CACHE_SIZE / 64, 0);
std::vector<std::bitset<128 * 8 + 1 + 16>> cache128(CACHE_SIZE / 128, 0);

int line_size;
int OffsetBits;
int IndexBits;
int TagBits = 16;

/* The following implements a random number generator */
unsigned int m_w = 0xABABAB55; /* must not be zero, nor 0x464fffff */
unsigned int m_z = 0x05080902; /* must not be zero, nor 0x9068ffff */
unsigned int rand_()
{
    m_z = 36969 * (m_z & 65535) + (m_z >> 16);
    m_w = 18000 * (m_w & 65535) + (m_w >> 16);
    return (m_z << 16) + m_w; /* 32-bit result */
}

unsigned int memGen1()
{
    static unsigned int addr = 0;
    return (addr++) % (DRAM_SIZE);
}

unsigned int memGen2()
{
    static unsigned int addr = 0;
    return rand_() % (24 * 1024);
}

unsigned int memGen3()
{
    return rand_() % (DRAM_SIZE);
}

unsigned int memGen4()
{
    static unsigned int addr = 0;
    return (addr++) % (4 * 1024);
}

unsigned int memGen5()
{
    static unsigned int addr = 0;
    return (addr++) % (1024 * 64);
}

unsigned int memGen6()
{
    static unsigned int addr = 0;
    return (addr += 32) % (64 * 4 * 1024);
}

// Direct Mapped Cache Simulator
cacheResType cacheSimDM(unsigned int addr)
{
    int Tag = addr >> (OffsetBits + IndexBits);
    int Index = (addr >> OffsetBits) & ((1 << IndexBits) - 1);
    switch (line_size)
    {
    case 16:
        if (cache16[Index][16 * 8 + 16] == 1 && ((cache16[Index] >> (16 * 8)).operator&=(0xFFFF)) == Tag)
        {
            return HIT;
        }
        else
        {
            cache16[Index][16 * 8 + 16] = 1;
            for (int i = 0; i < 16; i++)
            {
                cache16[Index][16 * 8 + i] = ((Tag >> i) & 0x1);
            }
            return MISS;
        }
        break;
    case 32:
        if (cache16[Index][32 * 8 + 16] == 1 && ((cache16[Index] >> (32 * 8)).operator&=(0xFFFF)) == Tag)
        {
            return HIT;
        }
        else
        {
            cache32[Index][32 * 8 + 16] = 1;
            for (int i = 0; i < 32; i++)
            {
                cache32[Index][32 * 8 + i] = ((Tag >> i) & 0x1);
            }
            return MISS;
        }
        break;
    case 64:
        if (cache64[Index][64 * 8 + 16] == 1 && ((cache64[Index] >> (64 * 8)).operator&=(0xFFFF)) == Tag)
        {
            return HIT;
        }
        else
        {
            cache64[Index][64 * 8 + 16] = 1;
            for (int i = 0; i < 64; i++)
            {
                cache64[Index][64 * 8 + i] = ((Tag >> i) & 0x1);
            }
            return MISS;
        }
        break;
    case 128:
        if (cache128[Index][128 * 8 + 16] == 1 && ((cache128[Index] >> (128 * 8)).operator&=(0xFFFF)) == Tag)
        {
            return HIT;
        }
        else
        {
            cache128[Index][128 * 8 + 16] = 1;
            for (int i = 0; i < 128; i++)
            {
                cache128[Index][128 * 8 + i] = ((Tag >> i) & 0x1);
            }
            return MISS;
        }
        break;
    }
    // This function accepts the memory address for the memory transaction and
    // returns whether it caused a cache miss or a cache hit

    // The current implementation assumes there is no cache; so, every transaction is a miss
    return MISS;
}

// Fully Associative Cache Simulator
cacheResType cacheSimFA(unsigned int addr)
{
    // This function accepts the memory address for the read and
    // returns whether it caused a cache miss or a cache hit

    // The current implementation assumes there is no cache; so, every transaction is a miss
    return MISS;
}
char *msg[2] = {"Miss", "Hit"};

#define NO_OF_Iterations 500000 // Change to 1,000,000
int main()
{
main:
    cout << "Choose the line size of the cache (16,32,64,128): ";
    cin >> line_size;
    if (line_size != 16 && line_size != 32 && line_size != 64 && line_size != 128)
    {
        goto main;
    }
    OffsetBits = log2(line_size);
    IndexBits = log2(CACHE_SIZE / line_size);

    unsigned int hit = 0;
    cacheResType r;

    unsigned int addr;
    cout << "Direct Mapped Cache Simulator\n";

    for (int inst = 0; inst < NO_OF_Iterations; inst++)
    {
        addr = memGen3();
        r = cacheSimDM(addr);
        if (r == HIT)
            hit++;
        cout << "0x" << setfill('0') << setw(8) << hex << addr << " (" << msg[r] << ")  " << dec << inst + 1 << "\n";
    }
    cout << "Hit ratio = " << fixed << setprecision(6) << ((float)100 * (float)hit / (float)NO_OF_Iterations) << endl;
}
