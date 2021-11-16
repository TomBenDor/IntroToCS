/***********************
* Tom Ben-Dor
*
* 01
* ex5
***********************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define PRINT_ERROR_IF(cond, msg) if (cond) {puts(msg); return;}
#define MAX_CLASSES 5
#define STUDENT_NAME_MAX_LENGTH 20
#define COURSE_TITLE_MAX_LENGTH 30
#define COURSE_ID_LENGTH 5
#define INPUT_MAX_LENGTH 200

typedef int (*Validator)(int);

typedef struct Node {
    void *value;
    struct Node *next;
} Node;

typedef struct {
    Node *head;
    Node *tail;
    int size;
} LinkedList;

typedef struct {
    char name[STUDENT_NAME_MAX_LENGTH + 1];
    int grade;
} Student;

typedef struct {
    char id[COURSE_ID_LENGTH + 1];
    char title[COURSE_TITLE_MAX_LENGTH + 1];

    LinkedList *students;
} Course;

typedef enum {
    FALSE, TRUE
} bool;

typedef enum {
    FAILURE, SUCCESS
} result;

/******************
 * Function Name: newNode
 * Input: void *value (a pointer a value that will be in the node).
 * Output: Node *
 * Function Operation: The function allocates memory for a node and returns the initialized node.
******************/
Node *newNode(void *value) {
    // Allocating memory.
    Node *node = (Node *) malloc(sizeof(Node));
    if (node == NULL)
        return NULL;
    // Setting attributes.
    node->value = value;
    node->next = NULL;

    return node;
}

/******************
 * Function Name: newList
 * Input:
 * Output: LinkedList *
 * Function Operation: The function allocates memory for a list and returns the initialized list.
******************/
LinkedList *newList() {
    // Allocating memory.
    LinkedList *list = (LinkedList *) malloc(sizeof(LinkedList));
    Node *head = newNode(NULL);
    if ((list == NULL) || (head == NULL))
        return NULL;
    // Setting attributes.
    list->head = list->tail = head;
    list->size = 0;

    return list;
}

/******************
 * Function Name: newCourse
 * Input: char *courseId (5 digits),
 *        char courseTitle (up to 30 characters; letters, numbers and spaces only).
 * Output: Course *
 * Function Operation: The function allocates memory for a course,
 *                     and returns the initialized course with the parameters as attributes.
******************/
Course *newCourse(char *courseId, char *courseTitle) {
    // Allocating memory.
    Course *course = (Course *) malloc(sizeof(Course));
    if (course == NULL)
        return NULL;
    // Setting attributes.
    strcpy(course->title, courseTitle);
    strcpy(course->id, courseId);
    course->students = newList();
    if (course->students == NULL)
        return NULL;

    return course;
}

/******************
 * Function Name: newStudent
 * Input: char *studentName (up to 20 characters; letters and spaces only.),
 *        int studentGrade (a valid grade in the range 0-100).
 * Output: Student *
 * Function Operation: The function allocates memory for a student,
 *                     and returns the initialized student with the parameters as attributes.
******************/
Student *newStudent(char *studentName, int studentGrade) {
    // Allocating memory.
    Student *student = (Student *) malloc(sizeof(Student));
    if (student == NULL)
        return NULL;
    // Setting attributes.
    strcpy(student->name, studentName);
    student->grade = studentGrade;

    return student;
}

/******************
 * Function Name: freeNode
 * Input: Node *node (a pointer to a node).
 * Output: void
 * Function Operation: The function uses recursion to free all nodes that are linked to the given node.
******************/
void freeNode(Node *node) {
    if (node == NULL)
        return;
    // Freeing all nodes after the node.
    freeNode(node->next);
    // Freeing the node.
    free(node->value);
    free(node);
}

/******************
 * Function Name: freeLinkedList
 * Input: LinkedList *list (initialized linked list created by newList).
 * Output: void
 * Function Operation: The function calls freeNode to free all nodes and then frees the list.
******************/
void freeLinkedList(LinkedList *list) {
    freeNode(list->head);
    free(list);
}

