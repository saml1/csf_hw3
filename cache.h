//
//Sam Lipschitz and Tadeusz Sikorski
//slipsch3@jhu.edu
//tsikors2@jhu.edu
//

#ifndef CSF_HW3_CACHE_H
#define CSF_HW3_CACHE_H

#include <vector>
#include <string>
#include <utility>


class Cache {
private:
    int numSets, numBlocks, numBytes, load_hit, load_miss, store_hit, store_miss, cycles;
    std::vector<std::vector<std::pair<bool, std::vector<std::string>>>> sets;
    bool writeAllocate, writeThrough, lru; //true if write-through; false if write-back

public:
    Cache(int sets, int blocks, int bytes, bool writeAllocate, bool writeThrough, bool lru);

    void printInfo() const;

    void inc_lh(std::string *trace);

    void inc_lm(std::string *trace);

    void inc_sh(std::string *trace);

    void inc_sm(std::string *trace);

    uint32_t getSet(std::string *trace) const;

    uint32_t getTag(std::string *trace) const;

    bool checkMemoryTrace(std::string trace);

    bool cacheFull(std::string *trace);

    void addBlock(std::string *trace);

    std::pair<bool, std::vector<std::string>> createBlock(std::string *trace) const;

    void updateBlockOrder(std::string *trace);

    void replace(std::string *trace);

    int findTag(std::string *trace);
};


#endif //CSF_HW3_CACHE_H
