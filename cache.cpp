//
// Created by Sam Lipschitz and Tadeusz Sikorski on 10/12/20.
//

#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include <cmath>
#include <sstream>
#include "cache.h"

/*struct Cache {
    char name[5];
};*/

Cache::Cache(int sets, int blocks, int bytes){
    numSets = sets;
    numBlocks = blocks;
    numBytes = bytes;
    load_miss = 0;
    load_hit = 0;
    store_miss = 0;
    store_hit = 0;
    cycles = 0;
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
void Cache::printInfo(){
    std:: cout << "Total loads: " << load_hit+load_miss << "\n";
    std:: cout << "Total stores: " << store_hit+store_miss << "\n";
    std:: cout << "Load hits: " << load_hit << "\n";
    std:: cout << "Load misses: " << load_miss << "\n";
    std:: cout << "Store hits: " << store_hit << "\n";
    std:: cout << "Store misses: " << store_miss << "\n";
    std:: cout << "Total cycles: " << cycles << "\n";
}

void Cache::inc_lh(){
    load_hit++;
    cycles++;
}
void Cache::inc_lm(){
    load_miss++;
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

bool Cache::cacheFull(std::string trace){ //checking if there's a block in the set that has room
    if((int)sets.at(getSet(trace)).size() < numBlocks){
        return false; //cache is not full
    }else{
        return true;
    }
}

void Cache::addBlock(std::string trace){
    //sets.at(getSet(trace)).insert(createBlock(trace));
    sets.at(getSet(trace)).push_back(createBlock(trace));
}

std::pair<int, std::vector<std::string>> Cache::createBlock(std:: string trace){
    std::vector<std::string> traces(numBytes / 4);
    std::string indexString = "0x" + trace;
    uint32_t address = stol(indexString, nullptr, 0);
    for(int i = 0; i < numBytes / 4; i++){
        std::stringstream sstream;
        sstream << std::hex << address;
        std::string result = sstream.str();
        traces.assign(i, result);
        address++;
    }
    cycles += 100 * numBytes / 4;
    return std::make_pair(0, traces);
}

/*void Cache::evict(std::string trace, std::string type){

}*/


/*void Cache::updateBlockOrder(std::string trace){

}*/
