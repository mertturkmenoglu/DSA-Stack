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

#define MAX_STACK_SIZE 100
#define MAX_INPUT_SIZE 100
#define FALSE 0
#define TRUE 1

/*
 * Every stack has a type field.
 * This program needs and uses two character type.
 * Others can be added.
 */
enum STACK_TYPE {
    CHAR, INT
};

/*
 * Iterating over string, every character has a type.
 * In an expression valid types are: Space, digit or punctuation
 * Wrong field indicates input error.
 */
enum CHAR_TYPE {
    SPACE, DIGIT, PUNCTUATION, WRONG
};

/*
 * Arithmetic operator precedence.
 */
enum PRECEDENCE {
    HIGHER, EQUAL, LOWER
};

/*
 * Using struct for Stack data type.
 * It holds and keeps everything about stack in one place.
 * void *item is keeps elements
 * int top is the stack pointer(SP) and shows top of the stack
 * STACK_TYPE is the type of the elements in the item array
 */
typedef struct {
    void *item;
    int top;
    enum STACK_TYPE type;
} STACK;

typedef int BOOLEAN;



// Function prototypes
void initStack(STACK *stack, enum STACK_TYPE type);

BOOLEAN isEmpty(const STACK *stack);

BOOLEAN isFull(const STACK *stack);

BOOLEAN push(void *x, STACK *stack);

BOOLEAN pop(void *x, STACK *stack);

BOOLEAN peek(void *x, const STACK *stack);

void printStack(const STACK *stack);

void printStackStatus(const STACK *operand, const STACK *operator);

void deleteStack(STACK *stack);

void finalize(STACK *stack, ...);

int evaluateExpression(const char *exp, STACK *operand, STACK *operator);

void operatorEval(char c, STACK *operator, STACK *operand);

void operate(char c, char peek, enum PRECEDENCE p, BOOLEAN flag, STACK *operator, STACK *operand);

enum CHAR_TYPE typeOfChar(char c);

enum PRECEDENCE compare(char input, char peekValue);

int digitHandler(const char *exp, int *i);

void executeOperation(STACK *operator, STACK *operand);

int toInt(const char *str);

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



/**
 * finalize function
 * This function handles memory free operations
 * Takes variable length arguments
 * @param s is the STACK* of the stack
 */
void finalize(STACK *s, ...) {
    va_list valist;
    va_start(valist, s);
    STACK *tmp = s;
    while (tmp != NULL) {
        deleteStack(tmp);
        free(tmp);
        tmp->item = NULL;
        tmp = va_arg(valist, STACK*);
    }
    va_end(valist);
}



/**
 * deleteStack function
 * Frees memory allocated and handles dangling pointers
 * @param s is the pointer of the stack
 */
void deleteStack(STACK *s) {
    free(s->item);
    s->item = NULL;
}



/**
 * typeOfChar function
 * Finds the enumeration type of the character given
 * @param c is the character to find the type
 * @return type of the character as enum
 */
enum CHAR_TYPE typeOfChar(char c) {
    if (isspace(c))
        return SPACE;
    else if (isdigit(c))
        return DIGIT;
    else if (ispunct(c))
        return PUNCTUATION;
    else
        return WRONG;
}



/**
 * executeOperation function
 * This function pops two operand and pops one operator from stacks
 * and does mathematical operation, pushes result back to operand stack
 * Function contains pop and push function calls. Before calling this function,
 * you must not pop or push any value.
 * @param operator is the pointer to operator stack
 * @param operand is the pointer to operand stack
 */
void executeOperation(STACK *operator, STACK *operand) {
    int a, b, result;
    char op;
    pop(&a, operand);
    pop(&b, operand);
    pop(&op, operator);
    switch (op) {
        case '+':
            result = b + a;
            break;
        case '-':
            result = b - a;
            break;
        case '*':
            result = b * a;
            break;
        case '/':
            result = b / a;
        default:
            break;
    }
    push(&result, operand);
}



/**
 * punctEval function
 * This function evaluates punctuation characters.
 * @param c is the punctuation character
 * @param operator is the pointer to operator stack
 * @param operand is the pointer to operand stack
 */
void punctEval(char c, STACK *operator, STACK *operand) {
    char tmp = 'a';
     
    // If it is opening parenthesis, push to stack
    if (c == '(') {
        push(&c, operator);
        return;
    }

    // If it is closing parenthesis, execute all operations
    // until relative opening parenthesis
    if (c == ')') {
        while (tmp != '(') {
            executeOperation(operator, operand);
            peek(&tmp, operator);
        }
        pop(&tmp, operator);
        return;
    }

    // If it is not a parenthesis then it is an operator
    operatorEval(c, operator, operand);
}



