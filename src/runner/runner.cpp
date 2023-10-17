#include "commands.h"
#include "runner.h"
#include "verror.h"

status compare_with_commands(struct spu *proc, elem_t *reg, elem_t arg)
{
    command_t command = (command_t)proc->all_codes[proc->ip_code].op;
    
    switch (command)
    {
    case PUSH:
        push(proc->stk, reg, arg);
        break;
    case POP:
        pop(proc->stk, reg);
        break;
    case ADD:
        add(proc->stk);
        break;
    case SUB:
        sub(proc->stk);
        break;
    case MUL:
        mul(proc->stk);
        break;
    case DIV:
        div(proc->stk);
        break;
    case SQRT:
        sqroot(proc->stk);
        break;
    case COS:
        cosine(proc->stk);
        break;
    case SIN:
        sinus(proc->stk);
        break;
    case IN:
        in(proc->stk);
        break;
    case OUT:
        out(proc->stk);
        break;
    case HLT:
        hlt(proc->stk);
        return END;
    // case JMP(&((size_t)arg)):
    //     jmp(&((size_t)arg));
    //     break;
    case NOT_COMMAND:
        VERROR("no such command as %s", command);
        return END;
    default:
        VERROR("unexpected command %s", command);
        return END;
    }

    return CONTINUE;
}

int runner(struct spu *proc)
{
    proc->cur_pos = 0;
    proc->ip_code = 0;

    for(; proc->ip_code < proc->n_codes; proc->ip_code++ && proc->cur_pos++)
    {
        elem_t *reg = NULL;
        elem_t arg = proc->all_codes[proc->ip_code].arg;

        if(proc->all_codes[proc->ip_code].reg > 0) // if has registers
        {
            reg = &(proc->registers.arr_regs[proc->all_codes[proc->ip_code].reg - 1]);
            proc->cur_pos++;
        }
        else if(proc->all_codes[proc->ip_code].has_arg)
        {
            proc->cur_pos++;
        }

        if(compare_with_commands(proc, reg, arg) == END) // put jump arg in arg
        {
            proc->ip_code++;
            proc->cur_pos++;
            return 0;
        }
    }
    return 0;
}
