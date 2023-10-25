#ifndef COMMAND_FUNC_H
#define COMMAND_FUNC_H

#include "stack_func.h"

struct spu
{
    struct stack *stk;
    struct stack *address_stk;
    size_t buf_size; // number of commands in arr all_codes
    // struct codes *all_codes;    // an arr of structures for each command
    char *buf;
    size_t ip_buf;

    elem_t *ram;

    union
    {
        struct
        {
            elem_t rax;
            elem_t rbx;
            elem_t rcx;
            elem_t rdx;    
        } regs;

        elem_t arr_regs[sizeof(regs) / sizeof(elem_t)];

    };
};

typedef enum
{
    CONTINUE = 0,
    END = 1,
} status;

status compare_with_commands(struct spu *proc, elem_t *reg, elem_t arg);

int runner(struct spu *proc);

#define DEF_CMD(NAME, command_code, code) status F_##NAME(struct spu *proc, elem_t arg);
DEF_CMD(PUSH, 1, printf("ddd");)
#include "def_cmd.txt"
#undef DEF_CMD

#endif