/******************
 * Function Name: freeCourses
 * Input: LinkedList *courses (initialized linked list of courses created by newList).
 * Output: void
 * Function Operation: The function frees a courses list.
 *                     By freeing the students list in every course and then freeing the courses list.
******************/
void freeCourses(LinkedList *courses) {
    // Freeing the students lists.
    if (courses->size > 0) {
        for (Node *node = courses->head; node != NULL; node = node->next)
            freeLinkedList(((Course *) node->value)->students);
    }

    // Freeing courses, nodes and the courses list.
    freeLinkedList(courses);
}

/******************
 * Function Name: appendToList
 * Input: LinkedList *list (initialized linked list created by newList),
 *        void *value (a value to be appended to the list).
 * Output: result (FAILED or SUCCEEDED).
 * Function Operation: The function creates a new node and initializes it with the value.
 *                     Then, it sets the tail's next node to point to the new node and updates the tail.
******************/
result appendToList(LinkedList *list, void *value) {
    if (value == NULL)
        exit(1);
    // Increasing the SIZE attribute.
    list->size++;
    // If the list is empty, editing the empty node.
    if (list->head->value == NULL) {
        list->head->value = value;
        return SUCCESS;
    }

    // The list isn't empty, creating new node.
    Node *node = newNode(value);
    if (node == NULL)
        return FAILURE;
    // Updating the TAIL attribute to save the last node in the list.
    list->tail->next = node;
    list->tail = node;
    return SUCCESS;
}

/******************
 * Function Name: removeFromList
 * Input: LinkedList *list (initialized linked list created by newList),
 *        void *value (a value to be removed from the list).
 * Output: result (SUCCESS or FAILURE).
 * Function Operation: At first, the function removes the value from the head of the list.
 *                     Then, it iterates the values in the list and removes the value (if found)
 *                     by changing the 'next' attribute of node before it.
******************/
result removeFromList(LinkedList *list, void *value) {
    // Saving the previous node.
    Node *prev = list->head;
    // Iterating the list from the second node.
    Node *node = list->head->next;
    while (node != NULL) {
        // Found matching node.
        if (node->value == value) {
            // Updating the previous node by skipping the matching node.
            prev->next = node->next;
            // Freeing the node.
            free(node->value);
            free(node);
            list->size--;
        } else {
            prev = node;
        }
        node = prev->next;
    }
    // If the first node's value is equals to the parameter, removing it.
    if (list->head->value == value) {
        Node *temp = list->head;
        list->head = list->head->next != NULL ? list->head->next : newNode(NULL);
        if (list->head == NULL)
            return FAILURE;
        free(temp->value);
        free(temp);
        list->size--;
    }
    return SUCCESS;
}

/******************
 * Function Name: initSubstring
 * Input: char *substring (a destination for the substring).
 *        const char *string (a string),
 *        int start, int end (indexes in the range 0-len(string)).
 * Output: void
 * Function Operation: The function initializes the substring with the characters between start to end.
******************/
void initSubstring(char *substring, const char *string, int start, int end) {
    for (int i = 0; i < end - start; ++i)
        substring[i] = string[start + i];
    substring[end] = '\0';
}

/******************
 * Function Name: searchCourseById
 * Input: LinkedList *courses (initialized linked list of courses created by newList),
 *        char *id (a course id).
 * Output: Course *
 * Function Operation: The function iterates the list and searches for a course matching the id.
 *                     If found, it returns a pointer to the course, otherwise it returns NULL.
******************/
Course *searchCourseById(LinkedList *courses, char *id) {
    for (Node *node = courses->head; node != NULL; node = node->next) {
        if ((node->value != NULL) && (strcmp(((Course *) node->value)->id, id) == 0))
            return (Course *) node->value;
    }
    return NULL;
}

