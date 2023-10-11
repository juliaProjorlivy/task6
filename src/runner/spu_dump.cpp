#include "commands.h"
#include "stack_func.h"
#include "runner.h"

void spu_dump(struct spu *proc, const char *file, int line, const char *func, const char *arg)
{
    fprintf(stderr, "file %s(%d) in %s\n", file, line, func);
    fprintf(stderr, "SPU %s\n", arg);
    fprintf(stderr, "{\n");
    for(size_t i = 0; i < 2 * proc->n_codes; i++)
    {
        fprintf(stderr, "\t%2zu ", i);
    }
    for(size_t i_code = 0; i_code < proc->n_codes; i_code++)
    {
        fprintf(stderr, "%2d ", proc->all_codes[i_code].op);
        if(proc->all_codes[i_code].reg > 0)
        {
            fprintf(stderr, "%2d ", proc->all_codes[i_code].reg);
        }
        else if(proc->all_codes[i_code].has_arg)
        {
            fprintf(stderr, ELEM_PRINT_SPEC, proc->all_codes[i_code].arg);
        }
    }
    fprintf(stderr, "}\n");
}
