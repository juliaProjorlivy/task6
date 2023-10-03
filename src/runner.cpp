#include "commands.h"
#include "verror.h"
#include <stdio.h>

void compare_with_commands(command_t command)
{
    command_t d_commands[] = { PUSH, ADD, SUB, MUL, DIV, SQRT, COS, SIN, IN, OUT, HLT};
    switch (command)
    {
    case PUSH:
        elem_t variable = 0;
        int valid_read = fscanf(file, ELEM_PRINT_SPEC, &variable);
        break;
    case ADD:
        elem_t arg_1 = stack_pop(stk);
        elem_t arg_2 = stack_pop(stk);
        stack_push(stk, arg2 + arg1);
        break;
    case SUB:
        elem_t arg_1 = stack_pop(stk);
        elem_t arg_2 = stack_pop(stk);
        stack_push(stk, arg2 - arg1);
        break;
    case MUL:
        elem_t arg_1 = stack_pop(stk);
        elem_t arg_2 = stack_pop(stk);
        stack_push(stk, arg2 * arg1);
        break;
    case DIV:
        elem_t arg_1 = stack_pop(stk);
        elem_t arg_2 = stack_pop(stk);
        if(arg1 != 0)
        {
            stack_push(stk, arg2 / arg1);
        }
        else
        {

        }
        break;
    case SQRT:
        elem_t arg = stack_pop(stk);
        stack_push(stk, sqrt(arg));
        break;
    case COS:
        
    default:
        break;
    }
}

int assembler(const char *file_name)
{
    FILE *file = fopen(file_name, "r");
    if(file == NULL)
    {
        VERROR("file ",file_name, " cannot be opened");
        return 1;
    }

    command_t command = NOT_COMMAND;
    size_t valid_file = fscanf(file, "%d", &command);
    while(valid_file != EOF)
    {
        switch (command)
        {
        case PUSH:
            
            break;
        
        default:
            break;
        }
        if(command == PUSH || command == IN)
        {

        }
        valid_file = fscanf(file, "%d", &command);
    }

}



