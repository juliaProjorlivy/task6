#include "spu_ctor_dtor.h"
#include "stack_func.h"
#include "runner.h"
#include <stdlib.h>
#include <limits.h>

const int capacity = 20;


void spu_ctor(struct spu *proc, size_t code_size)
{
    struct stack stk = {};
    STACK_CTOR(&stk, capacity);

    proc->stk = &stk;
    proc->code = (char *)calloc(sizeof(char), code_size);
    for(size_t i = 0; i < code_size; i++)
    {
        *(proc->code + i) = code_poison; /// every bity fills with posion 
    }
    proc->code_size = code_size;
    char *ip = proc->code;
    
}

void spu_dtor(struct spu *proc)
{
    for(size_t i = 0; i < proc->code_size; i++)
    {
        *(proc->code + i) = code_poison;
    }

    free(proc->code);
}
