#include <string.h>

#include "../src/lexer.c"

// TODO:
// Despite the fact that this is ugly, it works. But we've got huge problems:
// -- we had to comment out the 'test_eq' funciton in test.h because otherwise it gets generated
// and causes typing errors since we provide an additional 'compare_tks' function in macro calls;
// -- tests printing is not informative since the arguments we pass are not literals;
// -- and who knows what else!

Lexer lexer;
const char *expr = "xsincos * - -.69";
const Token expected_tks[] = {
    { .kind = TK_ID,    .value.c = 'x' },
    { .kind = TK_FUNC,  .value.i = SIN },
    { .kind = TK_FUNC,  .value.i = COS },
    { .kind = TK_MUL,   .value.c = '*' },
    { .kind = TK_MINUS, .value.c = '-' },
    { .kind = TK_DEC,   .value.f = -0.69 },
};

static int compare_tks(Token a, Token b)
{
    if (a.kind != b.kind)
        return 0;

    TOKEN_KIND kind = a.kind;
    if (kind == TK_OPENP || kind == TK_CLOSEP || kind == TK_PLUS || kind == TK_MINUS || kind == TK_MUL || kind == TK_DIV || kind == TK_POW || kind == TK_ID)
        return a.value.c == b.value.c;
    else if (kind == TK_INT || kind == TK_FUNC)
        return a.value.i == b.value.i;
    else if (kind == TK_DEC)
        return a.value.f == b.value.f;
    else if (kind == TK_ID || kind == TK_ERROR)
        return strcmp(a.value.s, b.value.s) == 0;
    else
        return 0;
}

#define TESTS \
    TEST(token_next, Token, expected_tks[0], compare_tks, &lexer) \
    TEST(token_next, Token, expected_tks[1], compare_tks, &lexer) \
    TEST(token_next, Token, expected_tks[2], compare_tks, &lexer) \
    TEST(token_next, Token, expected_tks[3], compare_tks, &lexer) \
    TEST(token_next, Token, expected_tks[4], compare_tks, &lexer) \
    TEST(token_next, Token, expected_tks[5], compare_tks, &lexer)

#include "test.h"

int main(void)
{
    lexer = (Lexer) {
        .content = expr,
        .count = strlen(expr)
    };

    test_eq_custom();

    return 0;
}
