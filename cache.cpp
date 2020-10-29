/*
 * Cache Class
 * CSF Assignment 3
 * Tadeusz Sikorski and Sam Lipschitz
 * tsikors2@jhu.edu slipsch3@jhu.edu
 */

#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <sstream>
#include "cache.h"


Cache::Cache(int sets, int blocks, int bytes, bool writeAllocate, bool writeThrough, bool lru) {
    numSets = sets;
    numBlocks = blocks;
    numBytes = bytes;
    load_miss = 0;
    load_hit = 0;
    store_miss = 0;
    store_hit = 0;
    cycles = 0;

    // STRUCTURE:
    // list of sets, which is each a list of blocks, each block contains a list of assigned memory locations
    // each block is also marked dirty/clean, which is what the bool is for

    // populates the cache structure
    std::vector<std::vector<std::pair<bool, std::vector<std::string>>>> allSets(numSets);
    for (std::vector<std::pair<bool, std::vector<std::string>>> set : allSets) {
        std::vector<std::pair<bool, std::vector<std::string>>> emptySet(numBlocks);
        set = emptySet;
        for (std::pair<bool, std::vector<std::string>> block : set) {
            std::vector<std::string> temp(numBytes / 4);
            block.second = temp;
        }
    }

    this->sets = allSets;
    this->writeAllocate = writeAllocate;
    this->writeThrough = writeThrough;
    this->lru = lru;

}


/*
 * Prints final data
 *
 * Parameters:
 *
 *
 * Returns:
 *   void
 */
void Cache::printInfo() const {
    std::cout << "Total loads: " << load_hit + load_miss << "\n";
    std::cout << "Total stores: " << store_hit + store_miss << "\n";
    std::cout << "Load hits: " << load_hit << "\n";
    std::cout << "Load misses: " << load_miss << "\n";
    std::cout << "Store hits: " << store_hit << "\n";
    std::cout << "Store misses: " << store_miss << "\n";
    std::cout << "Total cycles: " << cycles << "\n";
}


/*
 * Logic for load hit
 *
 * Parameters:
 *   trace - pointer to current trace being calculated
 *
 * Returns:
 *
 */
void Cache::inc_lh(std::string *trace) {
    if (lru) { //must put recently accessed blocks in back
        updateBlockOrder(trace);
    }
    load_hit++;
    cycles++;
}

/*
 * Logic for load miss
 *
 * Parameters:
 *   trace - pointer to current trace being calculated
 *
 * Returns:
 *
 */
void Cache::inc_lm(std::string *trace) {
    load_miss++;
    if (!cacheFull(trace)) { //if cache isn't full
        addBlock(trace);
    } else { //cache is full
        replace(trace);
    }
    // must go to main memory
    cycles += (100 * (numBytes / 4));
}

/*
 * Logic for store hit
 *
 * Parameters:
 *   trace - pointer to current trace being calculated
 *
 * Returns:
 *
 */
void Cache::inc_sh(std::string *trace) {
    store_hit++;
    if (lru) {
        // must sort on hit
        updateBlockOrder(trace);
    }

    if (this->writeThrough) {
        //writes to cache immediately
        cycles += 100;
    } else {
        // mark as dirty if write-back
        sets.at(getSet(trace)).at(findTag(trace)).first = true;
        cycles++;
    }
}

/*
 * Logic for store miss
 *
 * Parameters:
 *   trace - pointer to current trace being calculated
 *
 * Returns:
 *
 */
void Cache::inc_sm(std::string *trace) {
    store_miss++;
    //if saving stores to cache
    if (this->writeAllocate) {
        // must go to memory
        cycles += (100 * (numBytes / 4));
        if (!cacheFull(trace)) { //if cache isn't full
            addBlock(trace);
        } else { //cache is full
            replace(trace);
        }
    }

    if (!this->writeThrough) {
        //write back only sets block as dirty
        sets.at(getSet(trace)).at(findTag(trace)).first = true;
    } else {
        // writes to cache immediately
        cycles += 100;
    }


}


/*
 * Searches for particular tag in block
 *
 * Parameters:
 *   trace - pointer to current trace being calculated
 *
 * Returns:
 *   index of the found tag
 */
int Cache::findTag(std::string *trace) {
    // get tag in trace
    uint32_t traceTag = getTag(trace);

    std::pair<bool, std::vector<std::string>> temp;
    for (int i = 0; i < (int) sets.at(getSet(trace)).size(); i++) {
        for (std::string t : sets.at(getSet(trace)).at(i).second) {
            // for each object in the block
            if (getTag(&t) == traceTag) {
                return i;
            }
        }
    }
    return 0;
}