/******************
 * Function Name: searchStudentByName
 * Input: LinkedList *students (initialized linked list of students created by newList),
 *        char *studentName (a student name).
 * Output: Student *
 * Function Operation: The function iterates the list and searches for a student matching the name.
 *                     If found, it returns a pointer to the student, otherwise it returns NULL.
******************/
Student *searchStudentByName(LinkedList *students, char *studentName) {
    for (Node *node = students->head; node != NULL; node = node->next) {
        if ((node->value != NULL) && (strcmp(((Student *) node->value)->name, studentName) == 0))
            return (Student *) node->value;
    }
    return NULL;
}

/******************
 * Function Name: isValidInput
 * Input: char *input (a string),
 *        Validator validators[] (an array of validators; each character must pass at least one of those validators).
 * Output: bool (True or False).
 * Function Operation: The function uses recursion to iterate the input.
 *                     It returns True if every character passes at least one of the validators.
******************/
bool isValidInput(char *input, Validator validators[], int numberOfValidators) {
    // If the input is an empty string, then the input is valid.
    if (strlen(input) == 0)
        return TRUE;
    // Looping through the validators.
    for (int j = 0; j < numberOfValidators; ++j) {
        // If a character passes the validator, continuing to the next char.
        if (validators[j](*input))
            return isValidInput(++input, validators, numberOfValidators);
    }
    // Found an invalid character, return False.
    return FALSE;
}

/******************
 * Function Name: removeLeadingSpaces
 * Input: char **string (a pointer to a string).
 * Output: void
 * Function Operation: The function sets the pointer to point to the first non-space character in the string.
******************/
void removeLeadingSpaces(char **string) {
    while (isspace(**string))
        (*string)++;
}

/******************
 * Function Name: removeTrailingSpaces
 * Input: char *string (a string).
 * Output: void
 * Function Operation: The function changes every space at the end of the string to \0.
******************/
void removeTrailingSpaces(char *string) {
    while (strlen(string) && isspace(string[strlen(string) - 1]))
        string[strlen(string) - 1] = '\0';
}

/******************
 * Function Name: insertOrUpdateCourse
 * Input: LinkedList *courses (initialized linked list of courses created by newList).
 * Output: void
 * Function Operation: The function gets input from the user and check if it's valid.
 *                     If the input is valid, the function creates a new course or updates the requested course.
******************/
void insertOrUpdateCourse(LinkedList *courses) {
    const char ERROR_MSG[] = "Error: invalid class number or name.";

    // Getting the course id and title.
    char courseTitle[INPUT_MAX_LENGTH + 1] = "", courseId[INPUT_MAX_LENGTH + 1] = "", temp;
    scanf(" %[^ \n]%c", courseId, &temp);
    PRINT_ERROR_IF(temp != ' ', ERROR_MSG)
    scanf("%[^\n]%*c", courseTitle);
    char *courseTitlePtr = courseTitle;
    removeLeadingSpaces(&courseTitlePtr);

    // Validating the length of the id the title as well as making sure they consists of valid letters.
    PRINT_ERROR_IF((strlen(courseTitlePtr) == 0) || (strlen(courseTitlePtr) > COURSE_TITLE_MAX_LENGTH), ERROR_MSG)
    PRINT_ERROR_IF(strlen(courseId) != COURSE_ID_LENGTH, ERROR_MSG)
    Validator idValidators[] = {isdigit};
    PRINT_ERROR_IF(!(isValidInput(courseId, idValidators, sizeof(idValidators) / sizeof(Validator))), ERROR_MSG)
    Validator titleValidators[] = {isalnum, isspace};
    PRINT_ERROR_IF(!(isValidInput(courseTitlePtr, titleValidators, sizeof(titleValidators) / sizeof(Validator))),
                   ERROR_MSG)

    // If there course already exists, updating the title.
    if (searchCourseById(courses, courseId) != NULL) {
        strcpy(searchCourseById(courses, courseId)->title, courseTitlePtr);
        printf("Class \"%s %s\" updated.\n", courseId, courseTitlePtr);
    } else {
        // The course id is unique, checking if the list is full.
        PRINT_ERROR_IF(courses->size == MAX_CLASSES, "Unfortunately, there is no room for more classes.")

        // Inserting a new course to the list.
        if (appendToList(courses, newCourse(courseId, courseTitlePtr)) == FAILURE) {
            freeCourses(courses);
            exit(1);
        }
        printf("Class \"%s %s\" added.\n", courseId, courseTitlePtr);
    }
}

