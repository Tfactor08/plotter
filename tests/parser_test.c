#include "parser.h"

#define TESTS \
    TEST(tree_eval, float, 4, tree_parse("2 + 2"),         0) \
    TEST(tree_eval, float, 0, tree_parse("1-1"),           0) \
    TEST(tree_eval, float, 1, tree_parse("cos(0)"),        0) \
    TEST(tree_eval, float, 8, tree_parse("x^3"),           2) \
    TEST(tree_eval, float, 2, tree_parse("2exp(0)cos(0)"), 0)

#include "test.h"

int main(void)
{
    test();

    return 0;
}
