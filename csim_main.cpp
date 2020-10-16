//
// Created by Sam Lipschitz and Tadeusz Sikorski on 10/12/20.
//

#include "csim_io.h"
#include <string>

struct Cache{
 //TODO: define structure
};


int main(int argc, char *argv[]) {
    if (argc != 6) {
        printf("Argument count wrong.");
        return 1;
    }
    int sets = *argv[1];
    int blocks = *argv[2];
    int bytes = *argv[3];


    return 0;
}