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
void initStack(STACK *, enum STACK_TYPE);

BOOLEAN isEmpty(const STACK *);

BOOLEAN isFull(const STACK *);

BOOLEAN push(void *, STACK *);

BOOLEAN pop(void *, STACK *);

BOOLEAN peek(void *, const STACK *);

void printStack(const STACK *);

int evaluateExpression(const char *, STACK *, STACK *);

void deleteStack(STACK *);

void finalize(STACK *s, ...);

enum CHAR_TYPE typeOfChar(char c);

enum PRECEDENCE compare(char inp, char peek);

int digitHandler(const char *exp, int *i);

void executeOperation(STACK *operator, STACK *operand);

void printStackStatus(const STACK *, const STACK*);

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


// TODO: COMMENT
/**
 * executeOperation function
 *
 * @param operator
 * @param operand
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

void punctEval(char c, STACK *operator, STACK *operand) {
    char tmp = 'a';
    BOOLEAN flag = TRUE;

    if (c == '(') {
        push(&c, operator);
        return;
    }

    if (c == ')') {
        while (tmp != '(') {
            executeOperation(operator, operand);
            peek(&tmp, operator);
        }
        pop(&tmp, operator);
        return;
    }

    if (!isEmpty(operator)) {
        peek(&tmp, operator);
        if ((tmp != '(') && (tmp != ')')) {
            enum PRECEDENCE p = compare(c, tmp);
            while (((p == LOWER) || (p == EQUAL)) && (flag)) {
                executeOperation(operator, operand);
                flag = peek(&tmp, operator) ? TRUE : FALSE;
                if ((tmp != '(') && (tmp != ')'))
                    p = compare(c, tmp);
                else
                    flag = FALSE;
            }
        }
    }
    push(&c, operator);
}

enum PRECEDENCE compare(char inp, char peek) {
    if (inp == '/') {
        switch (peek) {
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

    if (inp == '*') {
        switch (peek) {
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

    if (inp == '+') {
        switch (peek) {
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

    if (inp == '-') {
        switch (peek) {
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

    while (operand->top != 1) {
        executeOperation(operator, operand);
        printStackStatus(operand, operator);
    }
    // Return operand->item[0]
    return *((int *) (operand->item));
}

void initStack(STACK *s, enum STACK_TYPE t) {
    s->type = t;
    if (s->type == INT)
        s->item = (int *) malloc(MAX_STACK_SIZE * sizeof(int));
    else
        s->item = (char *) malloc(MAX_STACK_SIZE * sizeof(char));
    s->top = 0;
}

BOOLEAN isEmpty(const STACK *s) {
    return (s->top == 0) ? TRUE : FALSE;
}

BOOLEAN isFull(const STACK *s) {
    return (s->top == MAX_STACK_SIZE) ? TRUE : FALSE;
}

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

void printStackStatus(const STACK *operand, const STACK *operator) {
    printStack(operand);
    printStack(operator);
    printf("-----------\n");
}

int toInt(const char *str) {
    int n = 0;
    int i = 0;
    while(str[n++] != '\0');
    n--;
    int result=0;

    for(i = 0; i < n; i++) {
        result *= 10;
        result += str[i] - '0';
    }

    return result;
}
