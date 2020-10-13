//
// Created by Sam Lipschitz on 10/13/20.
//

// Unit tests for hex functions
// These tests should work for both your C implementations and your
// assembly language implementations
//Sam Lipschitz
//slipsch3@jhu.edu
#include <stdio.h>
#include <stdlib.h>
#include "tctestpp.h"
#include "csim_.h"

// test fixture object
typedef struct {
    char test_data_1[16];
} TestObjs;

// setup function (to create the test fixture)
TestObjs *setup(void) {
    TestObjs *objs = malloc(sizeof(TestObjs));
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

    TEST(testgetInput);

    TEST_FINI();

    return 0;
}

void testGetInput(TestObjs *objs) {
    (void) objs; // suppress warning about unused parameter
    char buf[16];
    hex_format_offset(1L, buf);
    //printf("buf: %s\n", buf);
    ASSERT(0 == strcmp(buf, "00000001"));

    hex_format_offset(12345L, buf);
    //printf("buf: %s\n", buf);
    ASSERT(0 == strcmp(buf, "00003039"));

    hex_format_offset(3497L, buf);
    //printf("buf: %s\n", buf);
    ASSERT(0 == strcmp(buf, "00000da9"));

    hex_format_offset(691111L, buf);
    //printf("buf: %s\n", buf);
    ASSERT(0 == strcmp(buf, "000a8ba7"));
}
