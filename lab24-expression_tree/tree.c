#include "tree.h"
#include "tree_stack.h"
#include "stack.h"


char *tokenGetValue(Token token) {
    char *str = malloc(sizeof(char) * (VARIABLE_MAX_LENGTH + 1));
    switch (token.type) {
        case OPERATION_PLUS:
            str[0] = '+';
            str[1] = '\0';
            break;
        case OPERATION_MINUS:
            str[0] = '-';
            str[1] = '\0';
            break;
        case OPERATION_MULT:
            str[0] = '*';
            str[1] = '\0';
            break;
        case OPERATION_DIVIDE:
            str[0] = '/';
            str[1] = '\0';
            break;
        case VARIABLE:
            strcpy(str, token.name);
            break;
        case INTEGER:
            sprintf(str, "%d", token.value);
            break;
    default:
        break;
    }
    return str;
}

void treeSimplePrint(TreeNode *node, int shift) {
    if (node == NULL) {
        return;
    }
    treeSimplePrint(node->right, shift + 3);
    for (int i = 0; i < shift; i++) {
        printf(" ");
    }
    printf("%s\n ", tokenGetValue(node->token));
    
    treeSimplePrint(node->left, shift + 3);
}

bool isVariable(const char* str) {
    if (str == NULL || *str == '\0' || isdigit(*str)) {
        return false;
    }
    const char* p = str;
    while (*p != '\0') {
        if (!isalnum(*p) && *p != '_') {
            return false;
        }
        p++;
    }
    return true;
}


TreeNode *createTree(Token *token, TreeNode *left, TreeNode *right) {
    TreeNode *node = malloc(sizeof(TreeNode));
    node->left = left;
    node->right = right;

    if (token == NULL) {
        Token new_token;
        new_token.type = NONE;
        node->token = new_token;
    } else {
        node->token = *token;
    }
    return node;

}

Token getTokenByOperationType(const char p) {
    Token token;
    switch (p) {
        case '+':
            token.type = OPERATION_PLUS;
            break;
        case '-':
            token.type = OPERATION_MINUS;
            break;
        case '*':
            token.type = OPERATION_MULT;
            break;
        case '/':
            token.type = OPERATION_DIVIDE;
            break;
        case '(':
            token.type = BRACKETS_OPEN;
            break;
        case ')':
            token.type = BRACKETS_CLOSE;
            break;
    }
    return token;
}

Token createVariableToken(const char *name) {
    Token token;
    token.type = VARIABLE;
    strncpy(token.name, name, VARIABLE_MAX_LENGTH - 1);
    token.name[VARIABLE_MAX_LENGTH - 1] = '\0';
    return token;
}

Token createIntegerToken(const char *number) {
    Token token;
    token.type = INTEGER;
    token.value = atoi(number);
    return token;
}

Token createOperationToken(char op) {
    Token token;
    switch (op) {
        case '+':
            token.type = OPERATION_PLUS;
            break;
        case '-':
            token.type = OPERATION_MINUS;
            break;
        case '*':
            token.type = OPERATION_MULT;
            break;
        case '/':
            token.type = OPERATION_DIVIDE;
            break;
    }
    return token;
}


bool tokenIsOperation(Token token) {
    return (token.type == OPERATION_PLUS || token.type == OPERATION_MINUS || token.type == OPERATION_MULT || token.type == OPERATION_DIVIDE);
}


TreeNode *createNodesInTNStack(TNStack *tn_stack) {
    TreeNode *node_op = malloc(sizeof(TreeNode));
    node_op->token = treeNodeStackPop(tn_stack)->token;
    node_op->right = treeNodeStackPop(tn_stack);
    node_op->left = treeNodeStackPop(tn_stack);
    treeNodeStackPush(tn_stack, node_op);
    return node_op;
}


