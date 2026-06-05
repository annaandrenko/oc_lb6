#include <iostream>
#include <iomanip>

using namespace std;

const int BLOCK_SIZE = 64;          // розмір одного блоку кешу
const int ASSOCIATIVITY = 4;        // кількість блоків в одному рядку
const int CACHE_SIZE = 32 * 1024;   // 32 КБ
const int LINE_COUNT = CACHE_SIZE / (BLOCK_SIZE * ASSOCIATIVITY);

struct CacheBlock
{
    bool valid;              // чи зайнятий блок
    unsigned int tag;        // тег блоку пам'яті
    int lastUsed;            // час останнього використання
};

class LRUCache
{
private:
    CacheBlock cache[LINE_COUNT][ASSOCIATIVITY];
    int timeCounter;

public:
    LRUCache()
    {
        timeCounter = 0;

        for (int i = 0; i < LINE_COUNT; i++)
        {
            for (int j = 0; j < ASSOCIATIVITY; j++)
            {
                cache[i][j].valid = false;
                cache[i][j].tag = 0;
                cache[i][j].lastUsed = 0;
            }
        }
    }

    void AccessMemory(unsigned int address)
    {
        timeCounter++;

        unsigned int blockNumber = address / BLOCK_SIZE;
        unsigned int line = blockNumber % LINE_COUNT;
        unsigned int tag = blockNumber / LINE_COUNT;
        unsigned int offset = address % BLOCK_SIZE;

        cout << "\nAddress: " << address << endl;
        cout << "Offset: " << offset << endl;
        cout << "Line: " << line << endl;
        cout << "Tag: " << tag << endl;

        for (int i = 0; i < ASSOCIATIVITY; i++)
        {
            if (cache[line][i].valid && cache[line][i].tag == tag)
            {
                cache[line][i].lastUsed = timeCounter;
                cout << "Result: CACHE HIT" << endl;
                PrintLine(line);
                return;
            }
        }

        cout << "Result: CACHE MISS" << endl;

        int place = -1;

        for (int i = 0; i < ASSOCIATIVITY; i++)
        {
            if (!cache[line][i].valid)
            {
                place = i;
                break;
            }
        }

        if (place == -1)
        {
            int minTime = cache[line][0].lastUsed;
            place = 0;

            for (int i = 1; i < ASSOCIATIVITY; i++)
            {
                if (cache[line][i].lastUsed < minTime)
                {
                    minTime = cache[line][i].lastUsed;
                    place = i;
                }
            }

            cout << "LRU replacement: block " << place << " was replaced" << endl;
        }
        else
        {
            cout << "Loaded into free block: " << place << endl;
        }

        cache[line][place].valid = true;
        cache[line][place].tag = tag;
        cache[line][place].lastUsed = timeCounter;

        PrintLine(line);
    }

    void PrintLine(unsigned int line)
    {
        cout << "Cache line " << line << ": ";

        for (int i = 0; i < ASSOCIATIVITY; i++)
        {
            if (cache[line][i].valid)
            {
                cout << "[B" << i
                    << " tag=" << cache[line][i].tag
                    << " time=" << cache[line][i].lastUsed
                    << "] ";
            }
            else
            {
                cout << "[B" << i << " empty] ";
            }
        }

        cout << endl;
    }
};

int main()
{
    LRUCache cache;

    unsigned int addresses[] = {
        0,
        8192,
        16384,
        24576,
        0,
        32768,
        8192
    };

    int count = sizeof(addresses) / sizeof(addresses[0]);

    for (int i = 0; i < count; i++)
    {
        cache.AccessMemory(addresses[i]);
    }

    return 0;
}