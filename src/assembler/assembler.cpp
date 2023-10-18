#include "assembler.h"
#include "commands.h"
#include "verror.h"
#include <stdlib.h>
#include <ctype.h>
#include <strings.h>

struct lable
{
    char *name;
    ssize_t ip;
};
const size_t n_lables = 10;

int is_register(struct codes *code, char *str_register)
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

int is_command(struct codes *code, char *str_command)
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

int is_lable(struct codes *code, struct lable **lables, const char *str_command)
{
    for(size_t i_lable = 0; i_lable < n_lables; i_lable++)
    {
        if(!strcmp(lables[i_lable]->name, str_command))
        {
            code->has_arg++;
            code->arg = (elem_t)(lables[i_lable]->ip);
            return 1;
        }
    }

    return 0;
}


int push_has_arg(struct codes *code, const char *line)
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

int pop_has_arg(struct codes *code, const char *line)
{
    char str_register[max_length] = {};

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

int jump_has_arg(struct codes *code, const char *line, struct lable **lables)
{
    char str_lable[max_length] = {};

    if(sscanf(line, "%s", str_lable) == EOF)
    {
        VERROR("no needed argument is given for %s", str_commands[code->op - 1]);
        return 0;
    }

    if(!is_lable(code, lables, str_lable))
    {
        VERROR("incorrect argument is given to %s : %s", str_commands[code->op - 1], str_lable);
        return 0;
    }

    return 1;
}

#define CASE_JUMP(NAME, name, command, line, lables)            \
        case NAME:                                              \
            if(!jump_has_arg((command), (line), (lables)))      \
            {                                                   \
                return 1;                                       \
            }                                                   \
            break                             


// convert line into the struct codes
int asm_for_single_line(const char *line, struct codes *command, struct lable **lables, size_t line_i, size_t *i_in_lables)
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
        char is_colon[max_length] = {};
        if(sscanf(line + 1 + len_com, "%s", is_colon) <= 0)
        {
            VERROR("troubles reading the line");
            return 1;
        }
        if(!strcmp(is_colon, ":"))
        {
            char str_lable[max_length] = {};
            strncpy(str_lable, str_command, strlen(str_command));
            lables[*i_in_lables]->name = str_lable;
            lables[*i_in_lables]->ip = (ssize_t)line_i + 1;
            (*i_in_lables)++;
            return 0;
        }

        VERROR("no such command as %s", str_command);
        return 1;
    }

    line += len_com + 1;
    switch(command->op)
    {
        case PUSH:
            if(!push_has_arg(command, line)) // if not then there is some error
            {
                return 1;
            }
            break;
        case POP:
            if(!pop_has_arg(command, line))
            {
                return 1;
            }
            break;
        CASE_JUMP(JMP, jmp, command, line, lables);
        CASE_JUMP(JA, ja, command, line, lables);
        CASE_JUMP(JAE, jae, command, line, lables);
        CASE_JUMP(JB, jb, command, line, lables);
        CASE_JUMP(JBE, jbe, command, line, lables);
        CASE_JUMP(JNE, jne, command, line, lables);
        CASE_JUMP(JE, je, command, line, lables);
        default:
            break;
    }
    return 0;
}

//                      arr of ptrs of lines | number of lines
struct codes *assembler(char **lines, size_t *n_lines)
{
    struct lable **lables = (struct lable **)calloc(sizeof(lable *), n_lables);
    for(size_t i_lable = 0; i_lable < n_lables; i_lable++)
    {
        lables[i_lable] = (struct lable *)calloc(sizeof(lable), 1);
        // lables[i_lable]->name = (char *)calloc(sizeof(char), max_length);
        lables[i_lable]->ip = -1;
    }
    size_t i_in_lables = 0;

    struct codes *all_codes = (struct codes *)calloc(sizeof(codes), *n_lines);

    for(size_t line_i = 0; line_i < *n_lines; line_i++)
    {
        if(asm_for_single_line(lines[line_i], all_codes + line_i - i_in_lables, lables, line_i, &i_in_lables))
        {
            VERROR("some troubles with asm_for_single_line");
            free(all_codes);
            return NULL;
        }
    }
    
    for(size_t i = 0; i < n_lables; i++)
    {
        // free(lables[i]->name);
        free(lables[i]);
    }
    free(lables);
    
    *n_lines -= i_in_lables;
    return all_codes;
}
