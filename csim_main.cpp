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

    int sets = std::stoi(argv[1]);
    int blocks = std::stoi(argv[2]);
    int bytes = std::stoi(argv[3]);
    if(!allValuesValid(sets, blocks, bytes)){
        return 1;
    }
    int load_hit = 0;
    int load_miss = 0;
    int store_hit = 0;
    int store_miss = 0;
    Cache cache(sets, blocks, bytes);
    std::vector<std::pair<char, std::string>> inputs = get_input();
    for(std::pair<char, std::string> input : inputs){
        if(input.first == 'l'){ //if load
            if(cache.checkMemoryTrace(input.second)){ //if hit
                load_hit++;
            } else { //if miss
                load_miss++;

            }
        }else{ //if store

        }
    }
    std::cout << "load hits: " << load_hit << "load miss" << load_miss;
    //std::cout << cache.getTag("1fffff50");
    return 0;
}