/***********************
* Tom Ben-Dor
*
* 01
* ass4
***********************/

#include <string.h>
#include <stdio.h>

#define PRINT_ERROR_IF(cond) {if (cond) {printf("Error\n"); return;}}

/******************
 * Function Name: printBoard
 * Input: char board[][SIZE * SIZE] (a sudoku board initialized by createBoard).
 * Output: void.
 * Function Operation: The function prints a sudoku board according to output.txt.
******************/
void printBoard(char board[][SIZE * SIZE]) {
    for (int blockOfRows = 0; blockOfRows < SIZE; ++blockOfRows) {
        for (int row = 0; row < SIZE; ++row) {
            for (int blockOfColumns = 0; blockOfColumns < SIZE; ++blockOfColumns) {
                for (int column = 0; column < SIZE; ++column)
                    printf("%c", board[blockOfRows * SIZE + row][blockOfColumns * SIZE + column]);
                if (blockOfColumns != SIZE - 1)
                    printf(" | ");
            }
            printf("\n");
        }
        printf("\n");
    }
}

/******************
 * Function Name: createBoard
 * Input: char board[][SIZE * SIZE] (an uninitialized sudoku board),
 *        char str[] (a string).
 * Output: void.
 * Function Operation: The function iterates str and puts the processed data into processedString.
 *                     For example: "2b3/1234/ax22/1" will be processed as followed: "2  31234 x221   ".
 *                     After processing, the board will be initialized directly from processedString.
******************/
void createBoard(char board[][SIZE * SIZE], char str[]) {
    int countRows = 0, index = 0;
    char processedString[SIZE * SIZE * SIZE * SIZE + 1] = "";

    // Iterating over str.
    while (index < strlen(str)) {
        // Processing one row every iteration.
        char row[SIZE * SIZE + 1] = "";

        int rowIndex = 0;
        while ((str[index + rowIndex] != '/') && (str[index + rowIndex] != '\0')) {
            // If the row is already full the input is invalid.
            PRINT_ERROR_IF(row[SIZE * SIZE - 1] != '\0')

            // Characters 'a' to 'a' + SIZE * SIZE - 1 should be interpreted as spaces.
            if ((str[index + rowIndex] >= 'a') && (str[index + rowIndex] <= 'a' + SIZE * SIZE - 1)) {
                for (int j = 0; j < str[index + rowIndex] - 'a' + 1; ++j)
                    strcat(row, " ");
            } else {
                char chr[2] = {str[index + rowIndex]};
                strcat(row, chr);
            }
            rowIndex++;
        }

        // Appending spaces to the end of the row if not enough data was found in str.
        while (row[SIZE * SIZE - 1] == '\0') {
            strcat(row, " ");
        }

        // Appending the row to the processed string.
        strcat(processedString, row);

        countRows++;
        index += rowIndex + 1;
    }

    /* The function doesn't handle input properly if the last character is '/',
     * so in that case we need to manually add an empty row.
     * */
    if (str[strlen(str) - 1] == '/') {
        for (int i = 0; i < SIZE * SIZE; ++i)
            strcat(processedString, " ");
        countRows++;
    }

    // If the number of rows found is not matching the size of the board, printing an error.
    PRINT_ERROR_IF(countRows != SIZE * SIZE)

    // Input is valid! Initializing board.
    for (int i = 0; i < SIZE * SIZE * SIZE * SIZE; ++i)
        board[i / (SIZE * SIZE)][i % (SIZE * SIZE)] = processedString[i];
}

/******************
 * Function Name: initParametersFromMove
 * Input: char parameters[] (uninitialized array of chars),
 *        int expectedNumberOfParameters (how many parameters we expect to find; length of parameters).
 * Output: int (0 - success, 1 - failure).
 * Function Operation: The function is called *after* first use of strtok.
 *                     The function initialize parameters from calling strtok and validating the output.
******************/
int initParametersFromMove(char parameters[], int expectedNumberOfParameters) {
    for (int i = 0; i < expectedNumberOfParameters; ++i) {
        // Getting next parameter from strtok.
        char *parameter = strtok(NULL, ",");

        // If parameter was found and it's length is 1 (valid character) then adding it to parameters array.
        if ((parameter != NULL) && (strlen(parameter) == 1))
            parameters[i] = parameter[0];
            // Parameter wasn't found or was found but it's an invalid character, return 1 (error accrued).
        else
            return 1;
    }

    // If another parameter was found (more than we need), return 1.
    if (strtok(NULL, ",") != NULL)
        return 1;

    // Found the expected number of parameters, return 0 (success).
    return 0;
}