TreeNode *modifyTNStackByOPeration(Token token, StationStack *st_stack, TNStack *tn_stack) {
    TreeNode *root;
    if (token.type == OPERATION_MINUS || token.type == OPERATION_PLUS) {
        Token prev_token;
        while (!(stationStackIsEmpty(st_stack))) {
            prev_token = *stationStackGetToken(st_stack);
            if (prev_token.type == OPERATION_MINUS || prev_token.type == OPERATION_PLUS || prev_token.type == OPERATION_DIVIDE || prev_token.type == OPERATION_MULT) {
                stationStackPop(st_stack);
                TreeNode *node = createTree(&prev_token, NULL, NULL);
                treeNodeStackPush(tn_stack, node);
                root = createNodesInTNStack(tn_stack);
            } else {
                break;
            }
        }
        stationStackPush(st_stack, token);
    } else if (token.type == OPERATION_MULT || token.type == OPERATION_DIVIDE) {
        Token prev_token;
        while (!(stationStackIsEmpty(st_stack))) {
            prev_token = *stationStackGetToken(st_stack);
            if (prev_token.type == OPERATION_DIVIDE || prev_token.type == OPERATION_MULT) {
                stationStackPop(st_stack);
                TreeNode *node = createTree(&prev_token, NULL, NULL);
                treeNodeStackPush(tn_stack, node);
                root = createNodesInTNStack(tn_stack);            
            } else {
                break;
            }
        }
        stationStackPush(st_stack, token);
    } else if (token.type == BRACKETS_OPEN) {
        stationStackPush(st_stack, token);
    } else if (token.type == BRACKETS_CLOSE) {
        Token prev_token;
        bool found_open_bracket = false;
        while (!(stationStackIsEmpty(st_stack))) {
            prev_token = *stationStackGetToken(st_stack);
            if (prev_token.type == OPERATION_MINUS || prev_token.type == OPERATION_PLUS || prev_token.type == OPERATION_DIVIDE || prev_token.type == OPERATION_MULT) {
                stationStackPop(st_stack);
                TreeNode *node = createTree(&prev_token, NULL, NULL);
                treeNodeStackPush(tn_stack, node);
                root = createNodesInTNStack(tn_stack);
            } else if (prev_token.type == BRACKETS_OPEN){
                stationStackPop(st_stack);
                found_open_bracket = true;
                break;
            }
        }
        if (!found_open_bracket)
            return NULL;
    }
    
    return root;
}


TreeNode *createTreeByExpression(const char *expression) {
    
    TreeNode *root;

    const char *operators = "+-*/()";
    const char *p = expression;
    
    StationStack *st_stack = createStationStack();
    TNStack *tn_stack = createTreeNodeStack();

    bool last_token_was_operator = true;

    while (*p != '\0') {
        
        while (isspace(*p)) {
            p++;
        }

        if (strchr(operators, *p) != NULL) {     
            if (last_token_was_operator && *p != '(' && *p != ')')
                return NULL;
            if (*p != '(' && *p != ')')
                last_token_was_operator = true;

            Token token = getTokenByOperationType(*p);
            root = modifyTNStackByOPeration(token, st_stack, tn_stack);
            if (root == NULL)
                return NULL;
            p++;

        } else {
            if (!last_token_was_operator)
                return NULL;
            last_token_was_operator = false;

            const char* start = p;

            while (*p != '\0' && !isspace(*p) && strchr(operators, *p) == NULL) {
                p++;
            }
            int len = p - start;
            if (len > 0) {
                char token_name[VARIABLE_MAX_LENGTH];       
                strncpy(token_name, start, len);
                token_name[len] = '\0';
                
                Token token;
                if (isVariable(token_name)) {
                    token = createVariableToken(token_name);              
                } else {
                    token = createIntegerToken(token_name);
                }
                TreeNode *node = createTree(&token, NULL, NULL);
                treeNodeStackPush(tn_stack, node);            
            }
        }               
    }

    if (last_token_was_operator)
        return NULL;

    while (!stationStackIsEmpty(st_stack)) {
        Token token = *stationStackPop(st_stack);
        if (token.type == BRACKETS_OPEN)
            return NULL;
        TreeNode *node = createTree(&token, NULL, NULL);
        treeNodeStackPush(tn_stack, node);
        root = createNodesInTNStack(tn_stack);
    }

    return root;
}

