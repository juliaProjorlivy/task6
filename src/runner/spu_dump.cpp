#include "commands.h"
#include "spu_dump.h"
#include "runner.h"
#include "verror.h"

#define GREEN "\033[92m"
#define END_OF_GREEN "\x1b[39;49m"

void spu_dump(struct spu *proc, const char *file, int line, const char *func, const char *arg_draw)
{
    fprintf(stderr, RED "file %s(%d) in %s\n", file, line, func);
    fprintf(stderr, "SPU %s", arg_draw);
    fprintf(stderr, END_OF_RED "{\n");

    size_t i_buf = 0;
    elem_t arg = 0;
    size_t i_code = 0;
    
    while(i_buf < proc->buf_size)
    {
        int has_args = 0;
        int print_arrow = 0;

        fprintf(stderr, "%2zu\t", i_code);

        struct codes cur_code = *((codes *)(proc->buf + i_buf));
        fprintf(stderr, "%2d\t", cur_code.op);
        if(proc->ip_buf == i_buf)
        {
            print_arrow = 1;
        }
        i_buf += sizeof(codes);

        if((has_arg(&cur_code)))
        {
            arg = *((elem_t *)(proc->buf + i_buf));
            fprintf(stderr, ELEM_PRINT_SPEC "\t", arg);
            i_buf += sizeof(elem_t);
        }
        if(cur_code.reg)
        {
            fprintf(stderr, "%s" "\t", registers[cur_code.reg - 1].str);
        }

        if(print_arrow)
        {
            fprintf(stderr, GREEN "\t <-" END_OF_GREEN);
        }
        fprintf(stderr, "\n");
    
        i_code++;
    }
      
    fprintf(stderr, "}\n");
}
