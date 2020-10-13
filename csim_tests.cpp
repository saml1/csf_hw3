//
// Created by Sam Lipschitz and Tadeusz Sikorski on 10/12/20.
//

#include <stdio.h>
#include <stdlib.h>
#include "tctestpp.h"
#include "csim_io.h"

// test fixture object
typedef struct {
    char test_data_1[16];
} TestObjs;

// setup function (to create the test fixture)
TestObjs *setup(void) {
    TestObjs *objs = (TestObjs*) malloc(sizeof(TestObjs));
    strcpy(objs->test_data_1, "Hello, world!\n");
    return objs;
}

// cleanup function (to destroy the test fixture)
void cleanup(TestObjs *objs) {
    free(objs);
}

// Prototypes for test functions
void testGetInput(TestObjs *objs);

int main(int argc, char **argv) {
    if (argc > 1) {
        tctest_testname_to_execute = argv[1];
    }

    TEST_INIT();

    TEST(testGetInput);

    TEST_FINI();

    return 0;
}

void testGetInput(TestObjs *objs) {
    (void) objs; // suppress warning about unused parameter
    std::cout << "\nEnter the following: s 0x1fffff50 1\n";
    std::vector<std::pair<char, std::string>> input = get_input();
    std::cout << "char: " <<  input[0].first << "\n";
    std::cout << "string: " <<  input[0].second << "\n";
    //std::cout << "char: " <<  std::get<0>(input[0]) << "\n";
    //std::cout << "string: " <<  std::get<1>(input[0]) << "\n";
    std::cout << "int: " <<  std::get<2>(input[0]) << "\n";
}
