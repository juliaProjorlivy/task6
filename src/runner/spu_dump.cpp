#include "commands.h"
#include "spu_dump.h"
#include "runner.h"
#include "verror.h"

void spu_dump(struct spu *proc, const char *file, int line, const char *func, const char *arg)
{
    fprintf(stderr, RED "file %s(%d) in %s\n", file, line, func);
    fprintf(stderr, "SPU %s", arg);
    fprintf(stderr, END_OF_RED "{\n");
    for(size_t i = 0; i < proc->n_codes * 2; i++)
    {
        fprintf(stderr, "\t%2zu ", i);
    }
    fprintf(stderr, "\n");

    for(size_t i_code = 0; i_code < proc->n_codes; i_code++)
    {
        fprintf(stderr, "\t%2d", proc->all_codes[i_code].op);
        if(proc->all_codes[i_code].reg > 0)
        {
            fprintf(stderr, "\t%2d ", proc->all_codes[i_code].reg);
        }
        else if(proc->all_codes[i_code].has_arg)
        {
            fprintf(stderr, "\t%2d", (int)proc->all_codes[i_code].arg);
        }
    }
    fprintf(stderr, "}\n");
}
