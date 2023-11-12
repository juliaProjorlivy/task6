#include "commands.h"
#include "verror.h"
#include "runner.h"
#include <math.h>
#include <assert.h>    

static inline int is_equal(elem_t x, elem_t y, double epsilon = 1e-9)
{
    assert (isfinite (x));
    assert (isfinite (y));
    assert (isfinite (epsilon));
  
    return (fabs (x - y) < epsilon);
}

#define MAKE_F_JUMP_EQUAL(sign, proc, arg, cur_code)\
        elem_t arg_1 = 0;                           \
        elem_t arg_2 = 0;                           \
        stack_pop((proc->stk), (&arg_1));           \
        stack_pop((proc->stk), (&arg_2));           \
        if(sign(is_equal(arg_1, arg_2)))            \
        {                                           \
            F_JMP((proc), (arg), (cur_code));       \
        }

#define MAKE_F_JUMP_ABE(sign, proc, arg, cur_code)\
        elem_t arg_1 = 0;                           \
        elem_t arg_2 = 0;                           \
        stack_pop((proc->stk), (&arg_1));           \
        stack_pop((proc->stk), (&arg_2));           \
        if(is_equal(arg_1, arg_2))                  \
        {                                           \
            F_JMP((proc), (arg), (cur_code));       \
        }                                           \
        else if((arg_2) sign (arg_1))               \
        {                                           \
            F_JMP((proc), (arg), (cur_code));       \
        }


#define MAKE_F_JUMP(sign, proc, arg, cur_code)      \
        elem_t arg_1 = 0;                           \
        elem_t arg_2 = 0;                           \
        stack_pop((proc->stk), (&arg_1));           \
        stack_pop((proc->stk), (&arg_2));           \
        if((arg_2) sign (arg_1))                    \
        {                                           \
            F_JMP((proc), (arg), (cur_code));       \
        }

#define POP_FUNC                                                \
    elem_t value = 0;                                           \
    stack_pop(proc->stk, &value);                               \
    if(cur_code.reg)                                            \
    {                                                           \
        elem_t *reg = &((proc->arr_regs[cur_code.reg - 1]));    \
        if(cur_code.to_ram)                                     \
        {                                                       \
            proc->ram[(size_t)(*reg)] = value;                  \
        }                                                       \
        else                                                    \
        {                                                       \
            *reg = value;                                       \
        }                                                       \
    }                                                           \
    else                                                        \
    {                                                           \
        proc->ram[(size_t)(arg)] = value;                       \
    }                                                           \


#define PUSH_FUNC                                               \
    if(cur_code.reg)                                            \
    {                                                           \
        elem_t in_reg_value = proc->arr_regs[cur_code.reg - 1]; \
        if(cur_code.to_ram)                                     \
        {                                                       \
            stack_push(proc->stk, proc->ram[(size_t)in_reg_value]);\
        }                                                       \
        else                                                    \
        {                                                       \
            stack_push(proc->stk, in_reg_value);                \
        }                                                       \
    }                                                           \
    else if(cur_code.to_ram)                                    \
    {                                                           \
        stack_push(proc->stk, proc->ram[(size_t)arg]);          \
    }                                                           \
    else                                                        \
    {                                                           \
        stack_push(proc->stk, arg);                             \
    }                                                           

 // TODO: make double comparison with function

#define DEF_CMD(NAME, command_code, n_args, code) status F_##NAME(struct spu *proc, elem_t arg, struct codes cur_code){code return CONTINUE;}
#include "def_cmd.txt"
#undef DEF_CMD