/**
 * operatorEval function
 * This function evaluates operations. For every operator state,
 * function checks and calls appropriate functions
 * @param c is the operator character
 * @param operator is the pointer to operator stack
 * @param operand is the pointer to operand stack
 */
void operatorEval(char c, STACK *operator, STACK *operand) {
    char tmp = 'a';
    BOOLEAN flag = TRUE;

    if (!isEmpty(operator)) {
        peek(&tmp, operator);
        if ((tmp != '(') && (tmp != ')')) {
            enum PRECEDENCE p = compare(c, tmp);
            operate(c, tmp, p, flag, operator, operand);
        }
    }
    push(&c, operator);
}



/**
 * operate function
 * This function handles operation execution order.
 * @param c is the operator character
 * @param peekValue is the top character on the operator stack
 * @param p is the precedence of the two operator
 * @param flag controls if pop operation happened
 * @param operator is the pointer to operator stack
 * @param operand is the pointer to operand stack
 */
void operate(char c, char peekValue, enum PRECEDENCE p, BOOLEAN flag, STACK *operator, STACK *operand) {
    while (((p == LOWER) || (p == EQUAL)) && (flag)) {
        executeOperation(operator, operand);
        flag = peek(&peekValue, operator) ? TRUE : FALSE;
        if ((peekValue != '(') && (peekValue != ')'))
            p = compare(c, peekValue);
        else
            flag = FALSE;
    }
}



/**
 * compare function
 * This function compares given two operator.
 * @param input is the character read from input string
 * @param peekValue is the character at the top of the stack
 * @return enum PRECEDENCE of the input character
 */
enum PRECEDENCE compare(char input, char peekValue) {
    if (input == '/') {
        switch (peekValue) {
            case '/':
                return EQUAL;
            case '*':
                return EQUAL;
            case '+':
                return HIGHER;
            case '-':
                return HIGHER;
            default:
                return LOWER;
        }
    }

    if (input == '*') {
        switch (peekValue) {
            case '/':
                return EQUAL;
            case '*':
                return EQUAL;
            case '+':
                return HIGHER;
            case '-':
                return HIGHER;
            default:
                return LOWER;
        }
    }

    if (input == '+') {
        switch (peekValue) {
            case '/':
                return LOWER;
            case '*':
                return LOWER;
            case '+':
                return EQUAL;
            case '-':
                return EQUAL;
            default:
                return LOWER;
        }
    }

    if (input == '-') {
        switch (peekValue) {
            case '/':
                return LOWER;
            case '*':
                return LOWER;
            case '+':
                return EQUAL;
            case '-':
                return EQUAL;
            default:
                return LOWER;
        }
    }

    return HIGHER;
}



/**
 * digitHandler function
 * This function finds the integer number as a string in a
 * given string and returns its integer equivalent
 * @param exp is the expression string
 * @param i is the pointer to index number of the starting character
 * @return integer equivalent of the string
 */
int digitHandler(const char *exp, int *i) {
    size_t len = strlen(exp);
    char str[MAX_INPUT_SIZE];
    char tmp[MAX_INPUT_SIZE];
    strcpy(str, "");
    strcpy(tmp, "");
    while ((*i < len) && (isdigit(exp[*i]))) {
        sprintf(tmp, "%c", exp[*i]);
        strcat(str, tmp);
        *i += 1;
    }
    int intRep = toInt(str);
    return intRep;
}



/**
 * evaluateExpression function
 * This function is the main evaluation function.
 * All evaluation functions are called from this function.
 * @throws Input/Output error when invalid character is encountered
 * @param exp is the expression string
 * @param operand is the pointer to operand stack
 * @param operator is the pointer to operator stack
 * @return result of the mathematical operations
 */
int evaluateExpression(const char *exp, STACK *operand, STACK *operator) {
    // Take the string length
    size_t len = strlen(exp);
    int i = 0;
    enum CHAR_TYPE charType;
    int tmp;

    while (i < len) {
        charType = typeOfChar(exp[i]);
        switch (charType) {
            case SPACE:
                i++;
                break;
            case DIGIT:
                tmp = digitHandler(exp, &i);
                push(&tmp, operand);
                printStackStatus(operand, operator);
                break;
            case PUNCTUATION:
                punctEval(exp[i], operator, operand);
                printStackStatus(operand, operator);
                i++;
                break;
            case WRONG:
                fprintf(stderr, "Invalid character\n");
                errno = 5;
                perror("Error: Invalid expression");
                finalize(operand, operator);
                exit(EXIT_FAILURE);
        }
    }

    // If any operation left, do operations until operand stack has 1 value
    while (operand->top != 1) {
        executeOperation(operator, operand);
        printStackStatus(operand, operator);
    }

    // Return operand->item[0]
    return *((int *) (operand->item));
}



