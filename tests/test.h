#ifndef TEST_H
#define TEST_H

#include <stdio.h>

// TODO:
// support multiple tests in one file (even possible?);
// work on naming (use conventions).

#define RED   "\033[31m"
#define GREEN "\033[32m"
#define RESET "\033[0m"

static void test_eq()
{
    #ifdef TESTS
    #define TEST(f, T, e, ...)                          \
    {                                                   \
        T res = f(__VA_ARGS__);                         \
        if (res != e) {                                 \
            printf(RED "Test FAILED:\n");               \
        } else {                                        \
            printf(GREEN "Test PASSED:\n");             \
        }                                               \
        char *desc = #f "(" #__VA_ARGS__ ")" " == " #e; \
        printf(RESET "%s\n", desc);                     \
    }

    TESTS

    #undef TEST
    #endif
}

static void test_eq_custom()
{
    #ifdef TESTS
    #define TEST(f, T, e, eq, ...)                      \
    {                                                   \
        T res = f(__VA_ARGS__);                         \
        if (!eq(e, res)) {                              \
            printf(RED "Test FAILED:\n");               \
        } else {                                        \
            printf(GREEN "Test PASSED:\n");             \
        }                                               \
        char *desc = #f "(" #__VA_ARGS__ ")" " == " #e; \
        printf(RESET "%s\n", desc);                     \
    }

    TESTS

    #undef TEST
    #endif
}

#endif // TEST_H