/******************
 * Function Name: getStudentName
 * Input: char studentName[] (an empty string).
 * Output: result (SUCCESS or FAILURE).
 * Function Operation: The function searches for a name in the following format: "<possible-spaces><name>:" in STDIN.
 *                     If found, it returns SUCCESS and initializes studentName with the name.
 *                     otherwise, it returns FAILURE and initializes studentName with the entire line of input.
******************/
result getStudentName(char studentName[]) {
    char temp;
    scanf(" %[^:\n]%c", studentName, &temp);
    return (temp == ':') || (strlen(studentName) == 0);
}

/******************
 * Function Name: insertOrUpdateStudent
 * Input: Course *requestedCourse (a course to insert the student to),
 *        char *studentName (the student's name),
 *        int studentGrade (the student's grade).
 * Output: result (SUCCESS or FAILURE).
 * Function Operation: The function checks if the student is already in the course.
 *                     If so, it updates the grade. Otherwise, it inserts the new student.
******************/
result insertOrUpdateStudent(Course *requestedCourse, char *studentName, int studentGrade) {
    // Looking for the student in the course.
    Student *student = searchStudentByName(requestedCourse->students, studentName);
    // If the student was found, updating the grade.
    if (student != NULL) {
        student->grade = studentGrade;
        printf("Student \"%s\" updated on class \"%s %s\" with grade %d.\n",
               studentName, requestedCourse->id, requestedCourse->title, studentGrade);
    } else {
        // The student is new, inserting to the list.
        if (appendToList(requestedCourse->students, newStudent(studentName, (int) studentGrade)) == FAILURE)
            return FAILURE;
        printf("Student \"%s\" added to class \"%s %s\" with grade %d.\n",
               studentName, requestedCourse->id, requestedCourse->title, studentGrade);
    }
    return SUCCESS;
}