#pragma region COMPRASION_OF_EXRESSIONS

void applyOneTimeDistributive(TreeNode *node) {
    if (!node || !node->left || !node->right) return;

    // Обработка случая (B +- C) */ A
    if ((node->left->token.type == OPERATION_PLUS || node->left->token.type == OPERATION_MINUS) && 
        (node->token.type == OPERATION_MULT || node->token.type == OPERATION_DIVIDE)) {
        TreeNode *B = node->left->left;
        TreeNode *C = node->left->right;
        TreeNode *A = node->right;

        TreeNode *newLeft = createTree(&node->token, B, A);  // создаем B * A   (B / A)
        TreeNode *newRight = createTree(&node->token, C, A); // создаем C * A   (C / A)
        Token node_token;
        node_token.type = node->left->token.type;

        node->token = node_token; // текущий узел становится + (-)
        node->left = newLeft;
        node->right = newRight;
    }
    // Обработка случая A */ (B +- C)
    else if ((node->right->token.type == OPERATION_PLUS || node->right->token.type == OPERATION_MINUS) && 
            (node->token.type == OPERATION_MULT || node->token.type == OPERATION_DIVIDE)) {
        TreeNode *A = node->left;
        TreeNode *B = node->right->left;
        TreeNode *C = node->right->right;

        TreeNode *newLeft = createTree(&node->token, A, B);  // создаем A * B   (A / B)
        TreeNode *newRight = createTree(&node->token, A, C); // создаем A * C   (A / C) 
        Token node_token;
        node_token.type = node->right->token.type;

        node->token = node_token; // текущий узел становится + (-)
        node->left = newLeft;
        node->right = newRight;
    }
    applyOneTimeDistributive(node->left);
    applyOneTimeDistributive(node->right);
}


bool tokencmp(Token t1, Token t2) {
    if (t1.type != t2.type) {
        return false;
    }
    if (t1.type == VARIABLE) {
        return strcmp(t1.name, t2.name) == 0;
    }
    if (t1.type == INTEGER) {
        return t1.value == t2.value;
    }
    if (tokenIsOperation(t1)) {
        return t1.type == t2.type;
    }
}


int heightOfExpressionTree(TreeNode* root) {
    if (root == NULL)
        return 0;

    int leftHeight = heightOfExpressionTree(root->left);
    int rightHeight = heightOfExpressionTree(root->right);

    return 1 + (leftHeight > rightHeight ? leftHeight : rightHeight);
}


void applyFullDistributive(TreeNode *root) {
    int height = heightOfExpressionTree(root);
    for (int i = 0; i < height; i++) {
        applyOneTimeDistributive(root);
    }
}



/////////////////////////////////////////////////////////////////////////////////////


void liftOrCollabeMinusesOneTime(TreeNode *node) {
    if (node == NULL) {
        return;
    }
    if (node->token.type == OPERATION_MINUS) {
        if (node->left->token.type == OPERATION_MINUS) {
            Token new_right_token = createOperationToken('+');
            TreeNode *new_right_node = createTree(&new_right_token, node->left->right, node->right);
            node->right = new_right_node;
            node->left = node->left->left;
        } else if (node->right->token.type == OPERATION_MINUS) {
            Token new_right_token = createOperationToken('+');
            TreeNode *new_left_node = createTree(&new_right_token, node->left, node->right->right);
            node->left = new_left_node;
            node->right = node->right->left;
        }
    } else if (node->token.type == OPERATION_PLUS) {
        if (node->left->token.type == OPERATION_MINUS) {
            Token new_left_token = createOperationToken('+');
            TreeNode *new_left_node = createTree(&new_left_token, node->left->left, node->right);
            node->right = node->left->right;
            node->left = new_left_node;
            node->token.type = OPERATION_MINUS;
        } else if (node->right->token.type == OPERATION_MINUS) {
            Token new_left_token = createOperationToken('+');
            TreeNode *new_left_node = createTree(&new_left_token, node->left, node->right->left);
            node->right = node->right->right;
            node->left = new_left_node;
            node->token.type = OPERATION_MINUS;
        }
    }
    liftOrCollabeMinusesOneTime(node->left);
    liftOrCollabeMinusesOneTime(node->right);
}

