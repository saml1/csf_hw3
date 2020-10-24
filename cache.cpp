//
// Created by Sam Lipschitz and Tadeusz Sikorski on 10/12/20.
//

#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include <cmath>
#include "cache.h"

/*struct Cache {
    char name[5];
};*/

Cache::Cache(int sets, int blocks, int bytes){
    numSets = sets;
    numBlocks = blocks;
    numBytes = bytes;
    std::vector<std::vector<std::pair<int, std::vector<std::string>>>> allSets(numSets);
    this->sets = allSets;


}

int Cache::getNumSets(){
  return numSets;
}

int Cache::getNumBlocks(){
    return numBlocks;
}
int Cache::getNumBytes(){
    return numBytes;
}

int Cache::getSet(std::string trace){
    int index_bits = log2(numSets);
    int offset_bits = log2(numBlocks);
    int tag_bits = 32 - offset_bits - index_bits;
    std::string indexString = "0x" + trace;
    uint32_t address = stol(indexString, nullptr, 0);
    uint32_t index = address << tag_bits;
    index = index >> (tag_bits + offset_bits);
    return index;

}

int Cache::getTag(std::string trace){
    int index_bits = log2(numSets);
    int offset_bits = log2(numBlocks);
    std::string indexString = "0x" + trace;
    uint32_t address = stol(indexString, nullptr, 0);
    return address >> (index_bits + offset_bits);
}

bool Cache::checkMemoryTrace(std::string trace) { //true if hit
    int traceTag = getTag(trace);
    for(std::pair<int, std::vector<std::string>> block : sets.at(getSet(trace))){
        for(std::string t : block.second){
            if(getTag(t) == traceTag){
                return true;
            }
        }
    }
    return false;
}

bool Cache::cacheFull(int set){ //checking if there's a block in the set that has room
    for(std::pair<int, std::vector<std::string>> block : sets.at(set)){
        if(block.second.size() < numBytes / 4){
            return false; //there's room
        }
    }
    return true;
}


