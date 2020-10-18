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
    int numSets, numBlocks, numBytes;
    std::vector<std::vector<std::pair<int, std::vector<std::string>>>> sets;

public:
    Cache(int sets, int blocks, int bytes);
    int getNumSets();
    int getNumBlocks();
    int getNumBytes();
};


#endif //CSF_HW3_CACHE_H