/******************
 * Function Name: assignStudentToCourses
 * Input: LinkedList *courses (initialized linked list of courses created by newList).
 * Output: void
 * Function Operation: The function gets input from the user and check if it's valid.
 *                     If the input is valid, the function iterates the courses and grades in the input
 *                     and assigning the student using insertOrUpdateStudent().
******************/
void assignStudentToCourses(LinkedList *courses) {
    const char ERROR_MSG[] = "Error: invalid name, class number or grade.";

    // Getting the student name and requested courses and grades.
    char studentName[INPUT_MAX_LENGTH + 1] = "", coursesAndGrades[INPUT_MAX_LENGTH + 1];
    PRINT_ERROR_IF(getStudentName(studentName) == FAILURE, ERROR_MSG)
    scanf("%[^\n]%*c", coursesAndGrades);
    removeTrailingSpaces(coursesAndGrades);
    // Checking if the format of the name and the list of grades is valid.
    PRINT_ERROR_IF(coursesAndGrades[strlen(coursesAndGrades) - 1] != ';', ERROR_MSG)
    Validator nameValidators[] = {isalpha, isspace};
    PRINT_ERROR_IF(strlen(studentName) > STUDENT_NAME_MAX_LENGTH, ERROR_MSG)
    PRINT_ERROR_IF(!(isValidInput(studentName, nameValidators, sizeof(nameValidators) / sizeof(Validator))),
                   ERROR_MSG)
    // Creating a copy of coursesAndGrades because strtok modifies the string.
    char temp[INPUT_MAX_LENGTH + 1];
    strcpy(temp, coursesAndGrades);
    // Using strtok to iterate each course id and grade.
    char *token = strtok(temp, ";");
    PRINT_ERROR_IF(token == NULL, ERROR_MSG)
    for (; token != NULL; token = strtok(NULL, ";")) {
        removeLeadingSpaces(&token);
        // Making sure that the pair of course id and grade is in a legal format.
        PRINT_ERROR_IF((strlen(token) < strlen("89110,1")) || (token[COURSE_ID_LENGTH] != ','), ERROR_MSG)
        char courseId[COURSE_ID_LENGTH + 1] = "", grade[INPUT_MAX_LENGTH + 1] = "", *left;
        // Getting the id.
        initSubstring(courseId, token, 0, COURSE_ID_LENGTH);
        // Verifying that the course exists.
        PRINT_ERROR_IF(searchCourseById(courses, courseId) == NULL, ERROR_MSG)
        // Getting the grade.
        initSubstring(grade, token, COURSE_ID_LENGTH + 1, strlen(token));
        // Verifying that the grade is valid (an integer between 0 - 100).
        Validator gradeValidators[] = {isdigit, isspace};
        PRINT_ERROR_IF(!(isValidInput(grade, gradeValidators, sizeof(gradeValidators) / sizeof(Validator))),
                       ERROR_MSG)
        long studentGrade = strtol(grade, &left, 10);
        PRINT_ERROR_IF(strlen(left) || (studentGrade < 0) || (studentGrade > 100), ERROR_MSG)
    }

    // Now we know for sure that the format it legal, iterating again and updating the students' lists.
    for (token = strtok(coursesAndGrades, ";"); token != NULL; token = strtok(NULL, ";")) {
        removeLeadingSpaces(&token);
        // Getting the course id and the grade.
        char courseId[COURSE_ID_LENGTH + 1] = "", grade[INPUT_MAX_LENGTH + 1] = "";
        initSubstring(courseId, token, 0, COURSE_ID_LENGTH);
        initSubstring(grade, token, COURSE_ID_LENGTH + 1, strlen(token));
        int studentGrade = (int) strtol(grade, NULL, 10);
        // Inserting the student and exiting the program if a malloc failed.
        if (insertOrUpdateStudent(searchCourseById(courses, courseId), studentName, studentGrade) == FAILURE) {
            freeCourses(courses);
            exit(1);
        }
    }
}

/******************
 * Function Name: countCoursesByStudentName
 * Input: LinkedList *courses (initialized linked list of courses created by newList),
 *        char *studentName (a student name).
 * Output: int
 * Function Operation: The function iterates the courses and searches for the student in each course.
 *                     It returns the number of courses the student is assigned to.
******************/
int countCoursesByStudentName(LinkedList *courses, char *studentName) {
    int count = 0;
    for (Node *node = courses->head; node != NULL; node = node->next) {
        // Checking if the student is a member of this course.
        if (searchStudentByName(((Course *) node->value)->students, studentName) != NULL)
            count++;
    }
    return count;
}

