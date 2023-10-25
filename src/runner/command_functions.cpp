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

#define MAKE_F_JUMP(sign, proc, arg)                \
        elem_t arg_1 = 0;                           \
        elem_t arg_2 = 0;                           \
        stack_pop((proc->stk), (&arg_1));           \
        stack_pop((proc->stk), (&arg_2));           \
        if((arg_1) sign (arg_2))                    \
        {                                           \
            F_JMP((proc), (arg));                   \
        }

#define POP_FUNC                                                \
    elem_t value = 0;                                           \
    stack_pop(proc->stk, &value);                               \
    struct codes cur_code = *((codes *)(proc->buf + proc->ip_buf));\
    if(cur_code.reg)                                            \
    {                                                           \
        elem_t *reg = &((proc->arr_regs[cur_code.reg - 1]));    \
        if(cur_code.to_ram)                                     \
        {                                                       \
            proc->ram[*((size_t *)reg)] = value;                \
        }                                                       \
        else                                                    \
        {                                                       \
            *reg = value;                                       \
        }                                                       \
    }                                                           \
    else                                                        \
    {                                                           \
        proc->ram[(size_t)(arg)] = value;                       \
        proc->ip_buf += sizeof(elem_t);                         \
    }                                                           \


#define PUSH_FUNC                                               \
    struct codes cur_code = *((codes *)((proc->buf) + proc->ip_buf));\
    if(cur_code.reg)                                            \
    {                                                           \
        size_t in_reg_value = (size_t)(proc->arr_regs[cur_code.reg - 1]);\
        if(cur_code.to_ram)                                     \
        {                                                       \
            stack_push(proc->stk, proc->ram[in_reg_value]);     \
        }                                                       \
        else                                                    \
        {                                                       \
            stack_push(proc->stk, in_reg_value);                \
        }                                                       \
    }                                                           \
    else if(cur_code.to_ram)                                    \
    {                                                           \
        stack_push(proc->stk, proc->ram[(size_t)arg]);           \
    }                                                           \
    else                                                        \
    {                                                           \
        stack_push(proc->stk, arg);                             \
    }                                                           \
    proc->ip_buf += sizeof(elem_t);

#define DEF_CMD(NAME, command_code, code) status F_##NAME(struct spu *proc, elem_t arg){code return CONTINUE;}
#include "def_cmd.txt"
#undef DEF_CMD

// #define DEF_CMD(name, code) name(proc, elem_t arg) {code}

// cmds.txt:
// DEF_CMD(JA, {MAKE_F_JUMP()})
