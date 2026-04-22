#include <string.h>
#include <stdbool.h>

#include "../src/lexer.c"

Lexer lexer;
const char *expr = "xsincos * - -.69";
Token expected_tks[] = {
    { .kind = TK_VAR,   .value.c = 'x',   .print = &token_var_print },
    { .kind = TK_FUNC,  .value.i = SIN,   .print = &token_func_print },
    { .kind = TK_FUNC,  .value.i = COS,   .print = &token_func_print },
    { .kind = TK_MUL,   .value.c = '*',   .print = &token_literal_print },
    { .kind = TK_MINUS, .value.c = '-',   .print = &token_literal_print },
    { .kind = TK_DEC,   .value.f = -0.69, .print = &token_dec_print }
};

#define COMPARE(a, b) compare_tks(&a, &b)
static bool compare_tks(Token *a, Token *b)
{
    if (a->kind != b->kind)
        return 0;
    TOKEN_KIND kind = a->kind;
    if (is_literal(kind))
        return a->value.c == b->value.c;
    else if (kind == TK_INT || kind == TK_FUNC)
        return a->value.i == b->value.i;
    else if (kind == TK_DEC)
        return a->value.f == b->value.f;
    else if (kind == TK_VAR || kind == TK_ERROR)
        return strcmp(a->value.s, b->value.s) == 0;
    else
        return 0;
}

#define PRINT(token) print_tk(&token)
static void print_tk(Token *token)
{
    LexPrintBuffer tk_buf = token->print(token);
    printf("%s", tk_buf.str);
}

#define TESTS \
    TEST(token_next, Token, expected_tks[0], &lexer) \
    TEST(token_next, Token, expected_tks[1], &lexer) \
    TEST(token_next, Token, expected_tks[2], &lexer) \
    TEST(token_next, Token, expected_tks[3], &lexer) \
    TEST(token_next, Token, expected_tks[4], &lexer) \
    TEST(token_next, Token, expected_tks[5], &lexer)

#include "test.h"

int main(void)
{
    lexer = (Lexer) {
        .content = expr,
        .count = strlen(expr)
    };

    test();

    return 0;
}
