/*autocomplete-movie-data-bst.c*/

//
// Auto-complete using a binary search tree.
//
// Alex Viznytsya
// MacOS Sierra, Xcode 8.2.1
// Spring 2017
//

// ignore stdlib warnings if working in Visual Studio:
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

#include "bst.h"

//
// skipRestOfInput:
//
// Inputs and discards the remainder of the current line for the
// given input stream, including the EOL character(s).
//
void skipRestOfInput(FILE *stream) {
    char restOfLine[256];
    int rolLength = sizeof(restOfLine) / sizeof(restOfLine[0]);
    
    fgets(restOfLine, rolLength, stream);
}

//
// getFileName: inputs a filename from the keyboard, make sure the file can be
// opened, and returns the filename if so.  If the file cannot be opened, an
// error message is output and the program is exited.
//
char *getFileName() {
    
    char filename[512];
    int  fnsize = sizeof(filename) / sizeof(filename[0]);
    
    // input filename from the keyboard:
    fgets(filename, fnsize, stdin);
    filename[strcspn(filename, "\r\n")] = '\0';  // strip EOL char(s):
    
    // make sure filename exists and can be opened:
    FILE *infile = fopen(filename, "r");
    if (infile == NULL)
    {
        printf("**Error: unable to open '%s'\n\n", filename);
        exit(-1);
    }
    
    fclose(infile);
    
    // duplicate and return filename:
    char *s = (char *)malloc((strlen(filename) + 1) * sizeof(char));
    strcpy(s, filename);
    
    return s;
}


void readfileToBST(char *filename, BST *tree) {
    
    FILE *file = fopen(filename, "r");

    long long weight = 0;
    char text[512];
    int  textSize = sizeof(text) / sizeof(text[0]);
    
    fscanf(file, "%lld", &weight);
    fgets(text, textSize, file);
    

    while(!feof(file)) {
        
        text[strcspn(text, "\r\n")] = '\0';
        char *cp = text;
        
        while (*cp == ' ' || *cp == '\t') {
            cp++;
        }
        
        BSTValue value;
        value.weight = weight;
        value.text = (char *)malloc(((int)strlen(cp) + 1) * sizeof(char));
        strcpy(value.text, cp);
        BSTKey key = value.text;
        
        BSTInsert(tree, key, value);
        
        fscanf(file, "%lld", &weight);
        fgets(text, textSize, file);
    }
    
    fclose(file);
    
}


int main() {
    
    char *filename;
    char  cmd[64];
    char  text[512];
    //int   textsize = sizeof(text) / sizeof(text[0]);
    
    printf("** Starting Autocomplete **\n");

    filename = getFileName();

    // now interact with user:
    
    printf("** Ready **\n");
    
    BST *tree = BSTCreate();
    readfileToBST(filename, tree);
    
    scanf("%s", cmd);
    
    while (strcmp(cmd, "exit") != 0) {
        
        if (strcmp(cmd, "stats") == 0) {
            
            // Print stats:
            
            printf("**Tree count:  %d\n", tree->Count);
            printf("**Tree Height: %d\n", BSTHeight(tree));
            
        } else if (strcmp(cmd, "add") == 0) {
            long long weight;
            char      part2[512];
            int       part2size = sizeof(part2) / sizeof(part2[0]);
            
            // Add weight text:
            
            scanf("%lld %s", &weight, text);
            fgets(part2, part2size, stdin);
            part2[strcspn(part2, "\r\n")] = '\0';  // strip EOL char(s):
            strcat(text, part2);
            
            // Add to the BST tree:
            
            BSTValue value;
            value.weight = weight;
            value.text = (char *)malloc(((int)strlen(text) + 1) * sizeof(char));
            strcpy(value.text, text);
            BSTKey key = value.text;

            if(BSTInsert(tree, key, value)) {
                printf("**Added\n");
            } else {
                printf("**Not added...\n");
            }
            
        } else if (strcmp(cmd, "find") == 0) {
            char part2[512];
            int  part2size = sizeof(part2) / sizeof(part2[0]);
            
            // Find text:

            scanf("%s", text);
            fgets(part2, part2size, stdin);
            part2[strcspn(part2, "\r\n")] = '\0';  // strip EOL char(s):
            strcat(text, part2);

            // Print results:
            
            BSTNode *searchedValue = BSTSearch(tree, text);
            if (searchedValue != NULL) {
                printf("%s: %lld\n", searchedValue->Value.text, searchedValue->Value.weight);
            } else {
                printf("**Not found...\n");
            }
        } else if (strcmp(cmd, "suggest") == 0) {
            int  k;
            char part2[512];
            int  part2size = sizeof(part2) / sizeof(part2[0]);
            
            // Suggest k text:
            
            scanf("%d %s", &k, text);
            fgets(part2, part2size, stdin);
            part2[strcspn(part2, "\r\n")] = '\0';  // strip EOL char(s):
            strcat(text, part2);
            
            // TODO:
            
            //Find subtree:
            BSTNode *subtree = BSTSearchSubstrings(tree, text);
            int subtreeCount = BSTSubtreeCount(subtree);
            
            int matchedCount = 0;
            BSTValue *matchedValues = BSTMatchSubstrings(subtree, text, subtreeCount, &matchedCount);
            
            if(subtree != NULL) {
                printf("%-20s", "** [Sub-tree root:");
                printf("(%s,%lld)]\n", subtree->Value.text, subtree->Value.weight);
                printf("%-20s", "** [Sub-tree count:");
                printf("%d]\n", subtreeCount);
                printf("%-20s", "** [Num matches:");
                printf("%d]\n", matchedCount);
                
                if(k > matchedCount) {
                    k = matchedCount;
                }
                
                int i = 0;
                for(i = 0; i < k; i++) {
                    printf("%s: %lld\n", matchedValues[i].text, matchedValues[i].weight);
                }
                
            } else {
                printf("**No suggestions...\n");
            }

        } else {
            printf("**unknown cmd, try again...\n");
        }
        
        scanf("%s", cmd);
    }
    
    printf("** Done **\n");
    
    return 0;
}
