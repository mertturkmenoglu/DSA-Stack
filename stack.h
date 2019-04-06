#ifndef EXPEVAL_STACK_H
#define EXPEVAL_STACK_H

#define MAX_STACK_SIZE 100
#define MAX_INPUT_SIZE 100
#define FALSE 0
#define TRUE 1

enum OPERATION_TYPE {
    OPERAND, OPERATOR
};

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

#endif //EXPEVAL_STACK_H