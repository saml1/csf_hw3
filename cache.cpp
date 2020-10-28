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
    for(std::vector<std::pair<bool, std::vector<std::string>>> set : allSets){
        std::vector<std::pair<bool, std::vector<std::string>>> emptySet (numBlocks);
        set = emptySet;
        for(std::pair<bool, std::vector<std::string>> block : set){
            std::vector<std::string> temp (numBytes / 4);
            block.second = temp;
        }
    }

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
        //std::cout << "cache not full\n";
        addBlock(trace);
    }else{ //cache is full
        replace(trace);
    }
}
void Cache::inc_sh(std::string * trace){
    store_hit++;
    if (!this->writeThrough) {
        sets.at(getSet(trace)).at(findTag(trace)).first = true;
    }
    cycles++;
}
void Cache::inc_sm(std::string * trace){
    store_miss++;
    if (this->writeAllocate) {
        if (!cacheFull(trace)) { //if cache isn't full
            addBlock(trace);
        } else { //cache is full
            replace(trace);
        }
    }
    if (!this->writeThrough) {
        sets.at(getSet(trace)).at(findTag(trace)).first = true;
    }

}

int Cache::findTag(std::string * trace) {
    int traceTag = getTag(trace);
    std::pair<bool, std::vector<std::string>> temp;
    //bool found = false;
    for(int i = 0; i < (int) sets.at(getSet(trace)).size(); i++){
        for(std::string t : sets.at(getSet(trace)).at(i).second){
            if(getTag(&t) == traceTag){
                return i;
            }
        }
    }
    return 0;
}

int Cache::getSet(std::string * trace) const{
    int index_bits = (int) log2(numSets);
    int offset_bits = (int) log2(numBytes);
    uint32_t tag_bits = 32 - offset_bits - index_bits;
    std::string indexString = "0x" + *trace;
    uint32_t address = stol(indexString, nullptr, 0);
    uint32_t index = address << tag_bits;
    index = index >> (tag_bits + offset_bits);
    //std::cout << "indexB: " << index_bits << " offset:" << offset_bits << "index:" << index << "\n";

    return index;

}

int Cache::getTag(std::string * trace) const{
    uint32_t index_bits = (int) log2(numSets);
    uint32_t offset_bits = (int) log2(numBytes);
    std::string indexString = "0x" + *trace;
    uint32_t address = stol(indexString, nullptr, 0);
    return address >> (index_bits + offset_bits);
}

bool Cache::checkMemoryTrace(std::string trace) { //true if hit
    int traceTag = getTag(&trace);
    // const std::pair<bool, std::vector<std::string>>& block : sets.at(getSet(&trace))
    for(unsigned long i = 0; i < sets.at(getSet(&trace)).size(); i++){
        for(std::string t : sets.at(getSet(&trace)).at(i).second){
            //std::cout << "trace:" << traceTag << " t:" << getTag(&t) <<"\n";
            if(getTag(&t) == traceTag){
                return true;
            }
        }
    }
    return false;
}

bool Cache::cacheFull(std::string * trace){ //checking if there's a block in the set that has room
    if((int)sets.at(getSet(trace)).size() < numBlocks){
        //std::cout << "b";
        return false; //cache is not full
    }else{
        //std::cout << "a";
        return true;
    }
}

void Cache::addBlock(std::string * trace){
    //sets.at(getSet(trace)).insert(createBlock(trace));
    //std::cout << * trace << " tag:" << getTag(trace) << " set: " << getSet(trace)  << " find : " << findTag(trace)<<"\n";

    auto block = createBlock(trace);
    sets.at(getSet(trace)).push_back(block);
    //std::cout << "block:" << block.second.size() << "\n";

    /*for (int i = 0; i < sets[0].size(); i++) {
        std::cout << "curr:" << sets[0][i].first << "\n";
    }*/
    //std::cout << getSet(trace) << "\n";
    cycles += 100 * numBytes / 4;
}

std::pair<bool, std::vector<std::string>> Cache::createBlock(std:: string * trace) const{
    //std::cout << "Inner Trace:" << trace << "\n";
    std::vector<std::string> traces(numBytes / 4);
    std::string indexString  = "0x" + *trace;
//    if (!(trace->substr(0, 2) == "0x")) {
//        indexString ;
//    }
    //std::cout << "Inner Trace:" << trace << "\n";

    uint32_t address = stol(indexString, nullptr, 0);
    //std::cout << "Address:" << address << "\n";
    for(int i = 0; i < numBytes / 4; i++){
        std::stringstream sstream;
        sstream << std::hex << address;
        std::string result = sstream.str();
        traces[i] = result;
        address++;
    }
    //std::cout << "Inner Trace:" << traces[0] << "\n";
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

void Cache::replace(std::string * trace){ //delete first block, create new block and add to the end
    if(sets.at(getSet(trace)).at(findTag(trace)).first) { //it's a dirty block
        cycles += 100 * numBytes / 4;
    }
    sets.at(getSet(trace)).erase(sets.at(getSet(trace)).begin());
    //sets.at(getSet(trace)).push_back(createBlock(trace));
    addBlock(trace);
    //std::cout << "replace\n";
}