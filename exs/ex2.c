#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-err34-c"
/***********************
* Tom Ben-Dor
*
* 01
* ex2
***********************/


#include <stdio.h>

/******************
 * Function Name: rhombus
 * Input:
 * Output:
 * Function Operation: the function gets a number from the user,
 *                     prints a rhombus which each side's length is that number.
******************/
void rhombus() {
    // Getting input from the user.
    printf("Enter the sides length:\n");
    int sidesLength;
    scanf("%d", &sidesLength);

    // Printing the top part of the rhombus.
    for (int i = 0; i < sidesLength; i++) {
        printf("%*s%*s%*s\n", sidesLength - i, "/", i + 1, "*", i + 1, "\\");
    }

    // Printing the middle raw of the rhombus.
    printf("|%*s%*s\n", sidesLength, "+", sidesLength, "|");

    // Printing the bottom part of the rhombus.
    for (int i = sidesLength - 1; i > -1; i--) {
        printf("%*s%*s%*s\n", sidesLength - i, "\\", i + 1, "*", i + 1, "/");
    }
}

/******************
 * Function Name: base20ToDec
 * Input:
 * Output:
 * Function Operation: the function gets a number in base 20 from the user and prints the number in base 10.
******************/
void base20ToDec() {
    // Getting input from the user.
    printf("Enter a reversed number in base 20:\n");
    char chr;
    scanf(" %c", &chr);

    int charIndex = 0, numInDec = 0, isValidNumber = 1;

    while (chr != '\n') {
        // Calculating pow(20, charIndex) for future use.
        int pow = 1;
        for (int i = 0; i < charIndex; i++)
            pow *= 20;

        // If the char is a digit:
        if ((chr >= '0') && (chr <= '9'))
            numInDec += (chr - '0') * pow;
        else {
            if (chr >= 'A' && chr <= 'J') {
                // Turning uppercase to lowercase:
                chr -= 'A' - 'a';
            }
            // The only case left which is a valid input is that the character is a lowercase letter between a-j.
            if (chr >= 'a' && chr <= 'j')
                // In base 20, 'a' is 10.
                numInDec += (chr - ('a' - 10)) * pow;
            else {
                printf("Error! %c is not a valid digit in base 20\n", chr);
                isValidNumber = 0;
            }
        }

        // Getting the next character from the buffer.
        scanf("%c", &chr);

        charIndex++;
    }

    // Printing the result.
    if (isValidNumber)
        printf("%d\n", numInDec);
}

/******************
 * Function Name: baseToDec
 * Input:
 * Output:
 * Function Operation: the function gets a base and a number from the user and prints the number in base 10.
******************/
void baseToDec() {
    // Getting input from the user.
    printf("Enter a base (2-10):\n");
    int base;
    scanf("%d", &base);

    printf("Enter a reversed number in base %d:\n", base);
    char chr;
    scanf(" %c", &chr);

    int charIndex = 0, numInDec = 0, isValidNumber = 1;

    while (chr != '\n') {
        if ((chr >= '0') && (chr < '0' + base)) {
            // Calculating pow(base, charIndex) for future use.
            int pow = 1;
            for (int i = 0; i < charIndex; i++)
                pow *= base;

            numInDec += (chr - '0') * pow;
        } else {
            printf("Error! %c is not a valid digit in base %d\n", chr, base);
            isValidNumber = 0;
        }

        // Getting the next character from the buffer.
        scanf("%c", &chr);

        charIndex++;
    }

    // Printing the result.
    if (isValidNumber)
        printf("%d\n", numInDec);
}

/******************
 * Function Name: pow2
 * Input:
 * Output:
 * Function Operation: the function gets a number from the user and prints whether it's a power of 2.
******************/
void pow2() {
    // Getting input from the user.
    printf("Enter a number:\n");
    unsigned int x;
    scanf("%d", &x);

    /* If x is a power of 2. Then x - 1 = ~x (when represented by the same number of bits).
       So (x & (x - 1)) is 0.
       x && ... is because 0 is not a power of 2.
    */

    // Printing the result.
    if (x && (!(x & (x - 1))))
        printf("%d is a power of 2\n", x);
    else
        printf("%d is not a power of 2\n", x);
}

/******************
 * Function Name: differentBits
 * Input:
 * Output:
 * Function Operation: the function gets two numbers  from the user and prints the number of different bits
 *                     when numbers are represented as binary.
******************/
void differentBits() {
    // Getting input from the user.
    printf("Enter two numbers:\n");
    int firstNum;
    scanf("%d", &firstNum);
    int secondNum;
    scanf("%d", &secondNum);

    // xor represents firstNum binary number which the number of non-zero bits stands for the number of different bits.
    int xor = firstNum ^secondNum, count = 0;

    // Counting how many non-zero (1) bits are in the number.
    while (xor) {
        count += xor & 1;
        xor >>= 1;
    }

    // Printing the result.
    printf("There are %d different bits\n", count);
}

/******************
 * Function Name: add
 * Input:
 * Output:
 * Function Operation: the function gets two numbers from the user and prints the sum of them.
******************/
void add() {
    // Getting input from the user.
    printf("Enter two numbers:\n");
    int firstNum;
    scanf("%d", &firstNum);
    int secondNum;
    scanf("%d", &secondNum);

    /* This algorithm nullifies the second number.
       Each time the second number gets one step closer towards zero,
       the number 1 is subtracted or added to the first number, depends on the sign of the second number.
    */

    if (secondNum > 0) {
        while (secondNum--)
            firstNum++;
    } else {
        while (secondNum++)
            firstNum--;
    }

    // Printing the result.
    printf("%d\n", firstNum);
}

/******************
 * Function Name: multiply
 * Input:
 * Output:
 * Function Operation: the function gets two numbers from the user and prints the multiplication of them.
******************/
void multiply() {
    // Getting input from the user.
    printf("Enter two numbers:\n");
    int firstNum;
    scanf("%d", &firstNum);
    int secondNum;
    scanf("%d", &secondNum);

    /* This algorithm nullifies the second number.
       Each time the second number gets one step closer towards zero,
       the first number is added to the result.
    */

    int result = 0;

    if (secondNum > 0) {
        while (secondNum--)
            result += firstNum;
    } else {
        while (secondNum++)
            result += firstNum;
        // We cannot use -= so we multiply result by -1 using bitwise operator.
        result = ~result + 1;
    }

    // Printing the result.
    printf("%d\n", result);
}

int main_ex2() {
    char choice;

    do {
        printf("Choose an option:\n"
               "1. Rhombus\n"
               "2. Base 20 to Decimal\n"
               "3. Base to Decimal\n"
               "4. Pow2\n"
               "5. Different bits\n"
               "6. Add\n"
               "7. Multiply\n"
               "0. Exit\n");
        scanf(" %c", &choice);

        switch (choice) {
            case '0':
                break;
            case '1':
                rhombus();
                break;
            case '2':
                base20ToDec();
                break;
            case '3':
                baseToDec();
                break;
            case '4':
                pow2();
                break;
            case '5':
                differentBits();
                break;
            case '6':
                add();
                break;
            case '7':
                multiply();
                break;
            default:
                printf("Wrong option!\n");
        }
    } while (choice != '0');

    return 0;
}

#pragma clang diagnostic pop