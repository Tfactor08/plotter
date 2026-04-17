#ifndef TEST_H
#define TEST_H

#include <stdio.h>

// TODO:
// Support multiple tests in one file (even possible?).

#define RED   "\033[31m"
#define GREEN "\033[32m"
#define RESET "\033[0m"

static void test()
{
#ifdef TESTS
    #if defined(COMPARE) && defined(PRINT)
        #define TEST(f, T, e, ...)                         \
        {                                                  \
            T res = f(__VA_ARGS__);                        \
            if (!COMPARE(e, res)) {                        \
                printf(RED "Test FAILED:\n");              \
            } else {                                       \
                printf(GREEN "Test PASSED:\n");            \
            }                                              \
            char *desc = #f "(" #__VA_ARGS__ ")" " == \""; \
            printf(RESET "%s", desc);                      \
            PRINT(e);                                      \
            printf("\"\n");                                \
        }
    #elif defined(COMPARE)
        #define TEST(f, T, e, ...)                          \
        {                                                   \
            T res = f(__VA_ARGS__);                         \
            if (!COMPARE(e, res)) {                         \
                printf(RED "Test FAILED:\n");               \
            } else {                                        \
                printf(GREEN "Test PASSED:\n");             \
            }                                               \
            char *desc = #f "(" #__VA_ARGS__ ")" " == " #e; \
            printf(RESET "%s\n", desc);                     \
        }
    #elif defined(PRINT)
        #define TEST(f, T, e, ...)                         \
        {                                                  \
            T res = f(__VA_ARGS__);                        \
            if (e != res) {                                \
                printf(RED "Test FAILED:\n");              \
            } else {                                       \
                printf(GREEN "Test PASSED:\n");            \
            }                                              \
            char *desc = #f "(" #__VA_ARGS__ ")" " == \""; \
            printf(RESET "%s", desc);                      \
            PRINT(e);                                      \
            printf("\"\n");                                \
        }
    #else
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
    #endif // defined(COMPARE) && defined(PRINT)

    TESTS

    #undef TEST
#endif // TESTS
}

#endif // TEST_H