int countMinusesInTree(TreeNode* node) {
    if (node == NULL) {
        return 0;
    }

    int isMinus = (node->token.type == OPERATION_MINUS) ? 1 : 0;
    int leftMinuses = countMinusesInTree(node->left);
    int rightMinuses = countMinusesInTree(node->right);

    return isMinus + leftMinuses + rightMinuses;
}


void liftOrCollabeMinusesFull(TreeNode *root) {
    while (countMinusesInTree(root) > 1) {
        liftOrCollabeMinusesOneTime(root);
    }
    liftOrCollabeMinusesOneTime(root);
}


void buildOneTimeBambooPlus(TreeNode *node) {
    if (node == NULL) {
        return;
    }
    if (node->token.type == OPERATION_PLUS) {
        if (node->left->token.type == OPERATION_PLUS && node->right->token.type == OPERATION_PLUS) {
            Token new_left_token = createOperationToken('+');
            TreeNode *new_left_node = createTree(&new_left_token, node->left, node->right->right);
            node->right = node->right->left; 
            node->left = new_left_node;
        } else if (node->right->token.type == OPERATION_PLUS) {
            TreeNode *temp = node->left;
            node->left = node->right;
            node->right = temp;
        }
    }
    buildOneTimeBambooPlus(node->left);
    buildOneTimeBambooPlus(node->right);
}


//       +           - есть ли такая ситуация 
//         +                                      
bool hasPlusRightChildPlus(TreeNode *node) {
    if (node == NULL) {
        return false;
    }

    if (node->token.type == OPERATION_PLUS) {
        if (node->right->token.type == OPERATION_PLUS) {
                return true;
            }
    }
    return hasPlusRightChildPlus(node->left) || hasPlusRightChildPlus(node->right);
}


void buildFullBambooPlus(TreeNode *root) {
    if (root->token.type == OPERATION_MINUS) {
        while (hasPlusRightChildPlus(root->left) || hasPlusRightChildPlus(root->right)) {
            buildOneTimeBambooPlus(root);
        }
    } else if (root->token.type == OPERATION_PLUS) {
        while (hasPlusRightChildPlus(root)) {
            buildOneTimeBambooPlus(root);
        }
    }
}


void liftOrCollabeDivideOneTime(TreeNode *node) {
    if (node == NULL) {
        return;
    }
    if (node->token.type == OPERATION_DIVIDE) {
        if (node->left->token.type == OPERATION_DIVIDE) {
            Token new_right_token = createOperationToken('*');
            TreeNode *new_right_node = createTree(&new_right_token, node->left->right, node->right);
            node->right = new_right_node;
            node->left = node->left->left;
        } else if (node->right->token.type == OPERATION_DIVIDE) {
            Token new_right_token = createOperationToken('*');
            TreeNode *new_left_node = createTree(&new_right_token, node->left, node->right->right);
            node->left = new_left_node;
            node->right = node->right->left;
        }
    } else if (node->token.type == OPERATION_MULT) {
        if (node->left->token.type == OPERATION_DIVIDE) {
            Token new_left_token = createOperationToken('*');
            TreeNode *new_left_node = createTree(&new_left_token, node->left->left, node->right);
            node->right = node->left->right;
            node->left = new_left_node;
            node->token.type = OPERATION_DIVIDE;
        } else if (node->right->token.type == OPERATION_DIVIDE) {
            Token new_left_token = createOperationToken('*');
            TreeNode *new_left_node = createTree(&new_left_token, node->left, node->right->left);
            node->right = node->right->right;
            node->left = new_left_node;
            node->token.type = OPERATION_DIVIDE;
        }
    }
    liftOrCollabeDivideOneTime(node->left);
    liftOrCollabeDivideOneTime(node->right);

}

