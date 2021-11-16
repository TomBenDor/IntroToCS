/***********************
* Tom Ben-Dor
*
* 01
* ex5
***********************/

#include <stdlib.h>
#include "ex5.h"

int main_ex5() {
    // Creating a courses list.
    LinkedList *courses = newList();
    if (courses == NULL)
        exit(1);
    // Running the program.
    runMenu(courses);
    // Freeing allocated memory.
    freeCourses(courses);
    return 0;
}