/***********************
* Tom Ben-Dor
*
* 01
* ex6
***********************/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "ex6.h"
#include "Utils.h"

#define PRINT_ERROR_IF(cond, msg) if (cond) {puts(msg); return;}


/******************
 * Function Name: freeMember
 * Input: Member *member (a member created by newMember()).
 * Output: void
 * Function Operation: The function frees the member.
******************/
void freeMember(void *member) {
    if (member == NULL)
        return;
    free(((Member *) member)->name);
    free(member);
}

/******************
 * Function Name: newTree
 * Input: Trin_Ari *parent (a parent for the new tree),
 *        void *value (a pointer to the value (in our case Member).
 * Output: Trin_Ari *
 * Function Operation: The function allocates a new tree node and sets it's parent and value to given parameters.
******************/
Trin_Ari *newTree(Trin_Ari *parent, void *value) {
    // Allocating memory.
    Trin_Ari *tree = (Trin_Ari *) malloc(sizeof(Trin_Ari));
    if (tree == NULL)
        return NULL;
    // Setting attributes.
    tree->parent = parent;
    tree->left = tree->middle = tree->right = NULL;
    tree->value = value;
    return tree;
}

/******************
 * Function Name: newMember
 * Input: char *name (the new member's name),
 *        int id (the new member's id).
 * Output: Member *
 * Function Operation: The function allocates a new member and sets it's name and id to given parameters.
******************/
static Member *newMember(char *name, int id) {
    // Allocating memory.
    Member *member = (Member *) malloc(sizeof(Member));
    if (member == NULL)
        return NULL;
    // Setting attributes.
    member->name = (char *) malloc(sizeof(char) * MEMBER_NAME_MAX_LENGTH);
    if (member->name == NULL)
        return NULL;
    strcpy(member->name, name);
    member->id = id;
    return member;
}

/******************
 * Function Name: searchTreeByMemberId
 * Input: Trin_Ari *root (a tree to search within),
 *        int id (an id).
 * Output: Trin_Ari *
 * Function Operation: The function uses recursion to go through the tree and search for matching node.
 *                     If found, it returns it, otherwise it return NULL.
******************/
static Trin_Ari *searchTreeByMemberId(Trin_Ari *root, int id) {
    // If the root is NULL, returning NULL.
    if (root == NULL)
        return NULL;
    // If found matching node, returning it.
    if (((Member *) root->value)->id == id)
        return root;
    // Searching in the left branch.
    Trin_Ari *matching;
    matching = searchTreeByMemberId(root->left, id);
    if (matching != NULL)
        return matching;
    // Searching in the middle branch.
    matching = searchTreeByMemberId(root->middle, id);
    if (matching != NULL)
        return matching;
    // Searching in the right branch.
    return searchTreeByMemberId(root->right, id);
}

/******************
 * Function Name: getNameAndId
 * Input: char *name (destination for the name),
 *        int *id (destination for the id).
 * Output: void
 * Function Operation: The function prints prompts and asks the user for name and id.
 *                     It initializes the name and id parameters with the input.
******************/
static void getNameAndID(char *name, int *id) {
    puts("enter name");
    scanf("%[^\n]%*c", name);
    puts("enter ID");
    scanf("%d%*c", id);
}

/******************
 * Function Name: createFamily
 * Input: Trin_Ari *family (a family created by newTree).
 * Output: void
 * Function Operation: The function verifies that the family wasn't created yet.
 *                     Then, it uses newMember() to allocate a new member and asks the user for name & id.
******************/
static void createFamily(Trin_Ari *family) {
    // Verifying that the family wasn't created yet.
    PRINT_ERROR_IF(family->value != NULL, "The family has already been created");
    // Getting name and id.
    char name[MEMBER_NAME_MAX_LENGTH] = "";
    int id;
    getNameAndID(name, &id);
    // Creating new member.
    family->value = newMember(name, id);
    // If malloc failed, freeing all memory and exiting the program.
    if (family->value == NULL) {
        puts("Malloc Failed!");
        freeFamilyTree(family, freeMember);
        exit(1);
    }
    printf("Hello %s, ID: %d\n", name, id);
}

