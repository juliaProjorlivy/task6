#ifndef COMMAND_FUNC_H
#define COMMAND_FUNC_H

#include "stack_func.h"

struct spu
{
    struct stack *stk;
    size_t n_codes;
    struct codes *all_codes;    // an arr of structures for each command
    struct codes *ip;                   // a ptr to the particular command in all_codes

    union
    {
        struct
        {
            elem_t rax;
            elem_t rbx;
            elem_t rcx;
            elem_t rdx;    
        } regs;

        elem_t *arr_regs;

    } registers;
};

typedef enum
{
    CONTINUE = 0,
    END = 1,
} status;

// status compare_with_commands(command_t command, struct spu *proc);

status compare_with_commands(command_t command, struct stack *stk, elem_t *reg, elem_t arg);

int runner(struct spu *proc);

void hlt(struct stack *stk);

void pop(struct stack *stk, elem_t *reg);

void push(struct stack *stk, elem_t *reg, elem_t arg);

void in(struct stack *stk);

void add(struct stack *stk);

void sub(struct stack *stk);

void mul(struct stack *stk);

void div(struct stack *stk);

void sqroot(struct stack *stk);

void sinus(struct stack *stk);

void cosine(struct stack *stk);

void out(struct stack *stk);

#endif
