#ifndef COMMAND_FUNC_H
#define COMMAND_FUNC_H

#include "stack_func.h"

struct spu
{
    struct stack *stk;
    size_t n_codes; // number of commands in arr all_codes
    struct codes *all_codes;    // an arr of structures for each command
    size_t ip_code;
    size_t cur_pos;

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

status compare_with_commands(struct spu *proc, elem_t *reg, elem_t arg);

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

#define DEF_JUMP_F(name) void name(struct spu *proc, size_t arg)
DEF_JUMP_F(jmp);
#define MAKE_F_JUMP(sign, proc, arg)                \
        elem_t arg_1 = 0;                           \
        elem_t arg_2 = 0;                           \
        stack_pop((proc->stk), (&arg_1));           \
        stack_pop((proc->stk), (&arg_2));           \
        if((arg_1) sign (arg_2))                    \
        {                                           \
            jmp((proc), (arg));                     \
        }   
DEF_JUMP_F(ja);
DEF_JUMP_F(j);
DEF_JUMP_F(jmp);
DEF_JUMP_F(jmp);
DEF_JUMP_F(jmp);

#endif
