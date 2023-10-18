#include "assembler.h"
#include "commands.h"
#include "verror.h"
#include <stdlib.h>
#include <ctype.h>
#include <strings.h>

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

// only for commands and not for single lable so need to remove changes in code
int is_lable(struct codes *code, struct lable **lables, const char *str_command, size_t *n_in_lables)
{
    for(size_t i_lable = 0; i_lable < *n_in_lables; i_lable++)
    {
        if(!strcmp(lables[i_lable]->name, str_command))
        {
            // code->has_arg++;
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

int jump_has_arg(struct codes *code, const char *line, struct lable **lables, size_t *n_in_lables)
{
    char str_lable[max_length] = {};

    if(sscanf(line, "%s", str_lable) == EOF)
    {
        VERROR("no needed argument is given for %s", str_commands[code->op - 1]);
        return 0;
    }

    if(!is_lable(code, lables, str_lable, n_in_lables))
    {

        VERROR("incorrect argument is given to %s : %s", str_commands[code->op - 1], str_lable);
        return 0;
    }

    code->has_arg++;

    return 1;
}

int fill_lables(struct lable **lables, char **lines, size_t n_lines, size_t *i_in_lables)
{
    size_t i_code = 0;
    for(size_t i_line = 0; i_line < n_lines; i_line++)
    {
        char line[max_length] = {};
        char colon[max_length] = {};

        if(sscanf(lines[i_line], "%s", line) <= 0)
        {
            VERROR("troubles reading the line");
            return 1;
        }
        if(sscanf(lines[i_line] + strlen(lines[i_line] + 1), "%s", colon) <= 0)
        {
            VERROR("troubles reading the line");
            return 1;
        }
        if(!strcmp(colon, ":"))
        {
            char *str_lable = (char *)calloc(sizeof(char), max_length);
            strncpy(str_lable, line, strlen(line));
            lables[*i_in_lables]->name = str_lable;
            lables[*i_in_lables]->ip = (ssize_t)i_code;
            (*i_in_lables)++;
            i_code--;
        }
        i_code++;
    }
     
    return 0;
}

#define CASE_JUMP(NAME, code, line, lables)                             \
        case NAME:                                                      \
            if(!jump_has_arg((code), (line), (lables), n_in_lables))    \
            {                                                           \
                return 1;                                               \
            }                                                           \
            break                             


// convert line into the struct codes
int asm_for_single_line(const char *line, struct codes *code, struct lable **lables, size_t *n_in_lables, size_t *i_code)
{
    char str_command[max_length] = {}; //free! TODO: static!
    int len_com = 0;

    if(sscanf(line, "%s%n", str_command, &len_com) <= 0)
    {
        VERROR("troubles reading the line");
        return 1;
    }

    if(!is_command(code, str_command)) // add if
    {
        if(is_lable(code, lables, str_command, n_in_lables))
        {
            return 0;
        }
        VERROR("no such command as %s", str_command);
        return 1;
    }

    (*i_code)++;
    line += len_com + 1;
    switch(code->op)
    {
        case PUSH:
            if(!push_has_arg(code, line)) // if not then there is some error
            {
                return 1;
            }
            break;
        case POP:
            if(!pop_has_arg(code, line))
            {
                return 1;
            }
            break;
        CASE_JUMP(JMP, code, line, lables);
        CASE_JUMP(JA, code, line, lables);
        CASE_JUMP(JAE, code, line, lables);
        CASE_JUMP(JB, code, line, lables);
        CASE_JUMP(JBE, code, line, lables);
        CASE_JUMP(JNE, code, line, lables);
        CASE_JUMP(JE, code, line, lables);
        CASE_JUMP(CALL, code, line, lables);
        default:
            break;
    }

    return 0;
}

static void free_lables(struct lable **lables, size_t n_in_lables)
{
    for(size_t i = 0; i < n_in_lables; i++)
    {
        free(lables[i]->name);
        free(lables[i]);
    }
    free(lables);
}

//                      arr of ptrs of lines | number of lines
struct codes *assembler(char **lines, size_t *n_lines)
{
    struct codes *all_codes = (struct codes *)calloc(sizeof(codes), *n_lines);
    if(all_codes == NULL)
    {
        VERROR_MEM;
        return NULL;
    }
    size_t i_code = 0;

    struct lable **lables = (struct lable **)calloc(sizeof(lable *), n_lables);
    if(lables == NULL)
    {
        VERROR_MEM;
        free(all_codes);
        return NULL;
    }
    for(size_t i_lable = 0; i_lable < n_lables; i_lable++)
    {
        lables[i_lable] = (struct lable *)calloc(sizeof(lable), 1);
        if(lables[i_lable] == NULL)
        {
            VERROR_MEM;
            free(all_codes);
            free_lables(lables, i_lable);
            return NULL;
        }
        lables[i_lable]->ip = -1;
    }
    size_t n_in_lables = 0;

    if(fill_lables(lables, lines, *n_lines, &n_in_lables))
    {
        free(all_codes);
        free_lables(lables, n_lables);
        return NULL;
    }

    for(size_t line_i = 0; line_i < *n_lines; line_i++)
    {
        if(asm_for_single_line(lines[line_i], all_codes + i_code, lables, &n_in_lables, &i_code))
        {
            VERROR("some troubles with asm_for_single_line");
            free(all_codes);
            free_lables(lables, n_lables);
            return NULL;
        }
    }

    free_lables(lables, n_lables);

    (*n_lines) = i_code;    
    return all_codes;
}
 
