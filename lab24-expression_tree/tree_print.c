#include "tree_print.h"

// ================
// Tree print
// ================

// === String functions ===

char* intToString(int val, int base) {
	static char buf[32] = {0};
    if (val == 0) {
        buf[0] = '0';
        buf[1] = '\0';
        return buf;
    }
	int i = 30;
	for(; val && i ; --i, val /= base)
		buf[i] = "0123456789abcdef"[val % base];
	return &buf[i+1];
}

// Returns unsafe string
char *stringGenerate(char c, int n) {
    char *result = malloc(sizeof(char) * (n + 1));
    if (n <= 0) {
        result[0] = '\0';
        return result;
    }
    for (int i = 0; i < n; i++) {
        result[i] = c;
    }
    result[n] = '\0';
    return result;
}

void stringConcatenate(char *str1, char *str2) {
    int ptr1 = 0, ptr2 = 0;
    while (str1[ptr1] != '\0') ptr1++;
    while (str2[ptr2] != '\0') {
        str1[ptr1] = str2[ptr2];
        ptr1++; ptr2++;
    }
    str1[ptr1] = '\0';
}

// Returns unsafe string
char *stringGenerateCell(char *str) {
    char *result = malloc(sizeof(char) * (CELL_LENGTH + 1));
    char curResult[20];
    curResult[0] = '\0';
    result[0] = '\0';
    stringConcatenate(curResult, str);
    int numberLength = strlen(curResult);
    int diff = CELL_LENGTH - numberLength;
    assert(diff >= 0);

    // Helping strings
    char *s1 = stringGenerate(' ', diff / 2);
    char *s2 = stringGenerate(' ', diff / 2 + 1);
    //

    if (diff % 2 == 0) {
        stringConcatenate(result, s1);
        stringConcatenate(result, curResult);
        stringConcatenate(result, s1);
    } else {
        stringConcatenate(result, s1);
        stringConcatenate(result, curResult);
        stringConcatenate(result, s2);
    }
    free(s1); free(s2);
    return result;
}

// ========================\ String functions

// === TextChunk interface ===

TextChunkNode *TextChunkNodeCreate() {
    TextChunkNode *node = malloc(sizeof(TextChunkNode));
    node->str = malloc(sizeof(char) * MAX_STRING_LENGTH);
    node->str[0] = '\0';
    node->next = NULL;
    return node;
}


TextChunk *textChunkCreate() {
    TextChunk *chunk = malloc(sizeof(TextChunk));
    chunk->rowCount = 0;
    chunk->columnCount = 0;
    chunk->first = NULL;
    chunk->last = NULL;
    return chunk;
}

void textChunkAdd(TextChunk *chunk1, TextChunk *chunk2) {
    assert(chunk1 != NULL && chunk2 != NULL);
    assert(chunk1->rowCount == chunk2->rowCount);
    if (chunk1->rowCount == 0) return;
    TextChunkNode *node1 = chunk1->first, *node2 = chunk2->first;
    while (node1 != NULL) {
        stringConcatenate(node1->str, node2->str);

        node1 = node1->next; node2 = node2->next;
    }
    chunk1->columnCount += chunk2->columnCount;
}

void textChunkConcatenate(TextChunk *chunk1, TextChunk *chunk2) {
    assert(chunk1 != NULL && chunk2 != NULL);
    if (chunk1->rowCount == 0) {
        chunk1->first = chunk2->first;
        chunk1->last = chunk2->last;
    } else {
        chunk1->last->next = chunk2->first;
        if (chunk2->last != NULL) chunk1->last = chunk2->last;
    }
    chunk1->rowCount += chunk2->rowCount;
}

TextChunk *textChunkGenerateBlank(int n, int m) {
    TextChunk *result = textChunkCreate();
    char *sm = stringGenerate(' ', m);
    for (int i = 0;i < n;i++) {
        textChunkPush(result, sm);
    }
    free(sm);
    return result;
}

TextChunkNode *textChunkGet(TextChunk *chunk, int index) {
    assert(index < chunk->rowCount);
    TextChunkNode *node = chunk->first;
    while (index--) node = node->next;
    return node;
}

void textChunkPush(TextChunk *chunk, char *str) {
    assert(chunk != NULL);
    TextChunkNode *node = TextChunkNodeCreate();
    stringConcatenate(node->str, str);
    if (chunk->rowCount == 0) {
        chunk->first = node;
        chunk->last = node;
        chunk->rowCount = 1;
        chunk->columnCount = strlen(str);
    } else {
        assert((int)strlen(str) == chunk->columnCount);
        chunk->last->next = node;
        chunk->last = node;
        chunk->rowCount++;
    }
}

void textChunkComplement(TextChunk *chunk, int n) {
    assert(chunk != NULL);
    if (chunk->rowCount < n) {
        TextChunk *blank = textChunkGenerateBlank(n - chunk->rowCount, chunk->columnCount);
        textChunkConcatenate(chunk, blank);
        free(blank);
    }
}

