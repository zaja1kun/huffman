#ifndef BINTREE_H
#define BINTREE_H

#include "core.h"


typedef struct btNode btnode_t;

typedef struct {
    btnode_t *root;
    uint64_t count;
} bt_t;

typedef struct {
    char symb;
    uint64_t freq;
} btdata_t;

struct btNode {
    btdata_t data;
    btnode_t *left;
    btnode_t *right;
};


bt_t *bt_init(void);
bt_t *bt_join(bt_t **tree1, bt_t **tree2);
void bt_free(bt_t **tree);
int bt_insert(bt_t *tree, btdata_t item);
int bt_search(const bt_t * const tree, btdata_t item);
void bt_print(const bt_t * const my_tree);

#endif /* end of include guard: BINTREE_H */
