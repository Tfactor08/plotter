/*
Id:      {char}+
Integer: {digit}+(.{digit}+)? | .{digit}+
Func:    sin | cos
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

#define MAX_STRING_LEN 256

#define SIN_STR "sin"
#define COS_STR "cos"

typedef enum {
    TK_ID = 1,
    TK_INT,
    TK_DEC,
    TK_OPENP,
    TK_CLOSEP,
    TK_PLUS,
    TK_MINUS,
    TK_MULT,
    TK_DIV,
    TK_POWER,
    TK_FUNC,
    TK_EOF
} TokenKind;

typedef struct Token {
    TokenKind kind;
    union {
        int   i;
        float f;
        char  c;
        char  s[MAX_STRING_LEN+1];
    } value;
    void (*print_self)(struct Token *t);
} Token;

typedef struct {
    char *content;
    size_t count;
    size_t pos;
} Lexer;

static void token_print_int(Token *tk)
{
    printf("int: %d\n", tk->value.i);
}

static void token_print_dec(Token *tk)
{
    printf("dec: %f\n", tk->value.f);
}

static void token_print_literal(Token *tk)
{
    printf("literal: %c\n", tk->value.c);
}

static void token_print_id(Token *tk)
{
    printf("id: %s\n", tk->value.s);
}

static void token_print_func(Token *tk)
{
    printf("func: %s\n", tk->value.s);
}

static Token token_create_int(int val)
{
    Token tk = { .kind = TK_INT, .value.i = val, .print_self = &token_print_int };
    return tk;
}

static Token token_create_dec(float val)
{
    Token tk = { .kind = TK_DEC, .value.f = val, .print_self = &token_print_dec };
    return tk;
}

static Token token_create_literal(TokenKind kind, char val)
{
    assert(kind == TK_OPENP || kind == TK_CLOSEP || kind == TK_PLUS || kind == TK_MINUS || kind == TK_MULT || kind == TK_DIV || kind == TK_POWER);
    Token tk = { .kind = kind, .value.c = val, .print_self = &token_print_literal };
    return tk;
}

static Token token_create_string(TokenKind kind, const char *val)
{
    assert(kind == TK_ID || kind == TK_FUNC);
    assert(strlen(val) <= MAX_STRING_LEN);
    Token tk = { .kind = kind };
    strcpy(tk.value.s, val);
    if (kind == TK_ID) tk.print_self = &token_print_id;
    else if (kind == TK_FUNC) tk.print_self = &token_print_func;
    return tk;
}

static Token token_create_eof()
{
    Token tk = { .kind = TK_EOF };
    return tk;
}

Token next(Lexer *l)
{
    while (l->pos < l->count && isspace(l->content[l->pos]))
        l->pos += 1;
    if (l->pos >= l->count)
        return token_create_eof();
    char c = l->content[l->pos];
    char next = '\0';
    if (l->pos+1 < l->count)
        next = l->content[l->pos + 1];
    if (isdigit(c) || (c == '-' && (isdigit(next) || next == '.')) || (c == '.' && isdigit(next))) {
        size_t start = l->pos;
        int is_float = 0;
        if (c == '-')
            l->pos += 1;
        while (l->pos < l->count && isdigit(l->content[l->pos]))
            l->pos += 1;
        if (l->pos < l->count && l->content[l->pos] == '.') {
            l->pos += 1;
            is_float = 1;
            while (l->pos < l->count && isdigit(l->content[l->pos]))
                l->pos += 1;
        }
        size_t num_len = l->pos - start;
        if (num_len > MAX_STRING_LEN) {
            fprintf(stderr, "ERROR: max number length is %d\n", MAX_STRING_LEN);
            exit(1);
        }
        char *num_start = l->content + start;
        char num_str[MAX_STRING_LEN+1];
        memcpy(num_str, num_start, num_len);
        num_str[num_len] = '\0';
        if (!is_float) {
            int num = atoi(num_str);
            return token_create_int(num);
        } else {
            float num = atof(num_str);
            return token_create_dec(num);
        }
    } else if (isalpha(l->content[l->pos])) {
        size_t start = l->pos;
        while (l->pos < l->count && isalpha(l->content[l->pos]))
            l->pos += 1;
        size_t string_len = l->pos - start;
        if (string_len > MAX_STRING_LEN) {
            fprintf(stderr, "ERROR: max id length is %d\n", MAX_STRING_LEN);
            exit(1);
        }
        char *string_start = l->content + start;
        char string[MAX_STRING_LEN];
        memcpy(string, string_start, string_len);
        string[string_len] = '\0';
        if (strcmp(string, SIN_STR) == 0 || strcmp(string, COS_STR) == 0)
            return token_create_string(TK_FUNC, string);
        else
            return token_create_string(TK_ID, string);
    } else if (c == '+') {
        l->pos += 1;
        return token_create_literal(TK_PLUS, c);
    } else if (c == '-') {
        l->pos += 1;
        return token_create_literal(TK_MINUS, c);
    } else if (c == '*') {
        l->pos += 1;
        return token_create_literal(TK_MULT, c);
    } else if (c == '/') {
        l->pos += 1;
        return token_create_literal(TK_DIV, c);
    } else if (c == '^') {
        l->pos += 1;
        return token_create_literal(TK_POWER, c);
    } else if (c == '(') {
        l->pos += 1;
        return token_create_literal(TK_OPENP, c);
    } else if (c == ')') {
        l->pos += 1;
        return token_create_literal(TK_CLOSEP, c);
    } else {
        fprintf(stderr, "ERROR: unknown symbol at %zu\n", l->pos);
        exit(1);
    }
}

int main(void)
{
    char *source = "sin * cos x - -.69 + duck / 2^duck";
    Lexer l = {
        .content = source,
        .count = strlen(source),
        .pos = 0
    };
    Token tk = {0};

    printf("%s\n", source);
    while ((tk = next(&l)).kind != TK_EOF) {
        tk.print_self(&tk);
    }
}