void textChunkEven(TextChunk *chunk1, TextChunk *chunk2) {
    textChunkComplement(chunk1, chunk2->rowCount);
    textChunkComplement(chunk2, chunk1->rowCount);
}

void textChunkPrint(TextChunk *chunk) {
    assert(chunk != NULL);
    TextChunkNode *node = chunk->first;
    while (node != NULL) {
        printf("%s\n", node->str);
        node = node->next;
    }
    printf("\n");
}

void textChunkNodeDestroy(TextChunkNode *node) {
    assert(node != NULL);
    free(node->str);
    free(node);
}

void textChunkDestroy(TextChunk *chunk) {
    assert(chunk != NULL);
    if (chunk->first == NULL) {
        free(chunk);
        return;
    }
    TextChunkNode *node, *next;
    node = chunk->first;
    while (node != NULL) {
        next = node->next;
        textChunkNodeDestroy(node);
        node = next;
    }
    free(chunk);
}

// =========================== \TextChunk interface

TextChunk *treeGenerateTextChunkForSubtree(void *node,
                                           char *(*valueAccessor)(void*),
                                           void *(*leftChildAccessor)(void*),
                                           void *(*rightChildAccessor)(void*)) {
    assert(node != NULL);
    char *value = valueAccessor(node);
    if (leftChildAccessor(node) == NULL && rightChildAccessor(node) == NULL) {
        TextChunk *chunk = textChunkCreate();
        char *cellStr = stringGenerateCell(value);
        chunk->rootIndex = strlen(cellStr) / 2;
        textChunkPush(chunk, cellStr);
        free(cellStr); free(value);
        return chunk;
    } else if (rightChildAccessor(node) == NULL) {
        TextChunk *lChunk = treeGenerateTextChunkForSubtree(leftChildAccessor(node),
                                                            valueAccessor,
                                                            leftChildAccessor,
                                                            rightChildAccessor);
        TextChunk *spaceChunk = textChunkGenerateBlank(lChunk->rowCount, SUBTREE_SPACING);
        TextChunk *rChunk = textChunkGenerateBlank(lChunk->rowCount, lChunk->columnCount);

        int subtreeWidth = lChunk->columnCount;
        int spaceWidth = SUBTREE_SPACING;
        int allWidth = 2 * subtreeWidth + spaceWidth;

        textChunkAdd(lChunk, spaceChunk);
        textChunkAdd(lChunk, rChunk);

        textChunkDestroy(spaceChunk);
        textChunkDestroy(rChunk);

        char *str1 = malloc(sizeof(char) * (allWidth + 1));
        char *str2 = malloc(sizeof(char) * (allWidth + 1));
        str1[0] = '\0'; str2[0] = '\0';
        // stringConcatenate(str2, s5);
        // stringConcatenate(str2, "|");
        // stringConcatenate(str2, s6);

        // Helping
        char *s1 = stringGenerate(' ', (allWidth - CELL_LENGTH) / 2);
        char *s2 = stringGenerateCell(value);
        char *s3 = stringGenerate(' ', subtreeWidth / 2);
        char *s4 = stringGenerate('-', subtreeWidth / 2);
        char *s5 = stringGenerate(' ', spaceWidth / 2);
        char *s6 = stringGenerate(' ', subtreeWidth + (spaceWidth / 2));
        //

        // str1
        stringConcatenate(str1, s1);
        stringConcatenate(str1, s2);
        stringConcatenate(str1, s1);

        // str2
        stringConcatenate(str2, s3);
        stringConcatenate(str2, "|");
        stringConcatenate(str2, s4);
        stringConcatenate(str2, s5);
        stringConcatenate(str2, "+");
        stringConcatenate(str2, s6);

        int rootIndex = strlen(s3) + 1 + strlen(s4) + strlen(s5);

        free(s1); free(s2); free(s3); free(s4); free(s5); free(s6);

        TextChunk *header = textChunkCreate();
        textChunkPush(header, str1);
        textChunkPush(header, str2);

        textChunkConcatenate(header, lChunk); // lChunk is alive !!!

        header->rootIndex = rootIndex;
        free(lChunk);
        free(str1); free(str2); free(value);
        return header;
    } else if (leftChildAccessor(node) == NULL) {
        TextChunk *rChunk = treeGenerateTextChunkForSubtree(rightChildAccessor(node),
                                                            valueAccessor,
                                                            leftChildAccessor,
                                                            rightChildAccessor);
        TextChunk *lChunk = textChunkGenerateBlank(rChunk->rowCount, rChunk->columnCount);
        TextChunk *spaceChunk = textChunkGenerateBlank(rChunk->rowCount, SUBTREE_SPACING);

        int subtreeWidth = rChunk->columnCount;
        int spaceWidth = SUBTREE_SPACING;
        int allWidth = 2 * subtreeWidth + spaceWidth;

        textChunkAdd(lChunk, spaceChunk);
        textChunkAdd(lChunk, rChunk);

        textChunkDestroy(spaceChunk);
        textChunkDestroy(rChunk);

        char *str1 = malloc(sizeof(char) * (allWidth + 1));
        char *str2 = malloc(sizeof(char) * (allWidth + 1));
        str1[0] = '\0'; str2[0] = '\0';

        // Helping
        char *s1 = stringGenerate(' ', (allWidth - CELL_LENGTH) / 2);
        char *s2 = stringGenerateCell(value);
        char *s3 = stringGenerate(' ', subtreeWidth / 2);
        char *s5 = stringGenerate('-', spaceWidth / 2 + subtreeWidth / 2);
        char *s6 = stringGenerate(' ', subtreeWidth + (spaceWidth / 2));
        //

        // str1
        stringConcatenate(str1, s1);
        stringConcatenate(str1, s2);
        stringConcatenate(str1, s1);

        // str2
        stringConcatenate(str2, s6);
        stringConcatenate(str2, "+");
        stringConcatenate(str2, s5);
        stringConcatenate(str2, "|");
        stringConcatenate(str2, s3);

        int rootIndex = strlen(s6);

        free(s1); free(s2); free(s3); free(s5); free(s6);

        TextChunk *header = textChunkCreate();
        textChunkPush(header, str1);
        textChunkPush(header, str2);

        textChunkConcatenate(header, lChunk); // lChunk is alive!!!
        header->rootIndex = rootIndex;

        free(lChunk);
        free(str1); free(str2); free(value);
        return header;
    } else {
        TextChunk *rChunk = treeGenerateTextChunkForSubtree(rightChildAccessor(node),
                                                            valueAccessor,
                                                            leftChildAccessor,
                                                            rightChildAccessor);
        TextChunk *lChunk = treeGenerateTextChunkForSubtree(leftChildAccessor(node),
                                                            valueAccessor,
                                                            leftChildAccessor,
                                                            rightChildAccessor);

        textChunkEven(lChunk, rChunk);

        TextChunk *spaceChunk = textChunkGenerateBlank(lChunk->rowCount, SUBTREE_SPACING);

        int leftWidth = lChunk->columnCount;
        int spaceWidth = SUBTREE_SPACING;
        int rightWidth = rChunk->columnCount;
        int allWidth = leftWidth + spaceWidth + rightWidth;
        int lRootIndex = lChunk->rootIndex;
        int rRootIndex = rChunk->rootIndex;


        textChunkAdd(lChunk, spaceChunk);
        textChunkAdd(lChunk, rChunk);

        textChunkDestroy(spaceChunk);
        textChunkDestroy(rChunk);

        char *str1 = malloc(sizeof(char) * (allWidth + 1));
        char *str2 = malloc(sizeof(char) * (allWidth + 1));
        str1[0] = '\0'; str2[0] = '\0';

        // Helping
        char *s1 = stringGenerate(' ', (leftWidth + spaceWidth / 2 - CELL_LENGTH / 2));
        char *s2 = stringGenerate(' ', (rightWidth + spaceWidth / 2 - CELL_LENGTH / 2));
        char *s3 = stringGenerate(' ', lRootIndex);
        char *s4 = stringGenerate('-', leftWidth + spaceWidth / 2 - lRootIndex - 1);
        char *s5 = stringGenerate('-', leftWidth + spaceWidth + rRootIndex - (leftWidth + spaceWidth / 2 + 1));
        char *s6 = stringGenerate(' ', rightWidth - rRootIndex - 1);
        char *s7 = stringGenerateCell(value);
        //

        // str1
        stringConcatenate(str1, s1);
        stringConcatenate(str1, s7);
        stringConcatenate(str1, s2);

        // str2
        stringConcatenate(str2, s3);
        stringConcatenate(str2, "|");
        stringConcatenate(str2, s4);
        stringConcatenate(str2, "+");
        stringConcatenate(str2, s5);
        stringConcatenate(str2, "|");
        stringConcatenate(str2, s6);

        int rootIndex = strlen(s3) + 1 + strlen(s4);

        free(s1); free(s2); free(s3); free(s4); free(s5); free(s6); free(s7);

        TextChunk *header = textChunkCreate();
        textChunkPush(header, str1);
        textChunkPush(header, str2);

        textChunkConcatenate(header, lChunk); // lChunk is alive!!!
        header->rootIndex = rootIndex;

        free(lChunk);
        free(str1); free(str2); free(value);
        return header;
    }
}

void treePrintUniversal(void *node,
                        char *(*valueAccessor)(void*),
                        void *(*leftChildAccessor)(void*),
                        void *(*rightChildAccessor)(void*)) {
    TextChunk *chunk = treeGenerateTextChunkForSubtree(node, valueAccessor, leftChildAccessor, rightChildAccessor);
    textChunkPrint(chunk);
    textChunkDestroy(chunk);
}