/******************
 * Function Name: makeMove
 * Input: char board[][SIZE * SIZE] (a sudoku board initialized by createBoard),
 *        char move[] (operation to be taken).
 * Output: void.
 * Function Operation: The function uses strtok to section the move into command and parameters.
 *                     After validating the input it executes the command.
******************/
void makeMove(char board[][SIZE * SIZE], char move[]) {
    // Saving the length of the move for future validation.
    unsigned int moveLength = strlen(move);

    // Getting the command.
    char *token = strtok(move, ",");

    PRINT_ERROR_IF(token == NULL)

    // Saving the command into a variable.
    char command[7];
    strcpy(command, token);

    // If the command is replaceAll.
    if (strcmp(command, "replaceAll") == 0) {
        // Validating the length of the replaceAll command.
        PRINT_ERROR_IF(moveLength != strlen("replaceAll,x,y"))

        // Initializing parameters array and verifying that the initialization succeeded.
        int expectedNumberOfParameters = 2;
        char parameters[expectedNumberOfParameters + 1];

        PRINT_ERROR_IF(initParametersFromMove(parameters, expectedNumberOfParameters))

        // Checking if all parameters are valid.
        PRINT_ERROR_IF(parameters[0] == ' ')
        PRINT_ERROR_IF((parameters[1] == ' ') || (parameters[1] == '/') ||
                       ((parameters[1] >= 'a') && (parameters[1] <= 'a' + SIZE * SIZE - 1)))

        // Replacing the first parameter with the second.
        int found = 0;

        for (int row = 0; row < SIZE * SIZE; ++row) {
            for (int column = 0; column < SIZE * SIZE; ++column) {
                if (board[row][column] == parameters[0]) {
                    board[row][column] = parameters[1];
                    found = 1;
                }
            }
        }

        // If the first parameter was not found in the board even once, printing an error.
        PRINT_ERROR_IF(!found)

        // If the command is change or add (similar commands).
    } else if ((strcmp(command, "change") == 0) || (strcmp(command, "add") == 0)) {
        // Validating the length of the command.
        PRINT_ERROR_IF((moveLength != strlen("change,i,j,x")) && (moveLength != strlen("add,i,j,x")))

        // Initializing parameters array and verifying that the initialization succeeded.
        int expectedNumberOfParameters = 3;
        char parameters[expectedNumberOfParameters + 1];

        PRINT_ERROR_IF(initParametersFromMove(parameters, expectedNumberOfParameters))

        // Checking if all parameters are valid.
        PRINT_ERROR_IF(!((parameters[0] >= '0') && (parameters[0] < '0' + SIZE * SIZE)))
        PRINT_ERROR_IF(!((parameters[1] >= '0') && (parameters[1] < '0' + SIZE * SIZE)))
        PRINT_ERROR_IF((parameters[2] == ' ') || (parameters[2] == '/') ||
                       ((parameters[2] >= 'a') && (parameters[2] <= 'a' + SIZE * SIZE - 1)))

        // adding / changing accordingly to the command.
        char *box = &board[parameters[0] - '0'][parameters[1] - '0'];
        // Verifying that we are not changing a space or adding instead of a non-space (accordingly to the command).
        PRINT_ERROR_IF((strcmp(command, "change") == 0) ? *box == ' ' : *box != ' ')
        *box = parameters[2];

        // If the command is delete.
    } else if (strcmp(token, "delete") == 0) {
        // Validating the length of the replaceAll command.
        PRINT_ERROR_IF(moveLength != strlen("delete,i,j"))

        // Initializing parameters array and verifying that the initialization succeeded.
        int expectedNumberOfParameters = 2;
        char parameters[expectedNumberOfParameters + 1];

        PRINT_ERROR_IF(initParametersFromMove(parameters, expectedNumberOfParameters))

        // Checking if all parameters are valid.
        PRINT_ERROR_IF(!((parameters[0] >= '0') && (parameters[0] < '0' + SIZE * SIZE)))
        PRINT_ERROR_IF(!((parameters[1] >= '0') && (parameters[1] < '0' + SIZE * SIZE)))

        // Deleting as requested.
        char *box = &board[parameters[0] - '0'][parameters[1] - '0'];
        // Verifying that we are not deleting a space.
        PRINT_ERROR_IF(*box == ' ')
        *box = ' ';

    } else
        // Invalid command, printing an error.
        printf("Error\n");
}

