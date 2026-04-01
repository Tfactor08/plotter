#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "parser.h"

#define ARRAY_LEN(arr) (sizeof(arr) / sizeof(arr[0]))

#define TEST_ASSSERT(expr, desc)          \
    if (!(expr)) {                        \
        printf("Test FAILED:\n%s", desc); \
        exit(1);                          \
    }

typedef struct {
    char *expression;
    float expected_result;
} TreeTest;

int main(void)
{
    TreeTest tree_tests[] = {
        {"2 + 2", 4}, {"cos(0)", 1}, {"2^10", 1024}
    };

    for (int i = 0; i < ARRAY_LEN(tree_tests); i++) {
        TreeTest test = tree_tests[i];
        NodeTree *tree = tree_parse(test.expression);
        TEST_ASSSERT(tree != NULL, "tree != NULL");
        float result = tree_eval(tree, 0);
        char test_msg[128];
        snprintf(test_msg, sizeof(test_msg), "%s == %.2f\n", test.expression, test.expected_result);
        TEST_ASSSERT(result == test.expected_result, test_msg);
    }

    printf("All tests have passed.\n");

    return 0;
}
