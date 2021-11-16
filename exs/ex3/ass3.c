/***********************
* Tom Ben-Dor
*
* 01
* ass3
***********************/

#define BONUS
#define ABS(number) ((number > 0) ? number : -number)

#include <stdio.h>

void towerOfHanoi(int numDisks, char fromRod, char toRod, char auxRod);

int isPalindromeHelper(char *str, int len, int index);

void isPalindrome(char str[], int len);

void isPalindromeIter(char str[], int len);

void printAllCombinationsHelper(char *pattern, int len, int index);

void printAllCombinations(char pattern[], int len);

void powRec(long int firstNum, long int secondNum);

int isDivisibleBy3Helper(long long n, int sum);

void isDivisibleBy3(long long n);

void IsDividedBy3Iter(long long n);

void gcd(long int n1, long int n2);

int countDigitHelper(long long n, int d);

void countDigit(long long n, int d);


/******************
 * Function Name: towerOfHanoi
 * Input: int numDisks (number of disks),
 *        char fromRod (name of the rod on which all disks are at the beginning),
 *        char toRod (name of the rod on which all disks are at the end),
 *        char auxRod (name of the rod which operates as a temporary rod to help moving all disks).
 * Output: void.
 * Function Operation: The function uses recursion.
 *                     And prints step by step instructions of how to move all disks to the target rod.
******************/
void towerOfHanoi(int numDisks, char fromRod, char toRod, char auxRod) {
    if (numDisks <= 0)
        return;

    // Move n - 1 disks to the auxRod.
    towerOfHanoi(numDisks - 1, fromRod, auxRod, toRod);
    // Move the last disk to the destination rod.
    printf("Move disk %d from rod %c to rod %c.\n", numDisks, fromRod, toRod);
    // Move the n - 1 disks to the destination rod.
    towerOfHanoi(numDisks - 1, auxRod, toRod, fromRod);
}

/******************
 * Function Name: isPalindromeHelper
 * Input: char str[] (a string),
 *        int len (the string's length),
 *        int index (starts at 0).
 * Output: int (boolean).
 * Function Operation: The function uses recursion.
 *                     In each call the function checks if two chars are identical.
******************/
int isPalindromeHelper(char *str, int len, int index) {
    // If index > len - 1 then the string was already verified as a palindrome.
    if (index > len / 2)
        return 1;
    // If str[index] != str[len - index - 1] the second condition is not evaluated and the function returns 0.
    return (str[index] == str[len - index - 1]) && isPalindromeHelper(str, len, index + 1);
}

/******************
 * Function Name: isPalindrome
 * Input: char str[] (any string - no restrictions),
 *        int len (the string's length).
 * Output: void
 * Function Operation: The function simply calls the function isPalindromeHelper and prints the result.
******************/
void isPalindrome(char str[], int len) {
    // Printing the output.
    if (isPalindromeHelper(str, len, 0))
        printf("The reverse of %s is also %s.\n", str, str);
    else
        printf("The reverse of %s is not %s.\n", str, str);
}

/******************
 * Function Name: isPalindromeIter
 * Input: char str[] (any string - no restrictions),
 *        int len (the string's length).
 * Output: void
 * Function Operation: The function iterates the characters in the string.
 *                     For example: str = "abca", len = 4.
 *                     The function compares the first and the last char.
 *                     Because they are identical, the function continues to compare the second and third chars.
******************/
void isPalindromeIter(char str[], int len) {

    /*
     * The loop compares two chars in each round.
     * The first & the last, the second & the second from the end, ...
     */
    for (int i = 0; i < len / 2; ++i) {
        if (str[i] != str[len - i - 1]) {
            // The char matching the i-index is not identical to the char matching the (len-i-1) index.
            printf("The reverse of %s is not %s.\n", str, str);
            return;
        }
    }
    // If the program got here the string is a palindrome.
    printf("The reverse of %s is also %s.\n", str, str);
}

int numberOfCombinations = 0;