/*
 * calculate the set of a trace
 *
 * Parameters:
 *   trace - pointer to current trace being calculated
 *
 * Returns:
 *   value of set bits in trace
 *
 */
uint32_t Cache::getSet(std::string *trace) const {
    unsigned int index_bits = (int) log2(numSets);
    unsigned int offset_bits = (int) log2(numBytes);
    unsigned int tag_bits = 32U - offset_bits - index_bits;

    std::string indexString = "0x" + *trace;
    uint32_t address = stol(indexString, nullptr, 0);

    // 64 bit to account for larger shifts
    uint64_t index = address << tag_bits;
    index = index >> (tag_bits + offset_bits);

    // should auto drop back to 32 bit
    return index;

}

/*
 * calculate the tag of a trace
 *
 * Parameters:
 *   trace - pointer to current trace being calculated
 *
 * Returns:
 *   value of tag bits in trace
 *
 */
uint32_t Cache::getTag(std::string *trace) const {
    uint32_t index_bits = (int) log2(numSets);
    uint32_t offset_bits = (int) log2(numBytes);

    std::string indexString = "0x" + *trace;
    uint32_t address = stol(indexString, nullptr, 0);

    return address >> (index_bits + offset_bits);
}


/*
 * See of memory trace is present in cache
 *
 * Parameters:
 *   trace - pointer to current trace being calculated
 *
 * Returns:
 *   true if present
 *
 */
bool Cache::checkMemoryTrace(std::string trace) { //true if hit
    uint32_t traceTag = getTag(&trace);
    // search set
    for (auto &i : sets.at(getSet(&trace))) {
        // search block
        for (std::string t : i.second) {
            if (getTag(&t) == traceTag) {
                return true;
            }
        }
    }
    return false;
}


/*
 * checking if there's a block in the set that has room
 *
 * Parameters:
 *   trace - pointer to current trace being calculated
 *
 * Returns:
 *   true if cache is full
 *
 */
bool Cache::cacheFull(std::string *trace) {
    if ((int) sets.at(getSet(trace)).size() < numBlocks) {
        //cache is not full
        return false;
    } else {
        return true;
    }
}


/*
 * adding new block list, containing passed in trace
 *
 * Parameters:
 *   trace - pointer to current trace being calculated
 *
 * Returns:
 *
 */
void Cache::addBlock(std::string *trace) {

    auto block = createBlock(trace);
    sets.at(getSet(trace)).push_back(block);

}


/*
 * creates a new block from scratch
 *
 * Parameters:
 *   trace - pointer to current trace being calculated
 *
 * Returns:
 *   new generated block, with pair structure as above
 *
 */
std::pair<bool, std::vector<std::string>> Cache::createBlock(std::string *trace) const {
    std::vector<std::string> traces(numBytes / 4);
    std::string indexString = "0x" + *trace;

    uint32_t address = stol(indexString, nullptr, 0);
    for (int i = 0; i < numBytes / 4; i++) {
        std::stringstream sstream;
        sstream << std::hex << address;
        std::string result = sstream.str();

        traces[i] = result;
        address++;
    }
    return std::make_pair(false, traces);
}


/*
 * sort block for lru
 *
 * Parameters:
 *   trace - pointer to current trace being calculated
 *
 * Returns:
 *
 */
void Cache::updateBlockOrder(std::string *trace) {
    uint32_t traceTag = getTag(trace);
    std::pair<bool, std::vector<std::string>> temp;
    for (int i = 0; i < (int) sets.at(getSet(trace)).size(); i++) {
        for (std::string t : sets.at(getSet(trace)).at(i).second) {
            if (getTag(&t) == traceTag) {
                // found tag
                temp = sets.at(getSet(trace)).at(i);
                sets.at(getSet(trace)).erase(sets.at(getSet(trace)).begin() + i);
                sets.at(getSet(trace)).push_back(temp);
                // send to back and break
                break;
            }
        }
    }
}


/*
 * delete first block, create new block and add to the end
 *
 * Parameters:
 *   trace - pointer to current trace being calculated
 *
 * Returns:
 *
 */
void Cache::replace(std::string *trace) {

    if (sets.at(getSet(trace)).at(findTag(trace)).first) { //it's a dirty block
        cycles += (100 * (numBytes / 4));
    }
    sets.at(getSet(trace)).erase(sets.at(getSet(trace)).begin());
    addBlock(trace);
}