/******************
 * Function Name: insertToTree
 * Input: Trin_Ari *tree (a tree created by newTree; has at least one empty child),
 *        Member *member (a member to insert).
 * Output: int (0 - succeeded, 1 - failed).
 * Function Operation: The function creates a new tree using newTree() with the member as value.
 *                     Then, the function inserts the tree to the first empty child according to this order:
 *                     left child, middle child, right child.
******************/
static int insertToTree(Trin_Ari *tree, Member *member) {
    // Creating a new node.
    Trin_Ari *newTreeNode = newTree(tree, member);
    // If malloc failed, returning 1.
    if (newTreeNode == NULL)
        return 1;
    // inserting to the first empty branch.
    if (tree->left == NULL)
        tree->left = newTreeNode;
    else if (tree->middle == NULL)
        tree->middle = newTreeNode;
    else if (tree->right == NULL)
        tree->right = newTreeNode;
    return 0;
}

/******************
 * Function Name: getRequestedMember
 * Input: Trin_Ari *family (a family created by newTree),
 *        char *prompt (the message asking the user for id),
 *        char *error (error message to print when member wasn't found.
 * Output: Trin_Ari *
 * Function Operation: The function uses the prompt to ask the user for an id.
 *                     It searches for the tree that has the entered id,
 *                     If found, it returns it, otherwise it returns NULL and prints the error.
******************/
static Trin_Ari *getRequestedMember(Trin_Ari *family, char *prompt, char *error) {
    // Getting id from user.
    int id;
    puts(prompt);
    scanf("%d%*c", &id);
    // Searching for the id in the tree.
    Trin_Ari *node = searchTreeByMemberId(family, id);
    // If wasn't found, printing error.
    if (node == NULL)
        printf(error, id);
    // Returning the node (can be NULL).
    return node;
}

/******************
 * Function Name: scheduleMeeting
 * Input: Trin_Ari *family (a family created by newTree).
 * Output: void
 * Function Operation: The function gets a member using getRequestedMember() and then asks the user for a new member.
 *                     Ones a unique member was received,
 *                     the function uses insertToTree() to insert it to the first member.
******************/
static void scheduleMeeting(Trin_Ari *family) {
    // Verifying that the family was created.
    PRINT_ERROR_IF(family->value == NULL, "Please create the family first");

    // Getting the member who wants to schedule.
    Trin_Ari *wantsToSchedule = getRequestedMember(family,
                                                   "Enter the ID of the person who wants to make an appointment?",
                                                   "There is no ID %d\n");
    if (wantsToSchedule == NULL)
        return;
    // Verifying that the member has an empty branch to insert into.
    if (wantsToSchedule->left && wantsToSchedule->middle && wantsToSchedule->right) {
        printf("%s ID: %d can't meet more than 3 members!\n",
               ((Member *) wantsToSchedule->value)->name, ((Member *) wantsToSchedule->value)->id);
        return;
    }

    // Getting name and id for the new member.
    char newMemberName[MEMBER_NAME_MAX_LENGTH] = "";
    int newMemberId;
    while (1) {
        getNameAndID(newMemberName, &newMemberId);
        Trin_Ari *tree = searchTreeByMemberId(family, newMemberId);
        // If member is unique, exiting the loop.
        if (tree == NULL)
            break;
        // Member with the same id was created, printing an error.
        printf("ID %d belongs to %s\n", ((Member *) tree->value)->id, ((Member *) tree->value)->name);
    }
    // Inserting to tree.
    Member *member = newMember(newMemberName, newMemberId);
    // If a malloc failed, freeing all memory and exiting the program.
    if ((member == NULL) || (insertToTree(wantsToSchedule, member))) {
        puts("Malloc Failed!");
        freeFamilyTree(family, freeMember);
        exit(1);
    }
    // Success! printing hello message.
    printf("Hello %s, ID: %d\n", member->name, member->id);
}

