/***********************
* Tom Ben-Dor
*
* 01
* ex5
***********************/

#ifndef _EX5_H
#define _EX5_H

typedef struct LinkedList LinkedList;

void runMenu(LinkedList *courses);

LinkedList *newList();

void freeCourses(LinkedList *courses);

#endif