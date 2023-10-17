#include "commands.h"
#include "spu_dump.h"
#include "runner.h"
#include "verror.h"

#define GREEN "\033[92m"
#define END_OF_GREEN "\x1b[39;49m"

void spu_dump(struct spu *proc, const char *file, int line, const char *func, const char *arg)
{
    fprintf(stderr, RED "file %s(%d) in %s\n", file, line, func);
    fprintf(stderr, "SPU %s", arg);
    fprintf(stderr, END_OF_RED "{\n");

    size_t i_com = 0, i_code = 0; 
    // for(size_t i_pos = 0; i_pos < )


    // proc->ip_code = 5;
    for(; i_code < proc->n_codes; i_code++)
    {
        fprintf(stderr, "%2zu\t", i_com);
        fprintf(stderr, "%2d", proc->all_codes[i_code].op);
        if(proc->cur_pos == i_com)
        {
            fprintf(stderr, GREEN "\t <-" END_OF_GREEN);
        }
        fprintf(stderr, "\n");

        if(proc->all_codes[i_code].reg > 0)
        {
            i_com++;
            fprintf(stderr, "%2zu\t", i_com);
            fprintf(stderr, "%2d", proc->all_codes[i_code].reg);
            if(proc->cur_pos == i_com)
            {
                fprintf(stderr, GREEN "\t <-" END_OF_GREEN);
            }
            fprintf(stderr, "\n");
        }
        
        else if(proc->all_codes[i_code].has_arg)
        {
            i_com++;
            fprintf(stderr, "%2zu\t", i_com);
            fprintf(stderr, "%2d", (int)proc->all_codes[i_code].arg);
            if(proc->cur_pos == i_com)
            {
                fprintf(stderr, GREEN "\t <-" END_OF_GREEN);
            }
            fprintf(stderr, "\n");
        }
        
        i_com++;
    }
   
    fprintf(stderr, "}\n");
    // printf("ip_code = %zu\n", proc->ip_code);
    // printf("cur_pos = %zu\n", proc->cur_pos);
}
