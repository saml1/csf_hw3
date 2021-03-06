/*
 * Main and IO functions
 * CSF Assignment 3
 * Tadeusz Sikorski and Sam Lipschitz
 * tsikors2@jhu.edu slipsch3@jhu.edu
 */


#include <iostream>
#include "cache.h"
#include <string>


/*
 * Tests if input int is valid.
 *
 * Parameters:
 *   val - the input int
 *
 * Returns:
 *   true if valid
 */
bool isValid(unsigned val) {
    if (val <= 0) {
        return false;
    }
    return (val & (val - 1)) == 0;
}


/*
 * Gets list of memory accesses and transforms into workable vector
 *
 * Parameters:
 *
 *
 * Returns:
 *   vector of pairs, where each pair is (load/miss, memory trace)
 */
std::vector<std::pair<char, std::string>> get_input() {

    std::vector<std::pair<char, std::string>> output;

    /*  s 0x1fffff50 1
        l 0x1fffff58 1
        l 0x1fffff88 6
        l 0x1fffff90 2
        l 0x1fffff98 2
        l 0x200000e0 2 */

    for (std::string input; std::getline(std::cin, input);) {
        auto temp = std::make_pair(input[0], input.substr(4, 8));
        output.emplace_back(temp);
    }

    return output;
}

/*
 * Tests if all input ints are valid.
 *
 * Parameters:
 *  sets - the count of sets
 *  blocks - the count of blocks
 *  bytes - the bytes per block
 *
 * Returns:
 *   1 if valid
 */
int allValuesValid(int sets, int blocks, int bytes) {
    if (!isValid(sets)) {
        printf("Invalid set value.\n");
        return 0;
    }
    if (!isValid(blocks)) {
        printf("Invalid block value.\n");
        return 0;
    }

    if (!isValid(bytes) || bytes < 4) {
        printf("Invalid byte value.\n");
        return 0;
    }
    return 1;
}

int main(int argc, char *argv[]) {
    if (argc != 7) {
        printf("Argument count wrong.\n");
        return 1;
    }
    std::string writeAllocate = argv[4];
    std::string writeThrough = argv[5];
    std::string evictType = argv[6];
    if (!allValuesValid(std::stoi(argv[1]), std::stoi(argv[2]), std::stoi(argv[3]))) {
        return 1;
    }

    bool allocate = writeAllocate == "write-allocate";
    bool through = writeThrough == "write-through";

    // Create cache object
    Cache cache(std::stoi(argv[1]), std::stoi(argv[2]),
                std::stoi(argv[3]), allocate, through, evictType == "lru");
    std::vector<std::pair<char, std::string>> inputs = get_input();
    for (auto &input : inputs) {
        if (input.first == 'l') { //if load
            if (cache.checkMemoryTrace(input.second)) { //if hit
                cache.inc_lh(&input.second); //increases load hits, etc
            } else { //if miss
                cache.inc_lm(&input.second); //increases load misses, etc
            }
        } else { //if store
            if (cache.checkMemoryTrace(input.second)) { //if hit
                cache.inc_sh(&input.second);
            } else { //if miss
                cache.inc_sm(&input.second);
            }
        }
    }
    cache.printInfo();
    return 0;
}