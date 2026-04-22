#ifndef PARSER_H
#define PARSER_H

#define PRINT_BUFFER_CAP (1 << 8)

#define NODETREE_HEAD \
    VTable *vtable;   \

typedef struct {
    char str[PRINT_BUFFER_CAP];
} PrintBuffer;

typedef struct {
    PrintBuffer (*print)(void *self);
    float (*eval)(void *self, float x);
    void (*free)(void *self);
} VTable;

typedef struct {
    NODETREE_HEAD
} NodeTree;

NodeTree *tree_parse(const char *src);
void tree_print(NodeTree *tree);
float tree_eval(NodeTree *tree, float x);
void tree_free(NodeTree *tree);

#endif
