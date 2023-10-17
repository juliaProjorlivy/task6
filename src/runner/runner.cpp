#include "commands.h"
#include "runner.h"
#include "verror.h"

status compare_with_commands(command_t command, struct stack *stk, elem_t *reg, elem_t arg, size_t *ip)
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
    // case JMP(ip):
    //     jmp(ip);
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
    // size_t i_code = proc->ip_code;
    for(; proc->ip_code < proc->n_codes; proc->ip_code++ && proc->cur_pos++)
    {
        elem_t *reg = NULL;
        elem_t arg = proc->all_codes[proc->ip_code].arg;
        // proc->cur_pos += (proc->cur_pos) ? 1 : 0;
        // proc->ip_code += (proc->ip_code) ? 1 : 0;

        if(proc->all_codes[proc->ip_code].reg > 0) // if has registers
        {
            reg = &(proc->registers.arr_regs[proc->all_codes[proc->ip_code].reg - 1]);
            proc->cur_pos++;
        }
        else if(proc->all_codes[proc->ip_code].has_arg)
        {
            proc->cur_pos++;
        }

        struct stack *stk = proc->stk;
        if(compare_with_commands((command_t)proc->all_codes[proc->ip_code].op, stk, reg, arg, &(proc->ip_code)) == END)
        {
            proc->ip_code++;
            proc->cur_pos++;
            return 0;
        }
    }
    return 0;
}
