#include "commands.h"
#include "spu_ctor_dtor.h"
#include "runner.h"
#include <stdlib.h>

const int capacity = 5;

void spu_ctor(struct spu *proc, struct codes *all_codes, size_t n_codes)
{
    struct stack *stk = (struct stack *)calloc(sizeof(stack), 1);
    STACK_CTOR(stk, capacity);

    struct stack *address_stk = (struct stack *)calloc(sizeof(stack), 1);
    STACK_CTOR(address_stk, capacity);

    proc->stk = stk;
    proc->address_stk = address_stk;
    proc->n_codes = n_codes;
    proc->ip_code = 0;
    proc->cur_pos = 0;
    proc->all_codes = all_codes;
    proc->registers.arr_regs = (elem_t *)calloc(sizeof(elem_t), n_registers); 
}

void spu_dtor(struct spu *proc)
{
    free(proc->registers.arr_regs);
    free(proc->stk);
    free(proc->all_codes);
}
