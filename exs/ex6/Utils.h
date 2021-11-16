/***********************
* Tom Ben-Dor
*
* 01
* ex6
***********************/

#include "ex6.h"

#ifndef _UTILS_H
#define _UTILS_H
#define PRINT_SPACES 3

void print2DUtil(Trin_Ari *root, int space);

void printPreOrder(Trin_Ari *root, const char *);

void printLeftOrder(Trin_Ari *root, const char *);

void printLevel(Trin_Ari *root, int level, const char *);

void printBFC(Trin_Ari *root, const char *);

#endif // !_UTILS_H