/******************
 * Function Name: printFamilyTree
 * Input: Trin_Ari *family (a family created by newTree).
 * Output: void
 * Function Operation: The function prints a menu and gets a choice from the user.
 *                     It then prints the family tree according to the choice and returns to the main menu.
******************/
static void printFamilyTree(Trin_Ari *family) {
    // Printing the menu.
    puts("Please select an operation:\n"
         "\t0. Return to the main menu.\n"
         "\t1. Print Trin-Ari family.\n"
         "\t2. Print Pre-order.\n"
         "\t3. Print Left-order.\n"
         "\t4. Print BFS.");

    // Getting the user's choice.
    char choice;
    scanf("%c%*c", &choice);
    // Checking if the choice is legal.
    if ((choice < '0') || (choice > '4')) {
        puts("Error: unrecognized operation.");
        printFamilyTree(family);
        return;
    }
    // If the family is empty or the choice is to leave, returning to main menu.
    if ((choice == '0') || (family->value == NULL))
        return;
    // Printing according to the user's choice.
    if (choice == '1') {
        print2DUtil(family, PRINT_SPACES);
        return;
    }
    void (*actions[])(Trin_Ari *, const char *) = {printPreOrder, printLeftOrder, printBFC};
    actions[choice - '2'](family, choice != '4' ? "%s ID: %d\n" : "%s %d\n");
}

/******************
 * Function Name: quarantine
 * Input: Trin_Ari *family (a family created by newTree).
 * Output: void
 * Function Operation: The function gets a member using getRequestedMember().
 *                     It prints #StayHome message including its parent then itself and then its children.
******************/
static void quarantine(Trin_Ari *family) {
    // Verifying that the family was created.
    PRINT_ERROR_IF(family->value == NULL, "Please create the family first");

    // Getting the member who is suspected.
    Trin_Ari *suspectedNode = getRequestedMember(family, "Enter the ID of the suspected member",
                                                 "ID %d does not exist\n");
    if (suspectedNode == NULL)
        return;
    // If that user has a parent, printing the parent.
    if (suspectedNode->parent != NULL) {
        Member *parent = suspectedNode->parent->value;
        printf("#StayHome! %s %d\n", parent->name, parent->id);
    }
    // Printing member.
    Member *member = suspectedNode->value;
    printf("#StayHome! %s %d\n", member->name, member->id);
    // Printing children.
    const int childrenLevel = 2;
    printLevel(suspectedNode, childrenLevel, "#StayHome! %s %d\n");
}

/******************
 * Function Name: superspreader
 * Input: Trin_Ari *family (a family created by newTree).
 * Output: void
 * Function Operation: The function gets a member using getRequestedMember().
 *                     It prints #StayHome message including its parent and it's entire tree in BFC order.
******************/
static void superspreader(Trin_Ari *family) {
    // Verifying that the family was created.
    PRINT_ERROR_IF(family->value == NULL, "Please create the family first");
    // Getting the member who is a superspreader.
    Trin_Ari *superspreaderNode = getRequestedMember(family, "Enter the ID of the suspected Superspreader",
                                                     "ID %d does not exist\n");
    if (superspreaderNode == NULL)
        return;
    // If that user has a parent, printing the parent.
    if (superspreaderNode->parent != NULL) {
        Member *parent = superspreaderNode->parent->value;
        printf("#StayHome! %s %d\n", parent->name, parent->id);
    }
    // Printing the entire tree with BFC order.
    printBFC(superspreaderNode, "#StayHome! %s %d\n");
}