/******************
 * Function Name: printAllCombinationsHelper
 * Input: char pattern[] (any string - no restrictions),
 *        int len (the string's length),
 *        int index (starts at 0).
 * Output: void.
 * Function Operation: The function uses recursion.
 *                     The function compares the character matching the index to the character '?'.
 *                     If they are identical, the function replaces the '?' by 0, 1 and 2.
******************/
void printAllCombinationsHelper(char *pattern, int len, int index) {
    if (len == index) {
        // A combination was found, printing it.
        printf("%s\n", pattern);
        // Increasing the counter of combinations by 1.
        numberOfCombinations++;
        return;
    }

    /*
     * When a '?' is found in the pattern, numberOfCombinations is doubled by 3.
     * Because this character can be replaced by '0', '1' & '2'.
     */
    if (pattern[index] == '?') {
        // Changing the character to '0', '1' & '2' and then back to '?'.
        pattern[index] = '0';
        printAllCombinationsHelper(pattern, len, index + 1);
        pattern[index] = '1';
        printAllCombinationsHelper(pattern, len, index + 1);
        pattern[index] = '2';
        printAllCombinationsHelper(pattern, len, index + 1);
        pattern[index] = '?';
    } else
        // Moving to next character.
        printAllCombinationsHelper(pattern, len, index + 1);
}

/******************
 * Function Name: printAllCombinations
 * Input: char pattern[] (any string - no restrictions),
 *        int len (the string's length).
 * Output: void.
 * Function Operation: The function simply calls the function printAllCombinationsHelper and prints the result.
******************/
void printAllCombinations(char pattern[], int len) {
    // Nullifying the number of combinations.
    numberOfCombinations = 0;
    // Printing all combinations.
    printAllCombinationsHelper(pattern, len, 0);
    // Printing the number of combinations found.
    printf("Number of combinations is: %d\n", numberOfCombinations);
}

/******************
 * Function Name: powRecHelper
 * Input: long int firstNum (any number),
 *        long int secondNum (positive number; can be zero).
 * Output: long int (firstNum to the power of secondNum).
 * Function Operation: The function uses recursion.
 *                     If the second number is 0, it returns 1.
 *                     Otherwise, it returns firstNum * powRecHelper(firstNum, secondNum - 1).
******************/
long int powRecHelper(long int firstNum, long int secondNum) {
    if (secondNum == 0)
        return 1;
    // x ^ y = x * x ^ (y - 1) (^ means to the power of).
    return firstNum * powRecHelper(firstNum, secondNum - 1);
}

/******************
 * Function Name: powRec
 * Input: long int firstNum (any number),
 *        long int secondNum (positive number; can be zero). * Output: void.
 * Output: void.
 * Function Operation: The function simply calls the function powRecHelper and prints the result.
 *                     Note:
 *                     This function is not able to handle 0 to the power of a negative number,
 *                     so it first checks if that's the case and handles it.
******************/
void powRec(long int firstNum, long int secondNum) {
    // Checking whether the first number is 0 and the second is negative.
    if ((firstNum == 0) && (secondNum < 0)) {
        printf("The result is nan.\n");
        return;
    }

    // Using powRecHelper to calculate the result.
    double pow;
    if (secondNum > 0)
        pow = powRecHelper(firstNum, secondNum);
    else
        pow = (double) 1 / powRecHelper(firstNum, -secondNum);

    // Printing the output.
    printf("The result is %.6f.\n", pow);
}

/******************
 * Function Name: isDivisibleBy3Helper
 * Input: long long n (a number consisting of digits 1 to 3),
 *        int sum (starts at 3).
 * Output: int (boolean).
 * Function Operation: The function uses recursion.
 *                     The function sums all the digits % 3 when 0 represented by 3.
 *                     A number is divisible by 3 if and only if the sum of it's digits is divisible by 3.
 *                     Once all the digits were added to the sum, the function returns if the sum is 3.
******************/
int isDivisibleBy3Helper(long long n, int sum) {
    if (n == 0)
        return sum == 3;

    int lastDigit = n % 10;

    /*
     * We want to return isDivisibleBy3Helper(n / 10, (sum + lastDigit) % 3)
     * (when 0 is 3, because the digits are only 1, 2 & 3)
     */
    if ((lastDigit == 3) || (sum == 3))
        return isDivisibleBy3Helper(n / 10, (sum == 3) ? lastDigit : sum);
    if (((lastDigit == 1) && (sum == 2)) || ((lastDigit == 2) && (sum == 1)))
        return isDivisibleBy3Helper(n / 10, 3);
    if (((lastDigit == 1) && (sum == 1)))
        return isDivisibleBy3Helper(n / 10, 2);
    if (((lastDigit == 2) && (sum == 2)))
        return isDivisibleBy3Helper(n / 10, 1);

    // Invalid parameter n, return -1.
    return -1;
}

