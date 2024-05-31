#ifndef __TREE__
#define __TREE__

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>


typedef double Data;

typedef struct Node {
    Data value;
    struct Node * left;
    struct Node * right;
    struct Node * parent;
} Node;

typedef struct {
    Node * root;
} Tree;

// Node interface ====

Node *tree_create();
Node *node_create(Data d, Node *left, Node *right, Node *parent);
Node *tree_find(Node *node, Data d);
Node *tree_find_min_node(Node *node);
Node *tree_find_max_node(Node *node);
Node *tree_add(Node *tree, Data d);
bool tree_node_remove(Node *tree, Data d);
int tree_is_empty(Node *tree);
void tree_node_destroy(Node *node);
void tree_destroy(Node *node);
bool tree_leaves_are_in_range(Node *tree, double x, double y);

// \Node interface ===


// Tree Print

#define TREE_PRINT_SHIFT 4
void tree_node_simple_print(Node *tree, int shift);

// 

#endif