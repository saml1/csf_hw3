//
// Created by Sam Lipschitz and Tadeusz Sikorski on 10/12/20.
//

#include <stdio.h>
#include <stdlib.h>
#include <cassert>
#include <iostream>
#include "tctestpp.h"
#include "cache.h"

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
void testReadStringFromSetStructure(TestObjs *objs);
void testGetSet(TestObjs *objs);
void testGetTag(TestObjs *objs);

int main(int argc, char **argv) {
    if (argc > 1) {
        tctest_testname_to_execute = argv[1];
    }

    TEST_INIT();

    TEST(testGetInput);

    TEST(testReadStringFromSetStructure);


    TEST(testGetSet);

    TEST(testGetTag);

    TEST_FINI();

    return 0;
}

void testReadStringFromSetStructure(TestObjs *objs) {
    std::vector<std::string> a = {"hello"};
    std::pair<int, std::vector<std::string>> b (2, a);
    std::vector<std::pair<int, std::vector<std::string>>> c = {b};
    std::vector<std::vector<std::pair<int, std::vector<std::string>>>> d = {c};
    assert(d.at(0).at(0).second.at(0).compare("hello") == 0);
}