/******************
 * Function Name: removeStudentFromCourses
 * Input: LinkedList *courses (initialized linked list of courses created by newList).
 * Output: void
 * Function Operation: The function gets input from the user and check if it's valid.
 *                     If the input is valid, the function iterates the courses in the input and removing the student.
******************/
void removeStudentFromCourses(LinkedList *courses) {
    const char ERROR_MSG[] = "Error: invalid name or class number.";

    // Getting the student name and list of requested courses.
    char studentName[INPUT_MAX_LENGTH + 1] = "", requestedCourses[INPUT_MAX_LENGTH + 1] = "";
    PRINT_ERROR_IF(getStudentName(studentName) == FAILURE, ERROR_MSG)
    scanf("%[^\n]%*c", requestedCourses);
    removeTrailingSpaces(requestedCourses);
    // Checking if the format of the name and the list of courses is valid.
    PRINT_ERROR_IF(requestedCourses[strlen(requestedCourses) - 1] != ',', ERROR_MSG)
    int numberOfCourses = countCoursesByStudentName(courses, studentName);
    // If the requested student is not assigned to any course or there are no courses, printing error.
    PRINT_ERROR_IF((numberOfCourses == 0) || (courses->size == 0), ERROR_MSG)

    // Creating a copy of requestedCourses because strtok modifies the string.
    char temp[INPUT_MAX_LENGTH + 1];
    strcpy(temp, requestedCourses);
    int countCourses = 0;
    // Using strtok to iterate the requested courses and counting them.
    for (char *token = strtok(temp, ","); token != NULL; token = strtok(NULL, ",")) {
        removeLeadingSpaces(&token);
        // Verifying that the course exists.
        Course *requestedCourse = searchCourseById(courses, token);
        PRINT_ERROR_IF(requestedCourse == NULL, ERROR_MSG)
        // Verifying that the student is a member of the requested course.
        if (searchStudentByName(requestedCourse->students, studentName) == NULL) {
            printf("Error: student is not a member of class \"%s %s\".\n",
                   requestedCourse->id, requestedCourse->title);
            return;
        }
        countCourses++;
    }
    PRINT_ERROR_IF(countCourses == 0, ERROR_MSG)
    // If the user is trying to remove a student from all courses, printing an error.
    PRINT_ERROR_IF(numberOfCourses == countCourses, "Error: student cannot be removed from all classes.")

    // Now we know for sure that the input is valid, removing the student.
    for (char *token = strtok(requestedCourses, ","); token != NULL; token = strtok(NULL, ",")) {
        removeLeadingSpaces(&token);
        Course *course = searchCourseById(courses, token);
        // Removing the student and exiting the program if a malloc failed.
        if (removeFromList(course->students, searchStudentByName(course->students, studentName)) == FAILURE) {
            freeCourses(courses);
            exit(1);
        }
        printf("Student \"%s\" removed from class \"%s %s\".\n", studentName, course->id, course->title);
    }
}

/******************
 * Function Name: nameComparator
 * Input: Student *student1, Student *student2 (Tho students to compare).
 * Output: int
 * Function Operation: The function uses strcmp to compare the students' names.
******************/
int nameComparator(Student *student1, Student *student2) {
    return strcmp(student1->name, student2->name);
}

/******************
 * Function Name: bubbleSort
 * Input: LinkedList *list (initialized linked list created by newList).
 * Output: void.
 * Function Operation: The function uses the bubble sort algorithm to sort the list in increasing order.
******************/
void bubbleSort(LinkedList *list, int(*comparator)()) {
    for (Node *i = list->head; i && i->next; i = i->next) {
        for (Node *j = i->next; j != NULL; j = j->next) {
            if (comparator(i->value, j->value) > 0) {
                // Swapping two nodes.
                void *temp = j->value;
                j->value = i->value;
                i->value = temp;
            }
        }
    }
}

/******************
 * Function Name: printCourse
 * Input: LinkedList *courses (initialized linked list of courses created by newList).
 * Output: void
 * Function Operation: The function scans for input and validating the course id.
 *                     Then, it uses bubbleSort() to sort the students list and prints the students.
******************/
void printCourse(LinkedList *courses) {
    char ERROR_MSG[] = "Error: invalid class number.";

    // Getting the course id and verifying that the course exists.
    char courseId[INPUT_MAX_LENGTH + 1];
    scanf(" %[^\n]%*c", courseId);
    Course *requestedCourse = searchCourseById(courses, courseId);
    PRINT_ERROR_IF(requestedCourse == NULL, ERROR_MSG)

    // Printing if the requested course has no students.
    if (requestedCourse->students->size == 0) {
        printf("Class \"%s %s\" has no students.\n", requestedCourse->id, requestedCourse->title);
        return;
    }

    // Sorting the students list.
    bubbleSort(requestedCourse->students, nameComparator);

    // Printing students.
    printf("Class \"%s %s\" students:\n", requestedCourse->id, requestedCourse->title);
    for (Node *node = requestedCourse->students->head; node != NULL; node = node->next) {
        Student *student = node->value;
        printf("%s, %d\n", student->name, student->grade);
    }
}

/******************
 * Function Name: getSumOfGrades
 * Input: LinkedList *students (initialized linked list of students created by newList).
 * Output: int
 * Function Operation: The function sums all the grades and returns the sum.
******************/
int getSumOfGrades(LinkedList *students) {
    int sum = 0;
    for (Node *node = students->head; node != NULL; node = node->next)
        sum += ((Student *) node->value)->grade;
    return sum;
}