/******************
 * Function Name: isDivisibleBy3
 * Input: long long n (a number consisting of digits 1 to 3).
 * Output: void.
 * Function Operation: The function simply calls the function isDivisibleBy3Helper and prints the result.
******************/
void isDivisibleBy3(long long n) {
    // Printing the output.
    if (isDivisibleBy3Helper(n, 3))
        printf("The number %lld is divisible by 3.\n", n);
    else
        printf("The number %lld is not divisible by 3.\n", n);
}

/******************
 * Function Name: IsDividedBy3Iter
 * Input: long long n (a number - no restrictions).
 * Output: void.
 * Function Operation: The function sums all digits into a final digit (0-9).
 *                     A number is divisible by 3 if and only if the sum of it's digits is divisible by 3.
 *                     Once all the digits were added to sumOfDigits,
 *                     the function returns if sumOfDigits is 0,3,6 or 9 and prints the result.
******************/
void IsDividedBy3Iter(long long n) {
    long long sumOfDigits = 0, copyOfn = n;

    /*
     * Calculating the final sum of the number's digits.
     * "final sum" = sum of digits of the sum of the digits and so... Until the sum is smaller than 10.
     * */
    while (copyOfn > 0) {
        sumOfDigits += (copyOfn % 10);
        // If sumOfDigits is bigger than 9 it consists of 2 digits, so we sum them.
        if (sumOfDigits > 9)
            sumOfDigits = (sumOfDigits % 10) + (sumOfDigits / 10);
        copyOfn /= 10;
    }

    /*
     * If the sum of the digits is 0, 3, 6, or 9 the number is divisible by 3.
     */
    switch (sumOfDigits) {
        case 0:
        case 3:
        case 6:
        case 9:
            printf("The number %lld is divisible by 3.\n", n);
            break;
        default:
            printf("The number %lld is not divisible by 3.\n", n);
    }
}

/******************
 * Function Name: gcd
 * Input: long int n1,
 *        long int n2.
 * Output: void.
 * Function Operation: The function implements the algorithm given from wikipedia.
******************/
void gcd(long int n1, long int n2) {
    // If one of the numbers is 0, gcd equals to the absolute value of the second number.
    if ((n1 == 0) || (n2 == 0)) {
        printf("GCD=%ld\n", (n1 == 0) ? ABS(n2) : ABS(n1));
        return;
    }

    // Printing a step.
    long int q = n1 / n2, r = n1 % n2;
    printf("%ld*%ld+%ld=%ld(a=%ld,b=%ld)\n", n2, q, r, n1, n1, n2);
    // Calling the function with the second number and the reminder.
    gcd(n2, r);
}

/******************
 * Function Name: countDigitHelper
 * Input: long long n (a number),
 *        int d (a digit; 0 <= d <= 9).
 * Output: int (positive; can be zero).
 * Function Operation: The function uses recursion.
 *                     If the number has only one digit, it returns 0 or 1 (whether the number is equals to d).
 *                     Otherwise, it returns the result for the number without the last digit,
 *                     plus 0 or 1 - depends whether the last digit is equals to d.
 *                     Note:
 *                     This function takes advantage of the fact that boolean expression has a value (0 or 1).
******************/
int countDigitHelper(long long n, int d) {
    // If the number has only one digit.
    if (n / 10 == 0)
        return n == d;
    // Counting how many times the digit appears in n / 10 and adding 1 or 0.
    return countDigitHelper(n / 10, d) + (n % 10 == d);
}

/******************
 * Function Name: countDigit
 * Input: long long n (a number),
 *        int d (a digit; 0 <= d <= 9).
 * Output: void.
 * Function Operation: The function simply calls the function countDigitHelper and prints the result.
******************/
void countDigit(long long n, int d) {
    // Printing the output.
    printf("%lld has %d times %d.\n", n, countDigitHelper(n, d), d);
}