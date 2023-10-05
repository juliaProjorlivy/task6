#include "verror.h"
#include "runner.h"

typedef enum
{
    DONT_END,
    END_HLT,
}whether_to_end;

static const int capacity = 20;

static whether_to_end compare_with_commands(command_t command, FILE *file, struct stack *stk, elem_t *res)
{
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
        in(stk);
        break;
    case OUT:
        *res = out(stk);
        break;
    case HLT:
        hlt(stk);
        return END_HLT;
        break;
    default:
        VERROR("unexpected command");
        break;
    }

    return DONT_END;
}

elem_t calculate(const char *file_name)
{
    FILE *file = fopen(file_name, "r");
    if(file == NULL)
    {
        VERROR_FOPEN(file_name);
        return 1;
    }

    struct stack stk = {};
    STACK_CTOR(&stk, capacity);

    command_t command = NOT_COMMAND;
    elem_t res = 0;

    int is_correctly_read = fscanf(file, "%d", &command);
    while(is_correctly_read != EOF)
    {
        whether_to_end end = compare_with_commands(command, file, &stk, &res);
        if(end == END_HLT) break;
        is_correctly_read = fscanf(file, "%d", &command);
    }

    close_file(file, file_name);
    return res;
}

int main()
{
    printf(ELEM_PRINT_SPEC "\n", calculate("to_calculate_bc.txt"));
    return 0;
}