/******************
 * Function Name: vaccine
 * Input: Trin_Ari *family (a family created by newTree).
 * Output: void
 * Function Operation: The function gets a member using getRequestedMember().
 *                     Then, it deletes the tree that its node is the member gotten.
******************/
static void vaccine(Trin_Ari *family) {
    // Verifying that the family was created.
    PRINT_ERROR_IF(family->value == NULL, "Please create the family first");
    // Getting the member who got vaccinated.
    Trin_Ari *vaccinatedNode = getRequestedMember(family, "Who got vaccinated (ID)?",
                                                  "There is no ID %d\n");
    if (vaccinatedNode == NULL)
        return;
    // If the member is the head of the family, removing all children and nullifying the node.
    if (vaccinatedNode->parent == NULL) {
        freeFamilyTree(vaccinatedNode->left, freeMember);
        freeFamilyTree(vaccinatedNode->middle, freeMember);
        freeFamilyTree(vaccinatedNode->right, freeMember);
        printf("%s ID: %d Survived!\n",
               ((Member *) vaccinatedNode->value)->name, ((Member *) vaccinatedNode->value)->id);
        freeMember(vaccinatedNode->value);
        vaccinatedNode->value = vaccinatedNode->left = vaccinatedNode->middle = vaccinatedNode->right = NULL;
        return;
    }

    // Setting the branch to NULL.
    if (vaccinatedNode->parent->left == vaccinatedNode)
        vaccinatedNode->parent->left = NULL;
    else if (vaccinatedNode->parent->middle == vaccinatedNode)
        vaccinatedNode->parent->middle = NULL;
    else
        vaccinatedNode->parent->right = NULL;
    // Freeing the branch.
    freeFamilyTree(vaccinatedNode, freeMember);

}

/******************
 * Function Name: printMenu
 * Input:
 * Output: void
 * Function Operation: The function prints the main menu.
******************/
static void printMenu() {
    puts("Please select an operation:\n"
         "\t0. Exit.\n"
         "\t1. Create family.\n"
         "\t2. Add family member.\n"
         "\t3. Print Family-Tree.\n"
         "\t4. Bidud.\n"
         "\t5. Superspreader.\n"
         "\t6. Vaccine.\n"
         "\t7. Print the menu.");
}

/******************
 * Function Name: runMenu
 * Input: Trin_Ari *family (a family created by newTree).
 * Output: void
 * Function Operation: The function gets the user's choice and calling the requested function.
******************/
void runMenu(Trin_Ari *family) {
    void
    (*actions[])(Trin_Ari *) = {createFamily, scheduleMeeting, printFamilyTree, quarantine, superspreader, vaccine};
    char choice;
    printMenu();
    while (1) {
        // Getting the choice.
        char temp;
        scanf("%c%c", &choice, &temp);

        if (choice == '0')
            break;

        if (choice == '7') {
            printMenu();
            continue;
        } else if (!((choice >= '1') && (choice <= '6'))) {
            // The choice is not supported, printing an error.
            puts("Error: unrecognized operation.");
        } else {
            // Calling the function.
            (*actions[choice - '1'])(family);
            if (choice == '3') {
                printMenu();
                continue;
            }
        }
        puts("Select the next operation (insert 7 for the entire menu):");
    }
}

/******************
 * Function Name: freeFamilyTree
 * Input: Trin_Ari *root (a tree created by newTree).
 * Output: void
 * Function Operation: The function uses recursion to free all nodes in the tree and prints a message for each one.
******************/
void freeFamilyTree(Trin_Ari *root, void(*freeValue)(void *)) {
    // If the root is empty, there is nothing to free, returning.
    if (root == NULL)
        return;
    // Freeing branches.
    freeFamilyTree(root->left, freeValue);
    freeFamilyTree(root->middle, freeValue);
    freeFamilyTree(root->right, freeValue);
    // Printing survived message.
    if (root->value != NULL)
        printf("%s ID: %d Survived!\n", ((Member *) root->value)->name, ((Member *) root->value)->id);
    // Freeing the node itself.
    freeValue(root->value);
    free(root);
}