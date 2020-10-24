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
    std::vector<std::vector<std::pair<int, std::vector<std::string>>>> sets;

public:
    Cache(int sets, int blocks, int bytes);
    int getNumSets();
    int getNumBlocks();
    int getNumBytes();
    void printInfo();
    void inc_lh();
    void inc_lm();
    int getSet(std::string trace);
    int getTag(std::string trace);
    bool checkMemoryTrace(std::string trace);
    bool cacheFull(std::string trace);
    void addBlock(std::string trace);
    std::pair<int, std::vector<std::string>> createBlock(std:: string trace);
};


#endif //CSF_HW3_CACHE_H
