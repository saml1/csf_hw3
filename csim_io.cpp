//
// Created by Sam Lipschitz and Tadeusz Sikorski on 10/12/20.
//

#include "csim_io.h"

#include <iostream>
#include <vector>
#include <string>
#include <tuple>


std::vector<std::tuple<char, std::string, int>> get_input() {

    std::vector<std::tuple<char, std::string, int>> output;

    /*  s 0x1fffff50 1
        l 0x1fffff58 1
        l 0x1fffff88 6
        l 0x1fffff90 2
        l 0x1fffff98 2
        l 0x200000e0 2 */

    for (std::string input; std::getline(std::cin, input); ) {
        auto temp = std::make_tuple(input[0], input.substr(2, 12) ,(int) (input[14]));
        output.emplace_back(temp);
    }

    return output;
}


void output(int load_hits, int load_miss, int store_hit, int store_miss, int total) {

    /*
        Total loads: 318197
        Total stores: 197486
        Load hits: 314171
        Load misses: 4026
        Store hits: 188047
        Store misses: 9439
        Total cycles: 9845283

    */
    std::cout << "Total loads: " << load_hits + load_miss << "\n";
    std::cout << "Total stores: " << store_hit + store_miss << "\n";

    std::cout << "Load hits: " << load_hits << "\n";
    std::cout << "Load misses: " << load_miss << "\n";

    std::cout << "Store hits: " << store_hit << "\n";
    std::cout << "Store misses: " << store_miss << "\n";

    std::cout << "Total cycles: " << total << "\n";
}