int countDividesInTree(TreeNode* node) {
    if (node == NULL) {
        return 0;
    }

    int isDivide = (node->token.type == OPERATION_DIVIDE) ? 1 : 0;
    int leftDivides = countDividesInTree(node->left);
    int rightDivides = countDividesInTree(node->right);

    return isDivide + leftDivides + rightDivides;
}

void liftOrCollabeDivideFullInMonom(TreeNode *root) {
    while (countDividesInTree(root) > 1) {
        liftOrCollabeDivideOneTime(root);
    }
    liftOrCollabeDivideOneTime(root);
}

void buildOneTimeBambooMult(TreeNode *node) {
    if (node == NULL) {
        return;
    }
    if (node->token.type == OPERATION_MULT) {
        if (node->left->token.type == OPERATION_MULT && node->right->token.type == OPERATION_MULT) {
            Token new_left_token = createOperationToken('*');
            TreeNode *new_left_node = createTree(&new_left_token, node->left, node->right->right);
            node->right = node->right->left; 
            node->left = new_left_node;
        } else if (node->right->token.type == OPERATION_MULT) {
            TreeNode *temp = node->left;
            node->left = node->right;
            node->right = temp;
        }
    }
    buildOneTimeBambooMult(node->left);
    buildOneTimeBambooMult(node->right);
}


//       *           - есть ли такая ситуация 
//         *                                      
bool hasMultRightChildMult(TreeNode *node) {
    if (node == NULL) {
        return false;
    }

    if (node->token.type == OPERATION_MULT) {
        if (node->right->token.type == OPERATION_MULT) {
                return true;
        }
    }
    return hasMultRightChildMult(node->left) || hasMultRightChildMult(node->right);
}


void buildFullBambooMult(TreeNode *root) {
    if (root->token.type == OPERATION_MINUS) {
        while (hasMultRightChildMult(root->left) || hasMultRightChildMult(root->right)) {
            buildOneTimeBambooPlus(root);
        }
    } else if (root->token.type == OPERATION_PLUS) {
        while (hasMultRightChildMult(root)) {
            buildOneTimeBambooMult(root);
        }
    }
}

// К КАНОНИЧЕСКОМУ ВИДУ, ЧТОБЫ БЫЛ МИНИМУМ ОДИН МИНУС И У НЕГО ДЕТИ ПЛЮСЫ
TreeNode *addTwoPluses(TreeNode *old_root) {
    Token plus_t = createOperationToken('+');
    Token zero_t = createIntegerToken("0");
    Token minus_t = createOperationToken('-');
    TreeNode *zero1_n = createTree(&zero_t, NULL, NULL);
    TreeNode *zero2_n = createTree(&zero_t, NULL, NULL);
    TreeNode *left_plus_n = createTree(&plus_t, old_root->left, zero1_n);
    TreeNode *right_plus_n = createTree(&plus_t, old_root->right, zero2_n);
    TreeNode *new_root = createTree(&minus_t, left_plus_n, right_plus_n);
    return new_root;
}


TreeNode *addMainMinus(TreeNode *old_root) {
    Token zero_t = createIntegerToken("0");
    TreeNode *zero_n = createTree(&zero_t, NULL, NULL);
    Token minus_t = createOperationToken('-');
    TreeNode *new_root = createTree(&minus_t, old_root, zero_n);
    return new_root;
} 


TreeNode *addDivideInMonom(TreeNode *old_root) {
    if (old_root->token.type == OPERATION_DIVIDE) {
        return old_root;
    }
    Token one_t = createIntegerToken("1");
    TreeNode *one_n = createTree(&one_t, NULL, NULL);
    Token div_t = createOperationToken('/');
    TreeNode *new_root = createTree(&div_t, old_root, one_n);
    return new_root;
}


// ИТЕРАТОРЫ И СРАВНЕНИЕ ПОДДЕРЕВЬЕВ

// Внутри монома ===
int monomSize(TreeNode *root, int setNum) {
    TreeNode *point = root;
    if (setNum == 1)
        point = root->left;
    else if (setNum == 2)
        point = root->right;
    
    int count = 1;
    
    while (point->left != NULL) {
        point = point->left;
        count++;
    }
    return count;
}
    
