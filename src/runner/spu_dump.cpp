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
    
    while(i_buf < proc->buf_size)
    {
        int has_args = 0;

        fprintf(stderr, "%2zu\t", i_buf);

        struct codes cur_code = *((codes *)(proc->buf + proc->ip_buf));
        fprintf(stderr, "%2d\t", cur_code.op);

        if((has_args = has_arg(&cur_code)))
        {
            arg = *((elem_t *)(proc->buf + i_buf));
            fprintf(stderr, ELEM_PRINT_SPEC "\t", arg);
        }
        if(cur_code.reg)
        {
            fprintf(stderr, "%s" "\t", registers[cur_code.reg - 1].str);
        }

        if(proc->ip_buf == i_buf)
        {
            fprintf(stderr, GREEN "\t <-" END_OF_GREEN);
        }
        fprintf(stderr, "\n");

        i_buf += sizeof(codes);
        i_buf += has_args * (sizeof(elem_t));
    }
      
    fprintf(stderr, "}\n");
}
