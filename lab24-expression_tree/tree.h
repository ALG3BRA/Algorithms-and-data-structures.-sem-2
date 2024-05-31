#ifndef __TREE__
#define __TREE__

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>


#define VARIABLE_MAX_LENGTH 50

typedef enum {
    NONE,               // 0
    INTEGER,            // 1
    VARIABLE,           // 2
    OPERATION_PLUS,     // 3
    OPERATION_MINUS,    // 4
    OPERATION_MULT,     // 5
    OPERATION_DIVIDE,   // 7
    OPERATION_UN_MINUS, // 6
    BRACKETS_OPEN,      // 7
    BRACKETS_CLOSE      // 8
} TokenType;


typedef struct {
    TokenType type;
    int value; // if type == INTEGER
    char name[VARIABLE_MAX_LENGTH]; // if type == VARIABLE
} Token;


typedef struct TreeNode {
    Token token;
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;


// Function declarations

// Token-related functions
char *tokenGetValue(Token token);
bool tokenIsOperation(Token token);
Token getTokenByOperationType(const char p);
Token createVariableToken(const char *name);
Token createIntegerToken(const char *number);
Token createOperationToken(char op);
bool tokencmp(Token t1, Token t2);
bool isVariable(const char *str);

// Tree-related functions
TreeNode *createTree(Token *token, TreeNode *left, TreeNode *right);
TreeNode *createTreeByExpression(const char *expression);
void treeSimplePrint(TreeNode *node, int shift);
int heightOfExpressionTree(TreeNode *root);

// Tree modification functions
void applyOneTimeDistributive(TreeNode *node);
void applyFullDistributive(TreeNode *root);
void liftOrCollabeMinusesOneTime(TreeNode *node);
void liftOrCollabeMinusesFull(TreeNode *root);
void buildOneTimeBambooPlus(TreeNode *node);
void buildFullBambooPlus(TreeNode *root);
void liftOrCollabeDivideOneTime(TreeNode *node);
void liftOrCollabeDivideFullInMonom(TreeNode *root);
void buildOneTimeBambooMult(TreeNode *node);
void buildFullBambooMult(TreeNode *root);
TreeNode *canonizeTree(TreeNode *root);

// Tree transformation functions
TreeNode *addTwoPluses(TreeNode *old_root);
TreeNode *addMainMinus(TreeNode *old_root);
TreeNode *addDivideInMonom(TreeNode *old_root);

// Monom-related functions
int monomSize(TreeNode *root, int setNum);
Token getTokenFromMonom(TreeNode *node, int setNum, int n);
bool twoMonomBranchesAreEqual(TreeNode *node1, TreeNode *node2, int size, int setNum);
bool monomsAreEqual(TreeNode *node1, TreeNode *node2);


//Iteration and getting monom from tree
int bambooSize(TreeNode *root, int setNum);
TreeNode *getMonomFromBamboo(TreeNode *node, int setNum, int n);
bool twoMomomesInBambooAreEqual(TreeNode *node1, TreeNode *node2, int size, int setNum);
bool mainBamboosAreEqual(TreeNode *node1, TreeNode *node2);


#endif