/*
E -> T {+|- T}
T -> P {*|/ P}
P -> F {^ F}
F -> Id | Integer | (E) | -F | Func(E)
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "lexer.c"
#include "utils.c"

#define NODETREE_HEAD \
    float (*eval)(void *self); \
    char  *(*print)(void *self);

typedef struct {
    NODETREE_HEAD
} NodeTree;

typedef struct {
    NODETREE_HEAD
    NodeTree *left;
    NodeTree *right;
} NodeBinary;

typedef struct {
    NODETREE_HEAD
    NodeTree *arg;
} NodeNegate;

typedef struct {
    NODETREE_HEAD
    int val;
} NodeInteger;

#define NODE_BINARY_CREATE_FUNC(name)                                 \
    NodeBinary *node_##name##_create(NodeTree *left, NodeTree *right) \
    {                                                                 \
        float node_##name##_eval(void *);                             \
        char *node_##name##_print(void *);                            \
        NodeBinary *node = malloc(sizeof(NodeBinary));                \
        node->eval = &node_##name##_eval;                             \
        node->print = &node_##name##_print;                           \
        node->left = left;                                            \
        node->right = right;                                          \
        return node;                                                  \
    }                                                                 \

#define NODE_BINARY_EVAL_FUNC(name, operator)  \
    float node_##name##_eval(void *self)       \
    {                                          \
        NodeBinary *node = (NodeBinary *)self; \
        NodeTree *l = node->left;              \
        NodeTree *r = node->right;             \
        return l->eval(l) operator r->eval(r); \
    }                                          \

#define NODE_BINARY_PRINT_FUNC(name, operator)                              \
    char *node_##name##_print(void *self)                                   \
    {                                                                       \
        NodeBinary *node = (NodeBinary *)self;                              \
        char *right_string = node->right->print(node->right);               \
        char *left_string = node->left->print(node->left);                  \
        size_t result_len = strlen(left_string) + strlen(right_string) + 6; \
        char *result = malloc(sizeof(char) * result_len);                   \
        sprintf(result, "(%s #operator %s)", left_string, right_string);    \
        return result;                                                      \
    }                                                                       \

NODE_BINARY_CREATE_FUNC(add);
NODE_BINARY_CREATE_FUNC(sub);
NODE_BINARY_CREATE_FUNC(mult);
NODE_BINARY_CREATE_FUNC(div);

NodeNegate *node_negate_create(NodeTree *arg)
{
    float node_negate_eval(void *);
    char *node_negate_print(void *);

    NodeNegate *node = malloc(sizeof(NodeNegate));
    node->eval = &node_negate_eval;
    node->print = &node_negate_print;
    node->arg = arg;
    return node;
}

NodeInteger *node_integer_create(int val)
{
    float node_integer_eval(void *);
    char *node_integer_print(void *);

    NodeInteger *node = malloc(sizeof(NodeInteger));
    node->eval = &node_integer_eval;
    node->print = &node_integer_print;
    node->val = val;
    return node;
}

NODE_BINARY_EVAL_FUNC(add, +);
NODE_BINARY_EVAL_FUNC(sub, -);
NODE_BINARY_EVAL_FUNC(mult, *);
NODE_BINARY_EVAL_FUNC(div, /);

float node_negate_eval(void *self)
{
    NodeNegate *node = (NodeNegate *)self;
    NodeTree *arg = node->arg;
    return -(arg->eval(arg));
}

float node_integer_eval(void *self)
{
    NodeInteger *node = (NodeInteger *)self;
    return node->val;
}

NODE_BINARY_PRINT_FUNC(add, +);
NODE_BINARY_PRINT_FUNC(sub, -);
NODE_BINARY_PRINT_FUNC(mult, *);
NODE_BINARY_PRINT_FUNC(div, /);

char *node_negate_print(void *self)
{
    NodeNegate *node = (NodeNegate *)self;
    char *arg_string = node->arg->print(node->arg);
    size_t result_len = strlen(arg_string) + 4;
    char *result = malloc(sizeof(char) * result_len);
    sprintf(result, "-(%s)", arg_string);
    return result;
}

char *node_integer_print(void *self)
{
    NodeInteger *node = (NodeInteger *)self;
    char *result = malloc(sizeof(char) * int_len(node->val));
    itoa(node->val, result);
    return result;
}

NodeTree *expression(Lexer *l)
{
    NodeTree *term(Lexer *);

    NodeTree *a = term(l);
    while (true) {
        if (lexer_current(l).kind == TK_PLUS) {
            lexer_next(l);
            NodeTree *b = term(l);
            a = (NodeTree *)node_add_create(a, b);
        } else if (lexer_current(l).kind == TK_MINUS) {
            lexer_next(l);
            NodeTree *b = term(l);
            a = (NodeTree *)node_sub_create(a, b);
        } else {
            return a;
        }
    }
}

NodeTree *term(Lexer *l)
{
    NodeTree *factor(Lexer *);

    NodeTree *a = factor(l);
    while (true) {
        if (lexer_current(l).kind == TK_MULT) {
            lexer_next(l);
            NodeTree *b = factor(l);
            a = (NodeTree *)node_mult_create(a, b);
        } else if (lexer_current(l).kind == TK_DIV) {
            lexer_next(l);
            NodeTree *b = factor(l);
            a = (NodeTree *)node_div_create(a, b);
        } else {
            return a;
        }
    }
}

NodeTree *factor(Lexer *l)
{
    Token tk = lexer_current(l);
    if (tk.kind == TK_INT) {
        lexer_next(l);
        return (NodeTree *)node_integer_create(token_get_int(&tk));
    } else if (tk.kind == TK_MINUS) {
        lexer_next(l);
        return (NodeTree *)node_negate_create(factor(l));
    } else if (tk.kind == TK_OPENP) {
        lexer_next(l);
        NodeTree *a = expression(l);
        if (lexer_current(l).kind == TK_CLOSEP) {
            lexer_next(l);
            return a;
        } else {
            fprintf(stderr, "ERROR (parser): unmatching )\n");
            exit(1);
        }
    } else {
        fprintf(stderr, "ERROR (parser): unknown token\n");
        exit(1);
    }
}

int main(void)
{
    char *source = "4 - (1 / 1)";
    Lexer lexer = lexer_create(source);

    NodeTree *result = expression(&lexer);
    if (lexer_next(&lexer).kind != TK_EOF) {
        fprintf(stderr, "ERROR: smth went wrong\n");
        exit(1);
    }
    printf("%.2f\n", result->eval(result));

    return 0;
}
