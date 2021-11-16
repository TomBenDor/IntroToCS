#ifndef MAIN_C_EX4_H
#define MAIN_C_EX4_H
#include <stdio.h>
void createBoard(char board[][SIZE*SIZE],char str[]);
void printBoard(char board[][SIZE*SIZE]);
int isSameBoard(char board[][SIZE*SIZE],char board1[][SIZE*SIZE]);
void makeMove(char board[][SIZE*SIZE], char token[]);
void completeBoard(char board[][SIZE*SIZE]);
int testBoard(char board[][SIZE*SIZE]);

#endif //MAIN_C_EX4_H

