#include "commands.h"
#include "spu_ctor_dtor.h"
#include "runner.h"
#include <stdlib.h>

static const int capacity = 5;
static const int ram_size = 20;

void spu_ctor(struct spu *proc, char *buf, size_t buf_size)
{
    struct stack *stk = (struct stack *)calloc(sizeof(stack), 1);
    STACK_CTOR(stk, capacity);

    struct stack *address_stk = (struct stack *)calloc(sizeof(stack), 1);
    STACK_CTOR(address_stk, capacity);

    elem_t *ram = (elem_t *)calloc(sizeof(elem_t), ram_size);

    proc->stk = stk;
    proc->address_stk = address_stk;
    proc->buf_size = buf_size;
    proc->ip_buf = 0;
    proc->buf = buf;
    proc->ram = ram;
    proc->regs.rax = 0;
    proc->regs.rbx = 0;
    proc->regs.rcx = 0;
    proc->regs.rdx = 0;
}

void spu_dtor(struct spu *proc)
{
    free(proc->stk);
    free(proc->address_stk);
    free(proc->buf);
    free(proc->ram);
}