/**
 * initStack function
 * This function initialize stack given as a pointer
 * with its data type. Stack is simulated over array structure.
 * Array is dynamically allocated and its type is depends on type of t
 * @param s is the stack pointer
 * @param t is the enumeration type of the stack
 */
void initStack(STACK *s, enum STACK_TYPE t) {
    s->type = t;
    if (s->type == INT)
        s->item = (int *) malloc(MAX_STACK_SIZE * sizeof(int));
    else
        s->item = (char *) malloc(MAX_STACK_SIZE * sizeof(char));
    s->top = 0;
}



/**
 * isEmpty function
 * This function controls top field of the given stack
 * and returns TRUE if top indicates 0 else FALSE
 * @param s is the stack pointer
 * @return TRUE if SP is 0 else FALSE
 */
BOOLEAN isEmpty(const STACK *s) {
    return (s->top == 0) ? TRUE : FALSE;
}



/**
 * isFull function
 * This function controls top field of the given stack
 * and returns TRUE if top indicates MAX_STACK_SIZE else FALSE
 * @param s is the stack pointer
 * @return TRUE if SP is MAX_STACK_SIZE else FALSE
 */
BOOLEAN isFull(const STACK *s) {
    return (s->top == MAX_STACK_SIZE) ? TRUE : FALSE;
}



/**
 * push function
 * This function appends given value to array field of the stack
 *
 * @param x is the pointer of the variable which holds value appended to array
 * @param s is the pointer to stack
 * @return TRUE if stack is not full else FALSE
 */
BOOLEAN push(void *x, STACK *s) {
    if (isFull(s) == FALSE) {
        if (s->type == INT)
            *((int *) (s->item) + s->top++) = *(int *) x;
        else
            *((char *) (s->item) + s->top++) = *(char *) x;
        return TRUE;
    }
    return FALSE;
}



/**
 * pop function
 * This function takes top value of array field of the stack
 *
 * @param x is the pointer of the variable which will hold value taken from array
 * @param s is the pointer to stack
 * @return TRUE if stack is not empty else FALSE
 */
BOOLEAN pop(void *x, STACK *s) {
    if (isEmpty(s) == FALSE) {
        s->top -= 1;
        if (s->type == INT)
            *(int *) x = *((int *) (s->item) + (s->top));
        else
            *(char *) x = *((char *) (s->item) + (s->top));

        return TRUE;
    }
    return FALSE;
}



/**
 * peek function
 * This function reads top value of array field of the stack
 *
 * @param x is the pointer of the variable which will hold value read from array
 * @param s is the pointer to stack
 * @return TRUE if stack is not empty else FALSE
 */
BOOLEAN peek(void *x, const STACK *s) {
    if (isEmpty(s) == FALSE) {
        if (s->type == INT)
            *(int *) x = *((int *) (s->item) + (s->top-1));
        else
            *(char *) x = *((char *) (s->item) + (s->top-1));
        return TRUE;
    }
    return FALSE;
}



/**
 * printStack function
 * This function prints given stack and its values
 * @param s is the pointer to stack
 */
void printStack(const STACK *s) {
    int i;
    printf("\nStack: \n");
    if (s->type == INT) {
        for (i = 0; i < s->top; i++)
            printf("%d\t", *((int *) (s->item) + i));
    } else {
        for (i = 0; i < s->top; i++)
            printf("%c\t", *((char *) (s->item) + i));
    }
    printf("\n");
}



/**
 * printStackStatus function
 * This function calls printStack function and prints styling
 * characters to terminal.
 * This function is called whenever a stack operation happens.
 * @param operand is the pointer to operand stack
 * @param operator is the pointer to operator stack
 */
void printStackStatus(const STACK *operand, const STACK *operator) {
    printStack(operand);
    printStack(operator);
    printf("-----------\n");
}



/**
 * toInt function
 * This function finds integer equivalent of the given string
 * @param str is the given string
 * @return integer equivalent of the given string
 */
int toInt(const char *str) {
    int len = 0;
    int i = 0;
    while(str[len++] != '\0');
    len--;
    int result=0;

    for(i = 0; i < len; i++) {
        result *= 10;
        result += str[i] - '0';
    }

    return result;
}
