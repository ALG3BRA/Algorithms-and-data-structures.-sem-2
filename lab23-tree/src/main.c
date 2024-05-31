#include "textmenu.h"
#include "tree.h"

// C & D

typedef struct {
    Node *node;
} Tree;

void *creator_Tree() {
    Tree *tree = malloc(sizeof(Tree));
    tree->node = tree_create();
    return tree;
}

void destroyer_Tree(void *obj) {
    Tree* tree = (Tree*) obj;
    tree_destroy(tree->node);
    free(tree);
}

// Handlers
void handler_simple_print(void *obj) {
    Tree *raw_tree = (Tree*)obj; 
    Node *tree = raw_tree->node;
    //
    tree_node_simple_print(tree, 0);
}

void handler_insert(void *obj) {
    Tree *raw_tree = (Tree*)obj; 
    Node *tree = raw_tree->node;
    //
    double value;
    scanf("%lf", &value);
    raw_tree -> node = tree_add(tree, value);
}

void handler_erase(void *obj) {
    Tree *raw_tree = (Tree*)obj; 
    Node *tree = raw_tree->node;
    //
    double value;
    scanf("%lf", &value);
    printf("Before remove: %f\n", tree -> value);
    if (!(tree_node_remove(tree, value))) {
        printf("This value doesn't exist anyway\n");
    }
}

void handler_func(void *obj) {
    Tree *raw_tree = (Tree*)obj; 
    Node *tree = raw_tree->node;
    //
    double x, y;
    scanf("%lf%lf", &x, &y);
    if (tree_leaves_are_in_range(tree, x, y)) printf("True\n");
    else printf("False\n");
}

int main() {
    textMenuFastInitialize(4,
                           "print", "print - prints tree", handler_simple_print,
                           "add", "add <value> - addes value in tree", handler_insert,
                           "delete", "delete <value> - deletes value in tree", handler_erase,
                           "func", "func - compute special function", handler_func
                           );
    textMenuFastStart(creator_Tree, destroyer_Tree);
    textMenuFastClose();
}