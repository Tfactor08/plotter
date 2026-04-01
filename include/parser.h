#ifndef PARSER_H
#define PARSER_H

#define NODETREE_HEAD \
    VTable *vtable;   \

typedef struct {
    void (*print)(void *self, char *buffer);
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
