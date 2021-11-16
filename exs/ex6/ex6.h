/***********************
* Tom Ben-Dor
*
* 01
* ex6
***********************/

#ifndef _EX6_H_
#define _EX6_H_
#define MEMBER_NAME_MAX_LENGTH 50


typedef struct {
    int id;
    char *name;
} Member;

typedef struct Trin_Ari {
    void *value;
    struct Trin_Ari *parent;
    struct Trin_Ari *left;
    struct Trin_Ari *middle;
    struct Trin_Ari *right;
} Trin_Ari;

Trin_Ari *newTree(Trin_Ari *parent, void *value);

void runMenu(Trin_Ari *family);

void freeMember(void *member);

void freeFamilyTree(Trin_Ari *root, void(*freeValue)(void *));

#endif