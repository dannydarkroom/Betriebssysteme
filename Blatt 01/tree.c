#include <stdio.h>

#include "tree.h"

node_t *createTree(int rootValue) {
    node_t *tree;

    if (!(tree = malloc(sizeof(struct _node_t)))) {
        printf("ERROR: Block of size %lu could not be allocated!\n", 
                sizeof(struct _node_t));
    } 

    tree->value = rootValue;
    tree->parent = NULL;
    tree->left = NULL;
    tree->right = NULL;
    
    return tree;
}
        
    

// insert value in tree
void insert(node_t *tree, int value) {
    node_t *curTree = tree; 
    node_t *parentTree = tree;
    // iterate through tree
    while(curTree) {
        if (curTree->value < value) {
            parentTree = curTree;
            curTree = curTree->right;
        } else if (curTree->value > value) {
            parentTree = curTree;
            curTree = curTree->left;
        } else {
            return;
        }
    }
    
    // allocate memory for new tree
    node_t *newTree;
    if (!(newTree = malloc(sizeof(struct _node_t)))) {
        printf("ERROR: Block of size %lu could not be allocated!\n", 
             sizeof(struct _node_t));
    }

    // initialize new tree
    newTree->value = value;
    newTree->parent = parentTree;
    newTree->left = NULL;
    newTree->right = NULL;

    // insert new tree into right position
    if (parentTree->value < value) {
        parentTree->right = newTree;
    } else {
        parentTree->left = newTree;
    }
}

// return 1 iff value is found in tree, 0 otherwise
int binarySearch(node_t *tree, int value) {
    // Iterative Methode
/*    node_t *curTree = tree;
    while(curTree) {
            if (curTree->value < value) {
                curTree = curTree->right;
            } else if (curTree->value > value) {
                curTree = curTree->left;
            } else {
                return 1;
            }
    }

    return 0; 
*/
    //Recursive Methode 2.0
    if (!tree) {
        return 0;
    }

    if (tree->value > value) {
        return binarySearch(tree->left, value);
    }
    else if (tree->value < value) {
        return binarySearch(tree->right, value);
    }

    return 1;
    // Recursive Methode
    /* if (tree->value < value) {
        if (tree->right) {
            return binarySearch(tree->right, value);
        } else {
            return 0;
        }
    } else if (tree->value > value) {
        if (tree->left) {
            return binarySearch(tree->left, value);
        } else {
            return 0;
        }
    } else {
        return 1;
    } */
}

// free all dynamically allocated memory of tree
void cleanUpTree(node_t *tree) {
    if (tree->left) {
        cleanUpTree(tree->left);
    } 

    if (tree->right) {
        cleanUpTree(tree->right);
    }

    free(tree); 
}

