/*bst.c*/

//
// Binary Search Tree ADT implementation file.
// Alex Viznytsya, prof. Joe Hummel
// MacOS Sierra, Xcode 8.2.1
// Spring 2017

// ignore stdlib warnings if working in Visual Studio:
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "bst.h"


//
// BSTCreate: dynamically creates and returns an empty
// binary search tree:
//
BST *BSTCreate() {
    BST *tree;
    
    tree = (BST *)malloc(sizeof(BST));
    tree->Root = NULL;
    tree->Count = 0;
    
    return tree;
}


//
// BSTCompareKeys: compares key1 and key2, returning
//   value < 0 if key1 <  key2
//   0         if key1 == key2
//   value > 0 if key1 >  key2
//
int BSTCompareKeys(BSTKey key1, BSTKey key2) {
    if (strcmp(key1, key2) < 0)
        return -1;
    else if (strcmp(key1, key2) == 0)
        return 0;
    else
        return 1;
}


//
// BSTSearch: searches the binary search tree for a node containing the
// same key.  If a match is found, a pointer to the node is returned,
// otherwise NULL is returned.
//
BSTNode *BSTSearch(BST *tree, BSTKey key) {
    BSTNode *cur = tree->Root;
    
    //
    // search the tree to see if it contains a matching key:
    //
    while (cur != NULL) {
        if (BSTCompareKeys(key, cur->Key) == 0) {  // found!
            return cur;
        } else if (BSTCompareKeys(key, cur->Key) < 0) {  // smaller, go left:
            cur = cur->Left;
        } else { // larger, go right:
            cur = cur->Right;
        }
    }
    
    // if we get here, we fell out of the tree, and didn't find it:
    return NULL;
}


//
// BSTSearchDepth: searches the binary search tree for a node containing the
// same key, returning the depth of that node if found.  The root node is
// considered depth 0, the next level down is a depth of 1, and so forth.
// If a matching key is not found, the function returns a depth of -1.
//
int BSTSearchDepth(BST *tree, BSTKey key) {
    BSTNode *cur = tree->Root;
    int depth = 0;
    
    //
    // search the tree to see if it contains a matching key:
    //
    while (cur != NULL) {
        if (BSTCompareKeys(key, cur->Key) == 0) { // found!
            return depth;
        } else if (BSTCompareKeys(key, cur->Key) < 0) { // smaller, go left:
            cur = cur->Left;
        } else { // larger, go right:
            cur = cur->Right;
        }
        depth++;
    }
    
    // if we get here, we fell out of the tree, and didn't find it:
    return -1;
}


//
// BSTInsert: inserts the given (key, value) pair into the binary search
// tree.  Returns true (non-zero) if the insert was successful, returns
// false (0) if the given key is already in the tree -- in which case the
// given (key, value) pair is not inserted.
//
int BSTInsert(BST *tree, BSTKey key, BSTValue value) {
    BSTNode *prev = NULL;
    BSTNode *cur = tree->Root;
    
    //
    // first we search the tree to see if it already contains key:
    //
    while (cur != NULL) {
        if (BSTCompareKeys(key, cur->Key) == 0) { // already in tree, failed:
            return 0;
        } else if (BSTCompareKeys(key, cur->Key) < 0) {  // smaller, go left:
            prev = cur;
            cur = cur->Left;
        } else { // larger, go right:
            prev = cur;
            cur = cur->Right;
        }
    }
    
    //
    // If we get here, tree does not contain key, so insert new node
    // where we fell out of tree:
    //
    BSTNode *T = (BSTNode *)malloc(sizeof(BSTNode));
    T->Key = key;
    T->Value = value;
    T->Left = NULL;
    T->Right = NULL;
    
    //
    // link T where we fell out of tree -- after prev:
    //
    if (prev == NULL) {  // tree is empty, insert @ root:
        tree->Root = T;
    } else if (BSTCompareKeys(key, prev->Key) < 0) { // smaller, insert to left:
        prev->Left = T;
    } else { // larger, insert to right:
        prev->Right = T;
    }
    tree->Count++;

    return 1;  // success:
}


int _getMax(int num1, int num2) {
    return (num1 > num2) ? num1 : num2;
}


int _BSTHeight(BSTNode *root) {
    if(root == 0) {
        return -1;
    } else {
        return 1 + _getMax(_BSTHeight(root->Left), _BSTHeight(root->Right));
    }
}


int BSTHeight(BST *tree) {
    int treeHeight = _BSTHeight(tree->Root);
    return treeHeight;
}


BSTNode *BSTSearchSubstrings(BST *tree, BSTKey key) {
    BSTNode *cur = tree->Root;
    
    //
    // search the tree to see if it contains any matching substrings:
    //
    while (cur != NULL) {
        char *prefix = (char *)malloc((strlen(key)+1) * sizeof(char));
        strncpy(prefix, cur->Key, strlen(key));
        prefix[strlen(key)] = '\0';
        
        if(BSTCompareKeys(key, prefix) == 0) {
            return cur;
        } else if (BSTCompareKeys(key, prefix) < 0) {
            cur = cur->Left;
        } else {
            cur = cur->Right;
        }
    }
    
    // if we get here, we fell out of the tree, and didn't find it:
    return NULL;
}


int _BSTSubtreeCont(BSTNode *root) {
    if(root == NULL) {
        return 0;
    } else {
        return 1 + _BSTSubtreeCont(root->Left) + _BSTSubtreeCont(root->Right);
    }
}


int BSTSubtreeCount(BSTNode *root) {
    return _BSTSubtreeCont(root);
}


void _BSTMatchSubstrings(BSTNode *root, BSTValue *values, char *key, int *index) {
    if(root == NULL) {
        return;
    } else {
        _BSTMatchSubstrings(root->Left, values, key, index);
        
        char *prefix = (char *)malloc((strlen(key)+1) * sizeof(char));
        strncpy(prefix, root->Key, strlen(key));
        prefix[strlen(key)] = '\0';
        
        if(BSTCompareKeys(key, prefix) == 0) {
            values[*index] = root->Value;
            (*index)++;
        }
        
        _BSTMatchSubstrings(root->Right, values, key, index);
    }
}


void _sortDescByWeightAndText(BSTValue *values, int valuesCount) {
    
    int i = 0;
    int j = 0;
    BSTValue tValue;
    
    //using insertion sotring, sort students list by student's major and then
    //by student's NetID
    for(i = 1; i < valuesCount; i++) {
        j = i;
        while(j > 0 && values[j].weight >= values[j - 1].weight) {
            
            if(values[j].weight == values[j - 1].weight && strcmp(values[j].text, values[j - 1].text) < 0) {
                tValue = values[j];
                values[j] = values[j - 1];
                values[j -1] = tValue;
                j--;
            } else if(values[j].weight > values[j - 1].weight) {
                tValue = values[j];
                values[j] = values[j - 1];
                values[j -1] = tValue;
                j--;
            } else {
                break;
            }
        }
    }
}


BSTValue *BSTMatchSubstrings(BSTNode *subtree, char *key, int count, int *index) {
    
    *index = 0;
    
    BSTValue *values = (BSTValue *)malloc(count * sizeof(BSTValue));
    
    _BSTMatchSubstrings(subtree, values, key, index);
    
    _sortDescByWeightAndText(values, *index);
    
    return values;
}
