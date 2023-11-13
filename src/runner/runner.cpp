#include "commands.h"
#include "runner.h"
#include "verror.h"
#include "spu_dump.h"

#define DEF_CMD(NAME, command_code, n_args, code)   \
        case NAME:                                  \
            return F_##NAME(proc, arg, cur_code);


status compare_with_commands(struct spu *proc, elem_t arg, struct codes cur_code)
{
    command_t command = (command_t)(cur_code.op);
    
    switch (command)
    {
#include "def_cmd.txt"
        default:
            VERROR("unexpected command \"%s\"", commands[command].str);
            return END;
    }

    return END; // SMTH WENT WRONG
}
#undef DEF_CMD

#define DEF_CMD(NAME, command_code, n_args, code)   \
        case NAME:                                  \
            return (n_args);

size_t has_arg(struct codes *code)
{
    command_t com = (command_t)(code->op);

    if(com == PUSH || com == POP)
    {
        return (!(code->reg));
    }
    switch (com)
    {
#include "def_cmd.txt"
        default:
            VERROR("unexpected command");
            return 0;
    }
}
#undef DEF_CMD

int runner(struct spu *proc)
{
    elem_t arg = 0;

    while(proc->ip_buf < proc->buf_size)
    {
        struct codes code = *((codes *)(proc->buf + proc->ip_buf));
    
        if(has_arg(&code))
        {
            arg = *((elem_t *)(proc->buf + proc->ip_buf + sizeof(codes)));
        }
        proc->ip_buf += sizeof(codes) + has_arg(&code) * sizeof(elem_t);
        // SPU_DUMP(proc);
        if(compare_with_commands(proc, arg, code) == END)
        {
            proc->ip_buf += sizeof(codes);
            return 0;
        }
        
        arg = 0;
    }

    return 0;
}
