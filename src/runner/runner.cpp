#include "verror.h"
#include "runner.h"

typedef enum
{
    CONTINUE,
    END,
}status;

static const int capacity = 20;

static status compare_with_commands(command_t command, FILE *file, struct stack *stk)
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
        out(stk);
        break;
    case HLT:
        hlt(stk);
        return END;
        break;
    default:
        VERROR("unexpected command");
        break;
    }

    return CONTINUE;
}



int calculate(const char *file_name)
{
    FILE *file = fopen(file_name, "rb");
    if(file == NULL)
    {
        VERROR_FOPEN(file_name);
        return 1;
    } 

    struct stack stk = {};
    STACK_CTOR(&stk, capacity);

    // struct runner spu = {};
    // spu.stk = stk;
    // spu.code = 

    command_t command = NOT_COMMAND;

    int is_correctly_read = fscanf(file, "%d", &command); // TODO: fread
    while(is_correctly_read != EOF)
    {
        status end = compare_with_commands(command, file, &stk);
        if(end == END) break;
        is_correctly_read = fscanf(file, "%d", &command);
    }

    close_file(file, file_name);
    return 0;
}

int main()
{
    printf(ELEM_PRINT_SPEC "\n", calculate("to_calculate_bc.txt"));
    return 0;
}




