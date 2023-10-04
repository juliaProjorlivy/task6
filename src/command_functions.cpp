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

void hlt(struct stack *stk)
{
    stack_dtor(stk);
}

void push(struct stack *stk, FILE *file)
{
    elem_t arg = 0;
    int is_correctly_read = fscanf(file, ELEM_PRINT_SPEC, &arg);

    if(!is_correctly_read)
    {
        VERROR("troubles reading the file");
    }

    stack_push(stk, arg);
}

void in(struct stack *stk, FILE *file)
{
    elem_t arg = 0;
    int is_correctly_read = fscanf(file, ELEM_PRINT_SPEC, &arg);

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

    if(is_equal(arg_1, 0))
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

elem_t out(struct stack *stk)
{
    elem_t arg = 0;
    stack_pop(stk, &arg);

    return arg;
}
