//
// Created by Sam Lipschitz and Tadeusz Sikorski on 10/12/20.
//

#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <sstream>
#include "cache.h"


Cache::Cache(int sets, int blocks, int bytes, bool writeAllocate, bool writeThrough, bool lru){
    numSets = sets;
    numBlocks = blocks;
    numBytes = bytes;
    load_miss = 0;
    load_hit = 0;
    store_miss = 0;
    store_hit = 0;
    cycles = 0;
    std::vector<std::vector<std::pair<bool, std::vector<std::string>>>> allSets(numSets);
    this->sets = allSets;
    this->writeAllocate = writeAllocate;
    this->writeThrough = writeThrough;
    this->lru = lru;

}

void Cache::printInfo() const{
    std:: cout << "Total loads: " << load_hit+load_miss << "\n";
    std:: cout << "Total stores: " << store_hit+store_miss << "\n";
    std:: cout << "Load hits: " << load_hit << "\n";
    std:: cout << "Load misses: " << load_miss << "\n";
    std:: cout << "Store hits: " << store_hit << "\n";
    std:: cout << "Store misses: " << store_miss << "\n";
    std:: cout << "Total cycles: " << cycles << "\n";
}

void Cache::inc_lh(std::string * trace){
    if(lru){ //must put recently accessed blocks in back
        updateBlockOrder(trace);
    }
    load_hit++;
    cycles++;
}
void Cache::inc_lm(std::string * trace){
    load_miss++;
    if(!cacheFull(trace)){ //if cache isn't full
        addBlock(trace);
    }else{ //cache is full
        replace(trace);
    }
}
void Cache::inc_sh(std::string * trace){
    store_hit++;
    if (this->writeAllocate) {
        sets.at(getSet(trace)).at(0).first = true;
    }
    cycles++;
}
void Cache::inc_sm(std::string * trace){
    store_miss++;
    if (this->writeAllocate) {
        sets.at(getSet(trace)).at(0).first = true;
    }

    if(!cacheFull(trace)){ //if cache isn't full
        addBlock(trace);
    }else{ //cache is full
        replace(trace);
    }

}
int Cache::getSet(std::string * trace) const{
    int index_bits = (int) log2(numSets);
    int offset_bits = (int) log2(numBlocks);
    uint32_t tag_bits = 32 - offset_bits - index_bits;
    std::string indexString = "0x" + *trace;
    uint32_t address = stol(indexString, nullptr, 0);
    uint32_t index = address << tag_bits;
    index = index >> (tag_bits + offset_bits);
    return index;

}

int Cache::getTag(std::string * trace) const{
    uint32_t index_bits = log2(numSets);
    uint32_t offset_bits = log2(numBlocks);
    std::string indexString = "0x" + *trace;
    uint32_t address = stol(indexString, nullptr, 0);
    return address >> (index_bits + offset_bits);
}

bool Cache::checkMemoryTrace(std::string trace) { //true if hit
    int traceTag = getTag(&trace);
    for(const std::pair<bool, std::vector<std::string>>& block : sets.at(getSet(&trace))){
        for(std::string t : block.second){
            if(getTag(&t) == traceTag){
                return true;
            }
        }
    }
    return false;
}

bool Cache::cacheFull(std::string * trace){ //checking if there's a block in the set that has room
    if((int)sets.at(getSet(trace)).size() < numBlocks){
        return false; //cache is not full
    }else{
        return true;
    }
}

void Cache::addBlock(std::string * trace){
    //sets.at(getSet(trace)).insert(createBlock(trace));
    sets.at(getSet(trace)).push_back(createBlock(trace));
    cycles += 100 * numBytes / 4;
}

std::pair<bool, std::vector<std::string>> Cache::createBlock(std:: string * trace) const{
    std::vector<std::string> traces(numBytes / 4);
    std::string indexString = "0x" + *trace;
    uint32_t address = stol(indexString, nullptr, 0);
    for(int i = 0; i < numBytes / 4; i++){
        std::stringstream sstream;
        sstream << std::hex << address;
        std::string result = sstream.str();
        traces.assign(i, result);
        address++;
    }
    return std::make_pair(false, traces);
}

void Cache::updateBlockOrder(std::string * trace){
    int traceTag = getTag(trace);
    std::pair<bool, std::vector<std::string>> temp;
    //bool found = false;
    for(int i = 0; i < (int) sets.at(getSet(trace)).size(); i++){
        for(std::string t : sets.at(getSet(trace)).at(i).second){
            if(getTag(&t) == traceTag){
                temp = sets.at(getSet(trace)).at(i);
                sets.at(getSet(trace)).erase(sets.at(getSet(trace)).begin() + i);
                sets.at(getSet(trace)).push_back(temp);
            }
        }
    }
}

void Cache::replace(std::string * trace){
    if(sets.at(getSet(trace)).at(0).first) { //it's a dirty block
        cycles += 100 * numBytes / 4;
    }
    sets.at(getSet(trace)).erase(sets.at(getSet(trace)).begin());
    //sets.at(getSet(trace)).push_back(createBlock(trace));
    addBlock(trace);
}