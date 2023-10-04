#ifndef COMMAND_FUNC_H
#define COMMAND_FUNC_H

#include "stack_func.h"
#include "commands.h"

void hlt(struct stack *stk);

void push(struct stack *stk, FILE *file);

void in(struct stack *stk, FILE *file);

void add(struct stack *stk);

void sub(struct stack *stk);

void mul(struct stack *stk);

void div(struct stack *stk);

void sqroot(struct stack *stk);

void sinus(struct stack *stk);

void cosine(struct stack *stk);

elem_t out(struct stack *stk);

elem_t calculate(const char *file_name);
// elem_t compare_with_commands(command_t command, FILE *file, struct stack *stk);

#endif