/******************
 * Function Name: getMinMax
 * Input: LinkedList *students (initialized linked list of students created by newList),
 *        char *option ('<' for minimum, '>' for maximum).
 * Output: int
 * Function Operation: The function iterates the students' grades and compares them with the maximum / minimum,
 *                     by saving the largest / smallest number that was found so far.
******************/
int getMinMax(LinkedList *students, char option) {
    // Saving the first students' grade.
    int result = ((Student *) students->head->value)->grade;
    for (Node *node = students->head->next; node != NULL; node = node->next) {
        int grade = ((Student *) node->value)->grade;
        // If we find a bigger or smaller grade (depends on the option), updating the result.
        if ((option == '>') ? (grade > result) : (grade < result))
            result = grade;
    }
    return result;
}

/******************
 * Function Name: printAllCourses
 * Input: LinkedList *courses (initialized linked list of courses created by newList).
 * Output: void
 * Function Operation: The function prints a menu and gets the user choice.
 *                     Then, it iterates the courses and prints each calculation according to the choice.
******************/
void printAllCourses(LinkedList *courses) {
    PRINT_ERROR_IF(courses->size == 0, "Error: there are no classes.")

    puts("Please select the aggregation method:\n"
         "\ta. Average Grade.\n"
         "\tb. Maximal Grade.\n"
         "\tc. Minimal Grade.\n"
         "\td. Sum.\n"
         "\te. Count.\n"
         "\t0. Return to the main menu.");
    // Getting the choice.
    char choice;
    scanf("%c%*c", &choice);
    if (choice == '0')
        return;

    // Iterating the courses and printing the calculation for each one.
    for (Node *node = courses->head; node != NULL; node = node->next) {
        Course *course = node->value;
        // Checking if the course has students.
        if (course->students->size == 0) {
            printf("Class \"%s %s\" has no students.\n", course->id, course->title);
            continue;
        }
        // Printing the calculation, 'default' is unnecessary because we know the input is legal.
        printf("%s %s, ", course->id, course->title);
        switch (choice) {
            case 'a':
                printf("%d\n", getSumOfGrades(course->students) / course->students->size);
                break;
            case 'b':
                printf("%d\n", getMinMax(course->students, '>'));
                break;
            case 'c':
                printf("%d\n", getMinMax(course->students, '<'));
                break;
            case 'd':
                printf("%d\n", getSumOfGrades(course->students));
                break;
            case 'e':
                printf("%d\n", course->students->size);
                break;
        }
    }
}

/******************
 * Function Name: printMenu
 * Input:
 * Output: void
 * Function Operation: The function prints the main menu.
******************/
void printMenu() {
    puts("Please select an operation:\n"
         "\t0. Exit.\n"
         "\t1. Insert or update a class.\n"
         "\t2. Insert or update a student.\n"
         "\t3. Remove a student from classes.\n"
         "\t4. Print a class.\n"
         "\t5. Print all classes with their computed scores.\n"
         "\t6. Print the menu.");
}

/******************
 * Function Name: runMenu
 * Input: LinkedList *courses (initialized linked list of courses created by newList).
 * Output: void
 * Function Operation: The function gets the user's choice and calling the requested function.
******************/
void runMenu(LinkedList *courses) {
    void (*actions[])(LinkedList *) = {insertOrUpdateCourse, assignStudentToCourses, removeStudentFromCourses,
                                       printCourse, printAllCourses};
    char choice;
    printMenu();
    while (1) {
        // Getting the choice.
        scanf(" %c%*c", &choice);
        if (choice == '0')
            break;

        if (choice == '6') {
            printMenu();
            continue;
        } else if (!((choice >= '1') && (choice <= '5'))) {
            // The choice is not supported, printing an error.
            puts("Error: unrecognized operation.");
        } else {
            // Calling the function.
            (*actions[choice - '1'])(courses);
        }
        puts("Select the next operation (insert 6 for the entire menu):");
    }
}