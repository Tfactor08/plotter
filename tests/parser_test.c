#include "parser.h"

#define TESTS \
    TEST(tree_eval, float, 4, tree_parse("2 + 2"),  0) \
    TEST(tree_eval, float, 1, tree_parse("cos(0)"), 0) \
    TEST(tree_eval, float, 8, tree_parse("2^3"),    0)

#include "test.h"

int main(void)
{
    test_eq();

    return 0;
}
