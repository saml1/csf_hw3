//
// Created by Sam Lipschitz and Tadeusz Sikorski on 10/12/20.
//

#ifndef CSF_HW3_CACHE_H
#define CSF_HW3_CACHE_H
#include <vector>
#include <string>
#include <utility>


class Cache{
private:
    int numSets, numBlocks, numBytes, load_hit, load_miss, store_hit, store_miss, cycles;
    std::vector<std::vector<std::pair<bool, std::vector<std::string>>>> sets;
    bool writeAllocate, writeThrough, lru; //true if write-through; false if write-back

public:
    Cache(int sets, int blocks, int bytes, bool writeAllocate, bool writeThrough, bool lru);
    int getNumSets();
    int getNumBlocks();
    int getNumBytes();
    void printInfo();
    void inc_lh(std::string trace);
    void inc_lm(std::string trace);
    void inc_sh();
    void inc_sm();
    int getSet(std::string trace);
    int getTag(std::string trace);
    bool checkMemoryTrace(std::string trace);
    bool cacheFull(std::string trace);
    void addBlock(std::string trace);
    std::pair<bool, std::vector<std::string>> createBlock(std:: string trace);
    void updateBlockOrder(std::string trace);
    void replace(std::string trace);
};


#endif //CSF_HW3_CACHE_H
