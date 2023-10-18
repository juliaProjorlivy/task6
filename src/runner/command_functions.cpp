#include "commands.h"
#include "verror.h"
#include "runner.h"
#include <math.h>
#include <assert.h>                                                                  
    
static int is_equal(elem_t x, elem_t y, double epsilon = 1e-9)
{
    assert (isfinite (x));
    assert (isfinite (y));
    assert (isfinite (epsilon));
  
    return (fabs (x - y) < epsilon);
}

void jmp(struct spu *proc, size_t new_ip_code)
{
    proc->ip_code = new_ip_code; 
    proc->cur_pos = 1; // 1 to include current code->id some bulshit
    for(size_t i_code = 0; i_code < new_ip_code; i_code++ && proc->cur_pos++)
    {
        if(proc->all_codes[i_code].has_arg || proc->all_codes[i_code].reg)
        {
            proc->cur_pos++;
        }
    }
    proc->cur_pos--;
    proc->ip_code--;
}

void ja(struct spu *proc, size_t arg)
{
    MAKE_F_JUMP(>, proc, arg);
}

void jae(struct spu *proc, size_t arg)
{
    MAKE_F_JUMP(>=, proc, arg);
}

void jb(struct spu *proc, size_t arg)
{
    MAKE_F_JUMP(<, proc, arg);
}

void jbe(struct spu *proc, size_t arg)
{
    MAKE_F_JUMP(<=, proc, arg);
}

void jne(struct spu *proc, size_t arg)
{
    MAKE_F_JUMP(!=, proc, arg);
}

void je(struct spu *proc, size_t arg)
{
    MAKE_F_JUMP(==, proc, arg);
}

void hlt(struct stack *stk)
{
    stack_dtor(stk);
}

void pop(struct stack *stk, elem_t *reg)
{
    elem_t arg = 0;
    stack_pop(stk, &arg);
    *reg = arg;
}

void push(struct stack *stk, elem_t *reg, elem_t arg)
{
    if(reg != NULL)
    {
        arg = *reg;
    }
    stack_push(stk, arg);
}

void in(struct stack *stk)
{
    elem_t arg = 0;
    int is_correctly_read = scanf(ELEM_PRINT_SPEC, &arg);

    if(!is_correctly_read)
    {
        VERROR("troubles reading the file");
    }

    stack_push(stk, arg);
}

void add(struct stack *stk)
{
    elem_t arg_1 = 0, arg_2 = 0;
    stack_pop(stk, &arg_1);
    stack_pop(stk, &arg_2);
    stack_push(stk, arg_2 + arg_1);
}

void sub(struct stack *stk)
{
    elem_t arg_1 = 0, arg_2 = 0;
    stack_pop(stk, &arg_1);
    stack_pop(stk, &arg_2);
    stack_push(stk, arg_2 - arg_1);
}

void mul(struct stack *stk)
{
    elem_t arg_1 = 0, arg_2 = 0;
    stack_pop(stk, &arg_1);
    stack_pop(stk, &arg_2);
    stack_push(stk, arg_2 * arg_1);
}

void div(struct stack *stk)
{
    elem_t arg_1 = 0, arg_2 = 0;
    stack_pop(stk, &arg_1);
    stack_pop(stk, &arg_2);

    if(!is_equal(arg_1, 0))
    {
        stack_push(stk, arg_2 / arg_1);
    }
    else
    {
        VERROR("division by zero");
    }
}

void sqroot(struct stack *stk)
{
    elem_t arg = 0;
    stack_pop(stk, &arg);

    if(arg > 0 || is_equal(arg, 0))
    {
        stack_push(stk, sqrt(arg));
    }
    else
    {
        VERROR("taking the root of a negative number");
    }
}

void sinus(struct stack *stk)
{
    elem_t arg = 0;
    stack_pop(stk, &arg);

    stack_push(stk, sin(arg));
}

void cosine(struct stack *stk)
{
    elem_t arg = 0;
    stack_pop(stk, &arg);

    stack_push(stk, cos(arg));
}

void out(struct stack *stk)
{
    elem_t arg = 0;
    stack_pop(stk, &arg);

    printf(ELEM_PRINT_SPEC "\n", arg);
}

