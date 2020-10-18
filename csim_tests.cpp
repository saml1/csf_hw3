//
// Created by Sam Lipschitz and Tadeusz Sikorski on 10/12/20.
//

#include <stdio.h>
#include <stdlib.h>
#include <cassert>
#include<iostream>
#include "tctestpp.h"
#include "csim_io.h"
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
void testIntFields(TestObjs *objs);

int main(int argc, char **argv) {
    if (argc > 1) {
        tctest_testname_to_execute = argv[1];
    }

    TEST_INIT();

    TEST(testGetInput);

    TEST(testReadStringFromSetStructure);

    TEST(testIntFields);

    TEST_FINI();

    return 0;
}

void testGetInput(TestObjs *objs) {
    (void) objs; // suppress warning about unused parameter
    std::cout << "\nEnter the following (then press ctrl-d): s 0x1fffff50 1\n";
    std::vector<std::pair<char, std::string>> input = get_input();
    assert('s' == input[0].first);
    assert(input[0].second.compare("1fffff50") == 0);
}

void testReadStringFromSetStructure(TestObjs *objs) {
    std::vector<std::string> a = {"hello"};
    std::pair<int, std::vector<std::string>> b (2, a);
    std::vector<std::pair<int, std::vector<std::string>>> c = {b};
    std::vector<std::vector<std::pair<int, std::vector<std::string>>>> d = {c};
    assert(d.at(0).at(0).second.at(0).compare("hello") == 0);
}

void testIntFields(TestObjs *objs) {
    Cache:Cache test(1, 2, 3);
    assert(test.getNumSets() == 1);
    assert(test.getNumBlocks() == 2);
    assert(test.getNumBytes() == 3);
}