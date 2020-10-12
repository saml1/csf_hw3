//
// Created by Sam Lipschitz on 10/12/20.
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
        auto temp = std::make_tuple(input[0], input.substr(2, 12) ,input[-1]);
        output.emplace_back(temp);
    }

    return output;
}
