//
// Created by Sam Lipschitz and Tadeusz Sikorski on 10/12/20.

//

#ifndef CSF_HW3_CSIM_IO_H
#define CSF_HW3_CSIM_IO_H
#include <iostream>
#include <vector>
#include <string>
#include <tuple>


std::vector<std::tuple<char, std::string, int>> get_input();

void output(int load_hits, int load_miss, int store_hit, int store_miss, int total);

#endif //CSF_HW3_CSIM_IO_H
