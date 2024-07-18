#include <iostream>
#include <iomanip>
#include <cmath>
#include <vector>

using namespace std;

#define DBG 1
#define DRAM_SIZE (64 * 1024 * 1024)
#define CACHE_SIZE (64 * 1024)
int line_size;
int FAcounter = 0;
enum cacheResType
{
	MISS = 0,
	HIT = 1
};
void printBits(unsigned int num, int numBits)
{
	cout << "\n";
	for (int i = numBits - 1; i >= 0; i--)
	{
		cout << ((num >> i) & 1);
	}
	cout << "\n";
}
struct cacheDM
{
};
struct cacheFA_
{
	unsigned int Tag;
	bool V;
};
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
	// This function accepts the memory address for the memory transaction and
	// returns whether it caused a cache miss or a cache hit

	// The current implementation assumes there is no cache; so, every transaction is a miss
	return MISS;
}
vector<cacheFA_> cacheFA;
// Fully Associative Cache Simulator
cacheResType cacheSimFA(unsigned int addr, vector<cacheFA_> &cacheFA)
{
	int numberOflines = CACHE_SIZE / line_size;
	int nbyteselect = log2(line_size);
	unsigned int TagBits = (addr >> nbyteselect);

	for (int i = 0; i < numberOflines; i++)
	{
		if (cacheFA[i].V == 1 && cacheFA[i].Tag == TagBits)
		{
			return HIT;
		}
	}
	if (FAcounter != numberOflines)
	{
		// cout << "HELLO";
		if (cacheFA[FAcounter + 1].V == 0)
		{
			cacheFA[FAcounter++].Tag = TagBits;
			cacheFA[FAcounter].V = 1;
			return MISS;
		}
	}
	// cout << "YAY";
	int randomIndex = rand_() % numberOflines;
	cacheFA[randomIndex].Tag = TagBits;
	cacheFA[randomIndex].V = 1;
	return MISS;
}

char *msg[2] = {"Miss", "Hit"};

// #define NO_OF_Iterations 65500 // CHange to 1,000,000
#define NO_OF_Iterations 1000000 // CHange to 1,000,000

int main()
{
	cout << "Choose the line size of the cache: ";
	cin >> line_size;
	unsigned int hit = 0;
	cacheResType r;

	unsigned int addr;
	int numberOflines = CACHE_SIZE / line_size;
	vector<cacheFA_> cacheFA(numberOflines);

	for (int i = 0; i < numberOflines; ++i)
	{
		cacheFA[i].V = 0;
	}
	cout << " Fully Associative Cache Simulator\n ";

	for (int inst = 0; inst < NO_OF_Iterations; inst++)
	{
		addr = memGen1();
		r = cacheSimFA(addr, cacheFA);
		if (r == HIT)
			hit++;

		cout << "0x" << setfill('0') << setw(8) << hex << addr << " (" << msg[r] << ")\n";
	}

	cout << "Hit ratio = " << fixed << setprecision(6) << (100.0 * hit / NO_OF_Iterations) << "%" << endl;
}