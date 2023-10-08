#ifndef COMMAND_FUNC_H
#define COMMAND_FUNC_H

#include "stack_func.h"
#include "commands.h"
#include "stack_error.h"
#include <limits.h>

struct spu
{
    struct stack *stk;
    size_t code_size;
    char *code; // using char to calculate everything in bytes. 
                //isnide different types of arguments(elem_t for stack value and int for codes)
    char *ip;
    int *registers;
    int rax;
    int rbx;
    int rcx;
    int rdx;
};

const char code_poison = CHAR_MAX;

void hlt(struct stack *stk);

void push(struct stack *stk, FILE *file);

void in(struct stack *stk);

void add(struct stack *stk);

void sub(struct stack *stk);

void mul(struct stack *stk);

void div(struct stack *stk);

void sqroot(struct stack *stk);

void sinus(struct stack *stk);

void cosine(struct stack *stk);

void out(struct stack *stk);

int calculate(const char *file_name);

#endif
