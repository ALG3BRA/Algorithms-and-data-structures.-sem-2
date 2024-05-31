#include "tree.h"


Node * tree_create() {
    return NULL;
}

Node * node_create(Data d, Node * left, Node * right, Node * parent) {
    Node * t = malloc(sizeof(Node));
    t -> value = d;
    t -> left = left;
    t -> right = right;
    t -> parent = parent;
    return t;
}

Node * tree_find(Node * node, Data d) {
    if (d == node -> value) return node;
    if (d < node -> value) return tree_find(node -> left, d);
    if (d > node -> value) return tree_find(node -> right, d);
    return NULL;
}

Node * tree_find_min_node(Node * node) {
    assert (node != NULL); 
    while(node -> left != NULL) {
        return tree_find_min_node(node -> left);
    }
    return node;
}

Node * tree_find_max_node(Node * node) {
    assert (node != NULL); 
    while(node -> right != NULL) {
        return tree_find_min_node(node -> right);
    }
    return node;
}


Node * tree_node_add(Node * node, Data d, Node * parent) {
    if (node == NULL) {
        return node_create(d, NULL, NULL, parent);
    }
    if (d < node -> value) {
        node -> left = tree_node_add(node -> left, d, node);
    }
    if (d > node -> value) {
        node -> right = tree_node_add(node -> right, d, node);
    }
    return node;
}

Node * tree_add(Node * tree, Data d) {
    if (tree == NULL) {
        tree = node_create(d, NULL, NULL, NULL);
        return tree;
    }
    tree = tree_node_add(tree, d, NULL);
    return tree;
}

bool tree_node_remove(Node * tree, Data d) {
    Node * del_node = tree_find(tree, d);
    if (del_node == NULL) {return false;}

    if ((del_node -> left == NULL) && (del_node -> right == NULL)) {
        if (del_node -> value < ((del_node -> parent) -> value)) {
            (del_node -> parent) -> left = NULL;
        } else {
            (del_node -> parent) -> right = NULL;
        }
        free(del_node);

    } else if (((del_node) -> left != NULL) && ((del_node) -> right == NULL)) {
        (del_node -> left) -> parent = (del_node -> parent);
        if ((del_node -> value) < (del_node -> parent) -> value) {
            (del_node -> parent) -> left = (del_node -> left);
        } else {
            (del_node -> parent) -> right = (del_node -> left);
        }
        free(del_node);

    } else if (((del_node) -> left == NULL) && ((del_node) -> right != NULL)) {
        (del_node -> right) -> parent = (del_node -> parent);
        if ((del_node -> value) < (del_node -> parent) -> value) {
            (del_node -> parent) -> left = (del_node -> right);
        } else {
            (del_node -> parent) -> right = (del_node -> right);
        }
        free(del_node);
    
    } else if (((del_node) -> left != NULL) && ((del_node) -> right != NULL)) {
        Node *min_in_right_tree = tree_find_min_node((del_node) -> right);
        (del_node) -> value = min_in_right_tree -> value;
        tree_node_remove(min_in_right_tree, min_in_right_tree -> value);
    }
    return true;
}
      
int tree_is_empty(Node * tree) {
    return tree == NULL;
}

void tree_node_destroy(Node *node) {
    if (node == NULL) return;
    free(node);
}

void tree_destroy(Node *node) {
    if (node != NULL) {
        tree_destroy(node -> left);
        tree_destroy(node -> right);
    }
    tree_node_destroy(node);
}

void tree_node_simple_print(Node * tree, int shift) {
    if (tree == NULL) {
        printf("\n"); 
        return;
    }
    tree_node_simple_print(tree->left, shift + TREE_PRINT_SHIFT);
    for (int i = 0; i < shift; i++) putchar(' ');
    printf("%.1f", tree->value);
    putchar('\n');
    tree_node_simple_print(tree->right, shift + TREE_PRINT_SHIFT);
}

bool tree_leaves_are_in_range(Node * tree, double x, double y) {
    bool expr1 = x <= (Data) tree_find_min_node(tree) -> value;
    bool expr2 = y >= (Data) tree_find_max_node(tree) -> value;
    return expr1 && expr2;
}

/*
int main() {
    Node * tree = tree_create();
    
    tree = tree_add(tree, 15);
    tree = tree_add(tree, 20);
    tree = tree_add(tree, 9);
    tree = tree_add(tree, 12);
    tree = tree_add(tree, 4);
    tree = tree_add(tree, 7);
    tree = tree_add(tree, 3);
    tree = tree_add(tree, 27);
    tree = tree_add(tree, 8);
    tree_node_simple_print(tree, 0);
    
    tree_node_remove(tree, 7);
    tree = tree_add(tree, 8);
    printf("#################################\n");
    tree_node_simple_print(tree, 0);
    printf("%d\n", tree_leaves_are_in_range(tree, 3.1, 28));

}
*/
