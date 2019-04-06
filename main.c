/**
 * Data Structures and Algorithms Assignment-1
 * Mert TURKMENOGLU
 *
 * Subject: Arithmetic expression evaluation using stack data structure
 *
 * This program demonstrates arithmetic expression evaluation steps and
 * uses stack data structure.
 *
 * User enters an expression from terminal and program iterates over string
 * char by char, finds every character type and calls appropriate function to handle
 * the case. Allowed characters are [1-9]+[0-9]* and '+', '-', '/', '*', '(', ')'.
 *
 * Important note: Program does integer arithmetic. So any floating point arithmetic value will be
 * converted to integer value. Typing allowed characters and considering floating point values is in users
 * responsibility.
 *
 * Some test(example) cases:
 *      13 + 5* (6+8/4) = 53
 *      8 + 2 * (21 / (7 - 4) + 2) = 26
 *      5 + 3 * 10 / 6 - 2 = 8
 *      (12 + 4 - 3 ) * (7 * 2 + 5) = 247
 *      21  /  ((4 + 8 ) * 2  -  17) = 3
 *
 *      4 - (3 + 5 + 6 * 2) + 17 = 1
 *      -4 + 8 = 4
 *      5*(-4*2) = -40
 *
 * @author Mert Turkmenoglu
 * @date 13.03.2019
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>
#include "stack.h"

extern int errno;

/**
 * Entry point of the program.
 * @param argc is the count of the argument entered
 * @param argv is the argument vector
 * @return 0 if successful termination else non-zero
 */
int main(int argc, char const *argv[]) {

    /**
     * expression[]: String toked from user
     * errnum: Error number for error handling
     * result: Result of the arithmetic expression
     */
    char expression[MAX_INPUT_SIZE];
    int errnum;
    int result;

    // Operand stack keeps numbers in the expression
    STACK *operand;
    operand = (STACK *) malloc(sizeof(STACK));

    // Error handling
    if (operand == NULL) {
        errnum = errno;
        fprintf(stderr, "Error No: %d\n", errno);
        perror("Memory could not allocated");
        fprintf(stderr, "Error memory allocation: %s\n", strerror(errnum));
        free(operand);
        exit(EXIT_FAILURE);
    }

    // Operator stack keeps operators in the expression
    STACK *operator;
    operator = (STACK *) malloc(sizeof(STACK));

    // Error handling
    if (operator == NULL) {
        errnum = errno;
        fprintf(stderr, "Error No: %d\n", errno);
        perror("Memory could not allocated");
        fprintf(stderr, "Error memory allocation: %s\n", strerror(errnum));
        free(operand);
        free(operator);
        exit(EXIT_FAILURE);
    }

    // Initializing of the stacks.
    initStack(operand, INT);
    initStack(operator, CHAR);

    // Taking expression from user
    printf("Enter arithmetic expression: \n");
    fgets(expression, MAX_INPUT_SIZE, stdin);
    printf("\nYou entered: %s", expression);

    // Evaluating and printing the result
    result = evaluateExpression(expression, operand, operator);
    printf("\nResult of the arithmetic expression is: %d\n", result);

    // Preventing memory leaks
    finalize(operand, operator);

    // Indicates successful termination
    return 0;
}