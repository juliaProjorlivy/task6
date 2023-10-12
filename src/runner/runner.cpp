#include "commands.h"
#include "runner.h"
#include "verror.h"

status compare_with_commands(command_t command, struct stack *stk, elem_t *reg, elem_t arg)
{
    switch (command)
    {
    case PUSH:
        push(stk, reg, arg);
        break;
    case POP:
        pop(stk, reg);
        break;
    case ADD:
        add(stk);
        break;
    case SUB:
        sub(stk);
        break;
    case MUL:
        mul(stk);
        break;
    case DIV:
        div(stk);
        break;
    case SQRT:
        sqroot(stk);
        break;
    case COS:
        cosine(stk);
        break;
    case SIN:
        sinus(stk);
        break;
    case IN:
        in(stk);
        break;
    case OUT:
        out(stk);
        break;
    case HLT:
        hlt(stk);
        return END;
    case NOT_COMMAND:
        VERROR("no such command as %s", command);
        return END;
    default:
        VERROR("unexpected command %s", command);
        return END;
    }

    return CONTINUE;
}

int runner(struct spu *proc) // maybe copy and not ptr
{
    for(size_t i_code = 0; i_code < proc->n_codes; i_code++)
    {
        elem_t *reg = NULL;
        elem_t arg = proc->all_codes[i_code].arg;
        proc->ip++;

        if(proc->all_codes[i_code].reg > 0) // if has registers
        {
            reg = &(proc->registers.arr_regs[proc->all_codes[i_code].reg - 1]);
            proc->ip++;
        }
        struct stack *stk = proc->stk;
        if(proc->all_codes[i_code].has_arg)
        {
            proc->ip++;
        }

        if(compare_with_commands((command_t)proc->all_codes[i_code].op, stk, reg, arg) == END)
        {
            return 0;
        }
    }
    return 0;
}
