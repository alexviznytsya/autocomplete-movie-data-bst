/*bst.h*/

//
// Binary Search Tree ADT header file.
// Alex Viznytsya, prof. Joe Hummel
// MacOS Sierra, Xcode 8.2.1
// Spring 2017
//

// make sure this header file is #include exactly once:
#pragma once


//
// BST type declarations:
//
typedef char   *BSTKey;
typedef struct BSTValue {
    long long   weight;
    char        *text;
} BSTValue;

typedef struct BSTNode {
    BSTKey    Key;
    BSTValue  Value;
    struct BSTNode  *Left;
    struct BSTNode  *Right;
} BSTNode;

typedef struct BST {
    BSTNode *Root;
    int      Count;
} BST;


//
// BST API: function prototypes
//
BST *BSTCreate();

int  BSTCompareKeys(BSTKey key1, BSTKey key2);

BSTNode *BSTSearch(BST *tree, BSTKey key);

int  BSTSearchDepth(BST *tree, BSTKey key);

int  BSTInsert(BST *tree, BSTKey key, BSTValue value);

int  BSTHeight(BST *tree);

int BSTSubtreeCount(BSTNode *root);

BSTNode *BSTSearchSubstrings(BST *tree, BSTKey key);

BSTValue *BSTMatchSubstrings(BSTNode *subtree, char *key, int count, int *index);
