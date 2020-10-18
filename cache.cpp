//
// Created by Sam Lipschitz and Tadeusz Sikorski on 10/12/20.
//

#include <iostream>
#include <vector>

#include "cache.h"

/*struct Cache {
    char name[5];
};*/

Cache::Cache(int sets, int blocks, int bytes){
    numSets = sets;
    numBlocks = blocks;
    numBytes = bytes;
}


