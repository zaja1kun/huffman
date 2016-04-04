/**
  @file btree.c
  @brief Simple binary tree implementation

  @author Zaitsev Yury
  @copyright Copyright (c) 2016, Zaitsev Yury
  @license This file is released under the GNU Public License
*/
#include "btree.h"

bt_t *bt_init(void) {
    bt_t *newTree = (bt_t*)s_malloc(sizeof(bt_t));
    newTree->root = NULL;
    newTree->count = 0;
    return newTree;
}

/**
  @brief Destroy subtree nodes

  Frees all the nodes recursivly.
  @param[in] tree btnode_t ** Subtree root
*/
void bt_freeSubtree(btnode_t **subtreeRoot) {
    if(!subtreeRoot || !*subtreeRoot) {
        return;
    }
    bt_freeSubtree(&(*subtreeRoot)->left);
    bt_freeSubtree(&(*subtreeRoot)->right);
    free(*subtreeRoot);
    *subtreeRoot = NULL;
}

void bt_free(bt_t **tree) {
    bt_freeSubtree(&(*tree)->root);
    free(*tree);
    *tree = NULL;
}

int bt_insert(bt_t *tree, btdata_t item) {
    btnode_t *curNode = tree->root;
    btnode_t **newNode = &tree->root;

    while(true) {
        if(!curNode) {
            curNode = *newNode = (btnode_t*)s_malloc(sizeof(btnode_t));
            curNode->left = curNode->right = NULL;
            curNode->data = item;
            tree->count++;
            return 1;
        } else if(item.symb == curNode->data.symb) {
            curNode->data.freq = item.freq;
            return 2;
        } else if(item.freq < curNode->data.freq) {
            newNode = &curNode->right;
            curNode = curNode->right;
        } else {
            newNode = &curNode->left;
            curNode = curNode->left;
        }
    }
}

bt_t *bt_join(bt_t **tree1, bt_t **tree2) {
    bt_t *joinedTree = bt_init();
    joinedTree->count =(*tree1)->count + (*tree2)->count;
    btdata_t rootData = {'\0', (*tree1)->root->data.freq+(*tree2)->root->data.freq};
    bt_insert(joinedTree, rootData);
    if((*tree1)->root->data.freq >= (*tree2)->root->data.freq) {
        joinedTree->root->right = (*tree1)->root;
        joinedTree->root->left = (*tree2)->root;
    } else {
        joinedTree->root->right = (*tree2)->root;
        joinedTree->root->left = (*tree1)->root;
    }
    free(*tree1);
    *tree1 = NULL;
    free(*tree2);
    *tree2 = NULL;
    return joinedTree;
}
