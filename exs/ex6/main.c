/***********************
* Tom Ben-Dor
*
* 01
* ex6
***********************/

#include <stdlib.h>
#include <stdio.h>
#include "ex6.h"

int main_ex6() {
    // Creating a root.
    Trin_Ari *family = newTree(NULL, NULL);
    if (family == NULL) {
        puts("Malloc Failed!");
        exit(1);
    }
    // Running the menu.
    runMenu(family);
    // Freeing the tree.
    freeFamilyTree(family, freeMember);
    return 0;
}