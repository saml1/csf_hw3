//
// Created by Sam Lipschitz and Tadeusz Sikorski on 10/12/20.
//

#include <iostream>
#include <vector>
#include <string>
#include <utility>

#include "cache.h"

/*struct Cache {
    char name[5];
};*/

Cache::Cache(int sets, int blocks, int bytes){
    numSets = sets;
    numBlocks = blocks;
    numBytes = bytes;
    /*std::vector<std::string> a = {"hello"};
    std::pair<int, std::vector<std::string>> b (2, a);
    std::vector<std::pair<int, std::vector<std::string>>> c = {b};
    std::vector<std::vector<std::pair<int, std::vector<std::string>>>> d = {c};
    this->sets.push_back(c);
    std::cout << this->sets.at(0).at(0).second.at(0);*/

}


