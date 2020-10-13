//
// Created by Sam Lipschitz on 10/13/20.
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
    std::vector<std::tuple<char, std::string, int>> input = get_input();
    std::cout << "char: " <<  std::get<0>(input);
}
