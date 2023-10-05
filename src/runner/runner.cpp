#include "verror.h"
#include "runner.h"

typedef enum
{
    DONT_END,
    END_OUT,
    END_HLT,
}end_calculations_t;

static const int capacity = 20;

static end_calculations_t compare_with_commands(command_t command, FILE *file, struct stack *stk, elem_t *res)
{
    end_calculations_t return_value = DONT_END;

    switch (command)
    {
    case PUSH:
        push(stk, file);
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
        in(stk, file);
        break;
    case OUT:
        *res = out(stk);
        return_value = END_OUT;
        break;
    case HLT:
        hlt(stk);
        return_value = END_HLT;
        break;
    default:
        VERROR("unexpected command");
        break;
    }

    return return_value;
}

elem_t calculate(const char *file_name)
{
    FILE *file = fopen(file_name, "r");
    if(file == NULL)
    {
        VERROR("failed to open", file_name);
    }

    struct stack stk = {};
    STACK_CTOR(&stk, capacity);

    command_t command = NOT_COMMAND;
    elem_t res = 0;

    int is_correctly_read = fscanf(file, "%d", &command);
    while(is_correctly_read != EOF)
    {
        end_calculations_t end_type = compare_with_commands(command, file, &stk, &res);
        if(end_type == END_HLT) break;
        else if(end_type == END_OUT) break;
        is_correctly_read = fscanf(file, "%d", &command);
    }

    fclose(file);
    return res;
}