Token getTokenFromMonom(TreeNode *node, int setNum, int n) {
    TreeNode *point = node;

    if (setNum == 1) 
        point = point->left;
    else if (setNum == 2) 
        point = point->right;  

    for (int i = 0; i < n; i++) 
        point = point->left;
    
    if (point->left == NULL) 
        return point->token;

    return point->right->token;
}

bool twoMonomBranchesAreEqual(TreeNode *node1, TreeNode *node2, int size, int setNum) {
    bool *arr1 = malloc(sizeof(bool) * size);
    bool *arr2 = malloc(sizeof(bool) * size);

    for (int i = 0; i < size; i++) {
        Token t1 = getTokenFromMonom(node1, setNum, i);
        for (int j = 0; j < size; j++) {
            Token t2 = getTokenFromMonom(node2, setNum, j);
            if (tokencmp(t1, t2) && arr2[j] == false) {
                arr1[i] = true;
                arr2[j] = true;
                break;
            }
        }
    }

    for (int i = 0; i < size; i++) {
        if (arr1[i] == false) {
            free(arr1);
            free(arr2);
            return false;
        }
    }
    free(arr1);
    free(arr2);
    return true;
}

bool monomsAreEqual(TreeNode *node1, TreeNode *node2) {

    int size_1_1 = monomSize(node1, 1);
    int size_1_2 = monomSize(node1, 2);
    int size_2_1 = monomSize(node2, 1);
    int size_2_2 = monomSize(node2, 2);
    
    if (size_1_1 != size_2_1 ||
        size_1_2 != size_2_2) {           
        return false;
    }

    bool eq_left = twoMonomBranchesAreEqual(node1, node2, size_1_1, 1);
    bool eq_right = twoMonomBranchesAreEqual(node1, node2, size_1_2, 2);
    
    return eq_left && eq_right;
}

// \Внутри монома ===

// Итерация по мономам и перебор мономов для сравнения в другом дереве 
int bambooSize(TreeNode *root, int setNum) {
    TreeNode *point = root;
    if (setNum == 1)
        point = root->left;
    else if (setNum == 2)
        point = root->right;
    
    int count = 1;

    while (point->left->token.type == OPERATION_PLUS) {
        point = point->left;
        count++;
    }
    return count + 1;
}

TreeNode *getMonomFromBamboo(TreeNode *node, int setNum, int n) {
    TreeNode *point = node;
    if (setNum == 1) 
        point = point->left;
    else if (setNum == 2) 
        point = point->right;  

    for (int i = 0; i < n; i++) 
        point = point->left;
        
    if (point->token.type != OPERATION_PLUS) 
        return point;

    return point->right;
}

void goToMonomAndListOrCollabeDivide(TreeNode *node, int setNum, int n) {
    TreeNode *point = node;
    if (setNum == 1) 
        point = point->left;
    else if (setNum == 2) 
        point = point->right;  

    if (n == bambooSize(node, setNum) - 1) {
        for (int i = 0; i < n - 1; i++) 
            point = point->left;
        liftOrCollabeDivideFullInMonom(point->left);
    } else {
        for (int i = 0; i < n; i++) 
            point = point->left;
        liftOrCollabeDivideFullInMonom(point->right);
    }  
}

void liftOrCollabeDivideFullInAllTree(TreeNode *root) {
    int size_left = bambooSize(root, 1);
    int size_right = bambooSize(root, 2);

    for (int i = 0; i < size_left; i++) {
        goToMonomAndListOrCollabeDivide(root, 1, i);
    }
    
    for (int i = 0; i < size_right; i++) {
        goToMonomAndListOrCollabeDivide(root, 2, i);
    }
}

void setCanonizeMonomToPlus(TreeNode *node, int setNum, int n) {
    TreeNode *point = node;
    if (setNum == 1) 
        point = point->left;
    else if (setNum == 2) 
        point = point->right;  

    if (n == bambooSize(node, setNum) - 1) {
        for (int i = 0; i < n - 1; i++) 
            point = point->left;
        point->left = addDivideInMonom(point->left);
    } else {
        for (int i = 0; i < n; i++) 
            point = point->left;
        point->right = addDivideInMonom(point->right);
    }  
}

