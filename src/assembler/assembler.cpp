#include "assembler.h"
#include "commands.h"
#include "verror.h"
#include <stdlib.h>
#include <ctype.h>
#include <strings.h>

static int is_register(struct codes *code, char *str_register)
{
    for(size_t reg_i = 0; reg_i < n_registers; reg_i++) // if it is one of registers:
    {
        if(!strcmp(str_register, str_registers[reg_i]))
        {
            code->reg = (unsigned char )(reg_i + 1) & REG_MASK; 
            return 1;
        }
    }

    return 0;
}

static int is_command(struct codes *code, char *str_command)
{
    for(size_t com_i = 0; com_i < n_commands; com_i++)
    {
        if(!strcmp(str_command, str_commands[com_i]))
        {
            code->op = (unsigned char)(com_i + 1) & OP_MASK;
            return 1;
        }
    }

    return 0;
}

static int is_push(struct codes *code, const char *line)
{
    char str_register[max_length] = {};
    elem_t arg = 0;

    if(sscanf(line, "%s", str_register) == EOF)
    {
        VERROR("no needed argument is given for push");
        return 0;
    } 

    if(!is_register(code, str_register)) // if it's not a string then it's a digit, fills code->reg inside is_register function
    {
        if(sscanf(line, ELEM_PRINT_SPEC, &arg) <= 0)
        {
            VERROR("incorrect argument is given for push");
            return 0;
        }

        code->arg = arg;
        code->has_arg = 1;
    }

    return 1;
}

static int is_pop(struct codes *code, const char *line)
{
    char str_register[max_length] = {}; // memory for register string

    if(sscanf(line, "%s", str_register) == EOF)
    {
        VERROR("no needed argument is given for pop");
        return 0;
    }

    if(!is_register(code, str_register)) // it shouldn't be anything but register. fills code->reg inside is_register function
    {
        VERROR("incorrect argument is given to pop %s", str_register);
        return 0;
    }

    return 1;
}

int asm_for_single_line(const char *line, struct codes *command)  // convert line into the struct codes
{
    char str_command[max_length] = {}; //free! TODO: static!
    int len_com = 0;

    if(sscanf(line, "%s%n", str_command, &len_com) <= 0)
    {
        VERROR("troubles reading the line");
        return 1;
    }

    if(!is_command(command, str_command))
    {
        VERROR("no such command as %s", str_command);
        return 1;
    }

    line += len_com + 1;
    if(command->op == PUSH)
    {
        if(!is_push(command, line)) // if not then there is some error
        {
            return 1;
        }
    }
    else if(command->op == POP) // if not then there is some error
    {
        if(!is_pop(command, line))
        {
            return 1;
        }
    }

    return 0;
}

//                      arr of ptrs of lines | number of lines
struct codes *assembler(char **lines, size_t n_lines)
{
    struct codes *all_codes = (struct codes *)calloc(sizeof(codes), n_lines);

    for(size_t line_i = 0; line_i < n_lines; line_i++)
    {
        if(asm_for_single_line(lines[line_i], all_codes + line_i))
        {
            VERROR("some troubles with asm_for_single_line");
            free(all_codes);
            return NULL;
        }
    }

    return all_codes;
}
