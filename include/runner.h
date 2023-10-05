#ifndef COMMAND_FUNC_H
#define COMMAND_FUNC_H

#include "stack_func.h"
#include "commands.h"
#include "stack_error.h"

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

elem_t out(struct stack *stk);

elem_t calculate(const char *file_name);

#endif