/******************
 * Function Name: foundConflict
 * Input: char board[][SIZE * SIZE] (a sudoku board initialized by createBoard),
 *        char chr (a char in the board).
 * Output: int (0 - no conflict, 1 - found conflict).
 * Function Operation: The function consists of two parts:
 *                      1) Searching for conflict in rows / columns.
 *                      2) Searching for conflict in sub-boards (SIZE by SIZE boards).
 *
 *                      (A conflict is when we find the same character in the same row / column or sub-board.)
******************/
int foundConflict(char board[][SIZE * SIZE], char chr) {
    // Searching for conflict in rows / columns.
    int foundByRow[SIZE * SIZE] = {}, foundByColumn[SIZE * SIZE] = {};

    for (int row = 0; row < SIZE * SIZE; ++row) {
        for (int column = 0; column < SIZE * SIZE; ++column) {
            if (board[row][column] != chr)
                continue;

            // If we have already found this character in the same row / column before, return 1 (conflict was found).
            if (foundByRow[row] || foundByColumn[column])
                return 1;

            // Saving that we found this character in this location.
            foundByRow[row] = foundByColumn[column] = 1;
        }
    }

    /* Index stands for the index of the top left character in a sub-board.
     * For example, in a 4 by 4 board this indexes are 0, 2, 8 & 10.
     * */
    int index = 0;

    for (int subBoard = 0; subBoard < SIZE * SIZE; ++subBoard) {
        int found = 0;

        // Iterating the characters in the sub-board only.
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                if (board[i + (index / (SIZE * SIZE))][j + (index % (SIZE * SIZE))] != chr)
                    continue;

                // If we have already found this character in the same sub-board before, return 1 (conflict was found).
                if (found)
                    return 1;

                // Saving that we found this character in this sub-board.
                found = 1;
            }
        }

        // Getting to the next sub-board.
        if ((index + SIZE) % (SIZE * SIZE))
            index += SIZE;
        else
            index = index - (index % (SIZE * SIZE)) + SIZE * SIZE * SIZE;
    }

    // No conflict was found, return 0.
    return 0;
}

/******************
 * Function Name: foundVariable
 * Input: char board[][SIZE * SIZE] (a sudoku board initialized by createBoard).
 * Output: int (1 - found, 0 - wasn't found).
 * Function Operation: The function returns whether or not all the characters in the board are digits and spaces.
******************/
int foundVariable(char board[][SIZE * SIZE]) {
    for (int row = 0; row < SIZE * SIZE; ++row) {
        for (int column = 0; column < SIZE * SIZE; ++column) {
            if ((board[row][column] == ' ') ||
                ((board[row][column] >= '1') && (board[row][column] < '1' + SIZE * SIZE)))
                continue;

            return 1;
        }
    }

    return 0;
}

/******************
 * Function Name: testBoard
 * Input: char board[][SIZE * SIZE] (a sudoku board initialized by createBoard).
 * Output: int (0 - failed, 1 - passed).
 * Function Operation: The function returns whether or not the board is a valid sudoku board.
******************/
int testBoard(char board[][SIZE * SIZE]) {
    // Searching for conflict.
    for (char chr = '1'; chr < '1' + SIZE * SIZE; chr++) {
        if (foundConflict(board, chr))
            return 0;
    }

    // Verifying that the board doesn't includes variables.
    return !foundVariable(board);
}

/******************
 * Function Name: isSameBoard
 * Input: char board1[][SIZE * SIZE] (a sudoku board initialized by createBoard),
 *        char board2[][SIZE * SIZE] (a sudoku board initialized by createBoard).
 * Output: int (boolean; 1 - identical, 0 - not identical).
 * Function Operation: The function compares each character in both boards.
******************/
int isSameBoard(char board1[][SIZE * SIZE], char board2[][SIZE * SIZE]) {
    for (int row = 0; row < SIZE * SIZE; ++row) {
        for (int column = 0; column < SIZE * SIZE; ++column) {
            if (board1[row][column] != board2[row][column]) {
                printf("Found inequality on row %d col %d.\n", row, column);
                return 0;
            }
        }
    }

    return 1;
}

/******************
 * Function Name: solveSudoku
 * Input: char board[][SIZE * SIZE] (a sudoku board initialized by createBoard).
 * Output: int (1 - solved, 0 - unsolvable).
 * Function Operation: The function uses recursion.
 *                     In each call the function searches for a solution to the board.
 *                     If found, it returns 1. If the board is unsolvable - it returns 0.
 *
 *                     This function can solve *any* Sudoku board, not only almost completed.
******************/
int solveSudoku(char board[][SIZE * SIZE]) {
    for (int row = 0; row < SIZE * SIZE; ++row) {
        for (int column = 0; column < SIZE * SIZE; ++column) {
            if (board[row][column] != ' ')
                continue;

            // Trying to replace the space with a digit.
            for (char i = '1'; i < '1' + SIZE * SIZE; ++i) {
                board[row][column] = i;

                /* If we can replace the space with <i> and we can solve the new board
                 * then the board is solved - return 1.
                 * */
                if (testBoard(board) && solveSudoku(board))
                    return 1;
            }
            // The board is unsolvable, return 0.
            board[row][column] = ' ';

            return 0;
        }
    }

    // There are no spaces in the board.
    return testBoard(board);
}

/******************
 * Function Name: completeBoard
 * Input: char board[][SIZE * SIZE] (a sudoku board initialized by createBoard).
 * Output: void
 * Function Operation: The function verifies the board is almost solved,
 *                     then solves it using solveSudoku() or prints error if it's unsolvable.
******************/
void completeBoard(char board[][SIZE * SIZE]) {
    // Verifying the board is almost solved.
    PRINT_ERROR_IF(foundConflict(board, ' '))

    // Solving it or printing an error if it's unsolvable.
    PRINT_ERROR_IF(!solveSudoku(board))
}