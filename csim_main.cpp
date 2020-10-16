//
// Created by Sam Lipschitz and Tadeusz Sikorski on 10/12/20.
//

#include "csim_io.h"
#include <string>
#include <cmath>

struct Cache{
 //TODO: define structure
};

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
    int offset_bits = log2(blocks);
    int index_bits = log2(sets);
    int tag_bits = 32 - offset_bits - index_bits;
    return 0;
}