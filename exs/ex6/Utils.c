/***********************
* Tom Ben-Dor
*
* 01
* ex6
***********************/

#include <stdio.h>
#include "Utils.h"

#define COUNT 8

// Function to print Trinary tree in 2D 
// It does reverse inorder traversal 
void print2DUtil(Trin_Ari *root, int space) {
    // Base case
    if (root == NULL)
        return;

    // Increase distance between levels
    space += COUNT;

    // Process right child first
    print2DUtil(root->right, space);
    for (int i = COUNT; i < space; i++)
        printf(" ");
    printf("%s %d\n", ((Member *) root->value)->name, ((Member *) root->value)->id);
    // Process middle child
    print2DUtil(root->middle, space);
    printf("\n");
    // Process left child
    print2DUtil(root->left, space);
}

/******************
 * Function Name: printPreOrder
 * Input: Trin_Ari *root (a tree to print),
 *        const char *format (a format to print the members).
 * Output: void
 * Function Operation: The function prints the tree in pre-order.
******************/
void printPreOrder(Trin_Ari *root, const char *format) {
    if (root == NULL)
        return;
    // Printing the root.
    Member *member = root->value;
    printf(format, member->name, member->id);
    // Printing branches.
    printPreOrder(root->left, format);
    printPreOrder(root->middle, format);
    printPreOrder(root->right, format);
}

/******************
 * Function Name: printLeftOrder
 * Input: Trin_Ari *root (a tree to print),
 *        const char *format (a format to print the members).
 * Output: void
 * Function Operation: The function prints the tree in left-order.
******************/
void printLeftOrder(Trin_Ari *root, const char *format) {
    if (root == NULL)
        return;
    // Printing the left branch.
    printLeftOrder(root->left, format);
    // Printing the root.
    Member *member = root->value;
    printf(format, member->name, member->id);
    // Printing middle and right branches.
    printLeftOrder(root->middle, format);
    printLeftOrder(root->right, format);
}

/******************
 * Function Name: printLevel
 * Input: Trin_Ari *root (a tree to print its level),
 *        int level (the level to print),
 *        const char *format (the format to print the members).
 * Output: void
 * Function Operation: The function uses recursion to print the requested level of the tree.
 *                     For example, let the tree be:
 *                         2
 *                     1
 *                         3
 *                     And the level is 2, it will print 2 and 3 (according to the format).
******************/
void printLevel(Trin_Ari *root, int level, const char *format) {
    // If the root is NULL there are no members, returning.
    if (root == NULL)
        return;
    // If the level is one, printing the root.
    if (level == 1) {
        Member *member = root->value;
        printf(format, member->name, member->id);
        return;
    }
    // Printing level - 1 of the children, which is level to the root.
    printLevel(root->left, level - 1, format);
    printLevel(root->middle, level - 1, format);
    printLevel(root->right, level - 1, format);
}

/******************
 * Function Name: max
 * Input: int a, int b, int c (numbers).
 * Output: int
 * Function Operation: The function returns the largest number among a, b & c.
******************/
static int max(int a, int b, int c) {
    int max = a;
    if (b > max)
        max = b;
    if (c > max)
        max = c;
    return max;
}

/******************
 * Function Name: countLevels
 * Input: Trin_Ari *root (a tree to count its levels).
 * Output: int
 * Function Operation: The function uses recursion to return how deep is the tree.
******************/
static int countLevels(Trin_Ari *root) {
    // If the tree is NULL, returning 0.
    if (root == NULL)
        return 0;
    // Evaluating how deep is each branch.
    int left = countLevels(root->left);
    int middle = countLevels(root->middle);
    int right = countLevels(root->right);
    // Returning the maximum dip of the branches and adding 1.
    return max(left, middle, right) + 1;
}

/******************
 * Function Name: printBFC
 * Input: Trin_Ari *root (a tree to print),
 *        const char *format (a format to print the members).
 * Output: void
 * Function Operation: The function prints the first level, then the second, ... (until the end of the tree).
******************/
void printBFC(Trin_Ari *root, const char *format) {
    for (int i = 1; i <= countLevels(root); i++)
        printLevel(root, i, format);
}