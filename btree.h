/**
  @file btree.h
  @brief Simple binary tree implementation

  @author Zaitsev Yury
  @copyright Copyright (c) 2016, Zaitsev Yury
  @license This file is released under the GNU Public License
*/
#ifndef BINTREE_H
#define BINTREE_H

#include "core.h"

/**
  Standardized name for btNote structure
*/
typedef struct btNode btnode_t;

/**
  Binary tree structure.
  Contains info about tree size and it's root.
*/
typedef struct {
    btnode_t *root;  /**< tree root */
    uint64_t count;  /**< tree node count */
} bt_t;

/**
  Binary tree node data structure.
  Contains symbol data.
*/
typedef struct {
    char symb;      /**< symbol */
    uint64_t freq;  /**< frequency of occurrence of the symbol */
} btdata_t;

/**
  Binary tree node structure.
  Contains node data and pointers to left and right children nodes.
*/
struct btNode {
    btdata_t data;    /**< tree node data */
    btnode_t *left;   /**< pointer to left child */
    btnode_t *right;  /**< pointer to right child */
};


/**
  @brief Create and initialize binary tree structure

  @return Pointer to new initialized tree
*/
bt_t *bt_init(void);

/**
  @brief Destroy tree and all of it's nodes

  Frees all the nodes recursivly.
  Then frees tree structure.
  @param[in] tree bt_t ** Tree to destroy
  @see bt_freeSubtree
*/
void bt_free(bt_t **tree);

/**
  @brief Insert new element to the tree

  Finds place for new element.
  Tree should be a binary search tree for proper work.
  @param[in] tree bt_t * Tree
  @param[in] item btdata_t Item to insert
  @return 1 if new item is unique, 2 otherwise
*/
int bt_insert(bt_t *tree, btdata_t item);

/**
  @brief Unite two trees into new one

  Given trees root nodes become older children of new tree root.
  Given tree structures are freed automatically.
  @param[in] tree1 bt_t ** First tree
  @param[in] tree2 bt_t ** Second tree
  @return Pointer to the new tree
*/
bt_t *bt_join(bt_t **tree1, bt_t **tree2);

#endif /* end of include guard: BINTREE_H */
