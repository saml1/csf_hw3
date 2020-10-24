//
// Created by Sam Lipschitz and Tadeusz Sikorski on 10/12/20.
//

#include "csim_io.h"
#include "cache.h"
#include <string>
#include <cmath>

int isValid(int val){
    if(val <= 0){
        return 0;
    }
    return (val & (val-1)) == 0;
}

int allValuesValid(int sets, int blocks, int bytes){
    if(!isValid(sets)){
        printf("Invalid set value.\n");
        return 0;
    }
    if(!isValid(blocks) || blocks < 4){
        printf("Invalid block value.\n");
        return 0;
    }

    if(!isValid(bytes)){
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
    if(!allValuesValid(std::stoi(argv[1]), std::stoi(argv[2]), std::stoi(argv[3]))){
        return 1;
    }
    Cache cache(std::stoi(argv[1]), std::stoi(argv[2]), std::stoi(argv[3]), writeAllocate.compare("write-allocate") == 0, writeThrough.compare("write-through") == 0, evictType.compare("lru") == 0);
    std::vector<std::pair<char, std::string>> inputs = get_input();
    for(std::pair<char, std::string> input : inputs){
        if(input.first == 'l'){ //if load
            if(cache.checkMemoryTrace(input.second)){ //if hit
                cache.inc_lh(input.second); //increases load hits, etc
            } else { //if miss
                cache.inc_lm(input.second); //increases load misses, etc
            }
        }else{ //if store
            if(cache.checkMemoryTrace(input.second)){ //if hit
                cache.inc_sh();
                //TODO: everything for store (have it deal with parameters, etc)
            }
        }
    }
    cache.printInfo();
    return 0;
}