void addDividesInMonomesInAllTree(TreeNode *root) {
    int size_left = bambooSize(root, 1);
    int size_right = bambooSize(root, 2);

    for (int i = 0; i < size_left; i++) {
        setCanonizeMonomToPlus(root, 1, i);
    }
    
    for (int i = 0; i < size_right; i++) {
        setCanonizeMonomToPlus(root, 2, i);
    }
}


bool twoMomomesInBambooAreEqual(TreeNode *node1, TreeNode *node2, int size, int setNum) {
    bool *arr1 = malloc(size * sizeof(bool));
    bool *arr2 = calloc(size, sizeof(bool));

    for (int i = 0; i < size; i++) {
        TreeNode *m1 = getMonomFromBamboo(node1, setNum, i);
        for (int j = 0; j < size; j++) {
            TreeNode *m2 = getMonomFromBamboo(node2, setNum, j);
            if (monomsAreEqual(m1, m2) && arr2[j] == false) {           
                arr1[i] = true;
                arr2[j] = true;
                break;
            }
        }
    }

    for (int i = 0; i < size; i++) {
        if (arr1[i] == false) { 
            free(arr1);
            free(arr2);
            return false;
        }
    }
    free(arr1);
    free(arr2);
    return true;
}


bool mainBamboosAreEqual(TreeNode *node1, TreeNode *node2) {

    int size_1_1 = bambooSize(node1, 1);
    int size_1_2 = bambooSize(node1, 2);
    int size_2_1 = bambooSize(node2, 1);
    int size_2_2 = bambooSize(node2, 2);
    
    if (size_1_1 != size_2_1 ||
        size_1_2 != size_2_2) {           
        return false;
    }
    
    bool eq_left = twoMomomesInBambooAreEqual(node1, node2, size_1_1, 1);
    bool eq_right = twoMomomesInBambooAreEqual(node1, node2, size_1_2, 2);
    
    return eq_left && eq_right;
}


TreeNode *canonizeTree(TreeNode *root) {
    root = addMainMinus(root);
    root = addTwoPluses(root);
    applyFullDistributive(root);
    liftOrCollabeMinusesFull(root);
    buildFullBambooPlus(root);
    liftOrCollabeDivideFullInAllTree(root);
    buildOneTimeBambooMult(root);
    addDividesInMonomesInAllTree(root);
    return root;
}

#pragma endregion COMPRASION_OF_EXRESSIONS


void testExpression(const char* expression, bool expectedSuccess) {
    TreeNode* tree = createTreeByExpression(expression);
    bool success = (tree != NULL);
    
    printf("Testing: %s\n", expression);
    printf("%d\n", tree == NULL);
    if (success == expectedSuccess) {
        printf("Test passed.\n");
        if (tree) {
            treeSimplePrint(tree, 0);
        }
    } else {
        printf("Test failed.\n");
    }
    printf("\n");
}


int main() {
    TreeNode *root1 = createTreeByExpression("(y1 + y2 + y4) * (y2 + y3 + y4) * (y1 + y3) * y4");
    TreeNode *root2 = createTreeByExpression("y1 * y2 + y1 * y4 + y2 * y3 + y3 * y4");

    if (!(root1 && root2)) {
        printf("Некорректное выражение.\n");
        return 0;
    }
    
    printf("Начальный вид:\n");
    treeSimplePrint(root1, 0);
    printf("----------\n");
    treeSimplePrint(root2, 0);

    root1 = canonizeTree(root1);
    root2 = canonizeTree(root2);

    printf("Канонический вид:\n");
    treeSimplePrint(root1, 0);
    printf("----------\n");
    treeSimplePrint(root2, 0);

    bool eq = mainBamboosAreEqual(root1, root2);
    if (eq)
        printf("Выражения эквивалентны.\n");
    else 
        printf("Выражения не являются эквивалентными.\n");

    return 0;
}
