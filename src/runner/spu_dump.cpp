#include "stack_func.h"
#include "runner.h"

void spu_dump(struct spu *proc, const char *file, int line, const char *func, const char *arg)
{
    fprintf(stderr, "file %s(%d) in %s\n", file, line, func);
    fprintf(stderr, "SPU %s\n", arg);
    fprintf(stderr, "{\n");
    for(size_t i = 0; i < proc->code_size; i++)
    {
        fprintf(stderr, "\t%2d ", i);
    }
    for(size_t i = 0; i < proc->code_size; i++)
    {
        fprintf(stderr, "%2d ", (int)proc->code[i]);
    }
    fprintf(stderr, "}\n");
}
