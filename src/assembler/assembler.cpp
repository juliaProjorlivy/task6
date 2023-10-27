#include "commands.h"
#include "assembler.h"
#include "verror.h"
#include <stdlib.h>
#include <strings.h>

char *to_lower(const char *line, char *new_line)
{
    size_t len = strlen(line);
    for(size_t i = 0; i < len; i++)
    {
        new_line[i] = (char)tolower(line[i]);
    }
    return new_line;
}

int fill_register(struct codes *code, char *str_register)
{
    char compare_line[max_length] = {};
    for(size_t reg_i = 0; reg_i < n_registers; reg_i++) // if it is one of registers:
    {
        if(!strncmp(str_register, to_lower(registers[reg_i].str, compare_line), strlen(registers[reg_i].str)))
        {
            code->reg = (unsigned char )(reg_i + 1) & REG_MASK; 
            return 1;
        }
    }

    return 0;
}

int fill_command(struct codes *code, char *str_command)
{
    char compare_line[max_length] = {};
    for(size_t com_i = 1; com_i < n_commands; com_i++) // the first one is NOT COMMAND
    {
        if(!strncmp(str_command, to_lower(commands[com_i].str, compare_line), strlen(commands[com_i].str)))
        {
            code->op = (unsigned char)(commands[com_i].d) & OP_MASK;
            return 1;
        }
    }

    return 0;
}


int label_arg(elem_t *arg, struct label *labels, const char *str_command, size_t n_filled_labels)
{
    for(size_t i_label = 0; i_label < n_filled_labels; i_label++)
    {
        if(!strcmp((labels + i_label)->name, str_command))
        {
            *arg = (elem_t )((labels + i_label)->ip);
            return 1;
        }
    }

    return 0;
}


int push_has_arg(elem_t *arg, struct codes *code, const char *line)
{
    char str_register[max_length] = {};
    int len = 0;

    if(sscanf(line, "%s%n", str_register, &len) == EOF)
    {
        VERROR("no needed argument is given for push");
        return 0;
    } 

    if(!fill_register(code, str_register)) // if it's not a string then it's a digit, fills code->reg inside is_register function
    {
        if(str_register[0] == '[' && str_register[len - 1] == ']')
        {
            char *str_digit = (char *)calloc(sizeof(char), max_length);
            strncpy(str_digit, str_register + 1, (size_t)len - 2);
            if(sscanf(str_digit, ELEM_PRINT_SPEC, arg) <= 0)
            {
                if(!fill_register(code, str_digit))
                {
                    VERROR("incorrect argument is given to %s", commands[code->op].str);
                    free(str_digit);
                    return 0;
                }
            }
            code->to_ram++;
            free(str_digit);
        }

        else if(sscanf(line, ELEM_PRINT_SPEC, arg) <= 0)
        {
            VERROR("incorrect argument is given to %s", (command_t)code->op);
            return 0;
        }

    }

    return 1;
}

int pop_has_arg(elem_t *arg, struct codes *code, const char *line)
{
    char str_register[max_length] = {};
    int len = 0;

    if(sscanf(line, "%s%n", str_register, &len) == EOF)
    {
        VERROR("no needed argument is given to %s", (command_t)code->op);
        return 0;
    }

    // TODO: rename to fill_register
    if(!fill_register(code, str_register)) // it shouldn't be anything but register. fills code->reg inside is_register function
    {
        if(str_register[0] == '[' && str_register[len - 1] == ']') // pop [5] | pop [rax]
        {
            char *str_digit = (char *)calloc(sizeof(char), max_length);
            strncpy(str_digit, str_register + 1, (size_t)len - 2);

            if(sscanf(str_digit, ELEM_PRINT_SPEC, arg) <= 0)
            {
                if(!fill_register(code, str_digit))
                {
                    VERROR("incorrect argument is given to %s", commands[code->op].str);
                    free(str_digit);
                    return 0;
                }
            }

            code->to_ram++;
            free(str_digit);
            return 1;
        }
        VERROR("incorrect argument is given to %s", commands[code->op].str);
        return 0;
    }

    return 1;
}

int jump_has_arg(elem_t *arg, struct codes *code, const char *line, struct label *labels, size_t *n_filled_labels)
{
    char str_label[max_length] = {};

    if(sscanf(line, "%s", str_label) == EOF)
    {
        VERROR("no needed argument is given for %s", commands[code->op].str);
        return 0;
    }

    if(!label_arg(arg, labels, str_label, *n_filled_labels))
    {
        *arg = -1;
        return 1;
    }

    return 1;
}

void free_labels(struct label *labels, size_t n_in_labels)
{
    for(size_t i = 0; i < n_in_labels; i++)
    {
        free((labels + i)->name);
    }
    free(labels);
}

int fill_empty_labels(struct label *labels, size_t i_label, size_t n_labels)
{
    for(; i_label < n_labels; i_label++)
    {
        char *name = (char *)calloc(sizeof(char), label_length);
        if(name == NULL)
        {
            VERROR_MEM;
            free_labels(labels, i_label);
            return 1;
        }
        labels[i_label].name = name;
        labels[i_label].ip = -1;
    }

    return 0;
}

struct label *fill_labels(size_t *n_labels, size_t *n_filled_labels)
{
    struct label *labels = (struct label *)calloc(sizeof(label), *n_labels);
    if(labels == NULL)
    {
        VERROR_MEM;
        return NULL;
    }
    if(fill_empty_labels(labels, *n_filled_labels, *n_labels))
    {
        VERROR_MEM;
        return NULL;
    }

    return labels;
}

struct label *realloc_labels(size_t *n_labels, size_t *n_filled_labels, struct label *labels)
{
    (*n_labels) *= 2;
    labels = (label *)realloc(labels, (*n_labels) * sizeof(label));
    if(labels == NULL)
    {
        VERROR_MEM;
        return NULL;
    }
    if(fill_empty_labels(labels, *n_filled_labels, *n_labels))
    {
        VERROR_MEM;
        return NULL;
    }

    return labels;
}

#define CASE_JUMP(NAME)                                                 \
        case NAME:                                                      \
            if(!jump_has_arg(arg, code, line, labels, n_filled_labels)) \
            {                                                           \
                return 1;                                               \
            }                                                           \
            *((codes *)(buf + *i_buf)) = *code;                         \
            (*i_buf) += sizeof(codes);                                  \
            *((elem_t *)(buf + *i_buf)) = *arg;                         \
            (*i_buf) += sizeof(elem_t);                                 \
            return 0


int asm_for_single_line(char *buf, size_t *i_buf, elem_t *arg, const char *line, struct codes *code, struct label *labels, size_t *n_filled_labels)
{
    char str_command[max_length] = {};
    int len_com = 0;


    if(sscanf(line, "%s%n", str_command, &len_com) <= 0)
    {
        VERROR("troubles reading the line");
        return 1;
    }

    if(!fill_command(code, str_command))
    {
        if(label_arg(arg, labels, str_command, *n_filled_labels))
        {
            return 0;
        }

        char colon[max_length] = {}; 
        sscanf(line + len_com  + 1, "%s", colon);
        if(!strncmp(":", colon, 1))
        {
            char *str_label = (char *)calloc(sizeof(char), label_length);
            strncpy(str_label, line, (size_t)len_com);
            (labels + *n_filled_labels)->name = str_label;
            (labels + *n_filled_labels)->ip = *((ssize_t *)(i_buf));
            (*n_filled_labels)++;
            return 0;
        }
        VERROR("no such command as %s", str_command);
        return 1;
    }

    line += len_com + 1;
    switch(code->op)
    {
        case PUSH:
            if(!push_has_arg(arg, code, line)) // if not then there is some error
            {
                return 1;
            }
         
            *((codes *)(buf + *i_buf)) = *code;
            (*i_buf) += sizeof(codes);
            if(!code->reg)
            {
                *((elem_t *)(buf + *i_buf)) = *arg;
                (*i_buf) += sizeof(elem_t);
            }
            return 0;
        case POP:
            if(!pop_has_arg(arg, code, line))
            {
                return 1;
            }

            *((codes *)(buf + *i_buf)) = *code;
            (*i_buf) += sizeof(codes);

            if(!code->reg)
            {
                *((elem_t *)(buf + *i_buf)) = *arg;
                (*i_buf) += sizeof(elem_t);
            }
            return 0;
        CASE_JUMP(JMP);
        CASE_JUMP(JA);
        CASE_JUMP(JAE);
        CASE_JUMP(JB);
        CASE_JUMP(JBE);
        CASE_JUMP(JNE);
        CASE_JUMP(JE);
        CASE_JUMP(CALL);
        default:
            *((codes *)(buf + *i_buf)) = *code;
            (*i_buf) += sizeof(codes);
            break;
    }

    return 0;
}

//                      arr of ptrs of lines | number of lines
int assembler(char *buf, size_t *i_buf, char **lines, size_t n_lines, struct label *labels, size_t *n_labels, size_t *n_filled_labels)
{
    *(i_buf) = 0;
   
    struct codes code = {};

    elem_t arg = 0;
    for(size_t line_i = 0; line_i < n_lines; line_i++)
    {
        if(asm_for_single_line(buf, i_buf, &arg, lines[line_i], &code, labels, n_filled_labels))
        {
            VERROR("troubles assembling the line \"%s\"", lines[line_i]);
            // free(all_codes);
            free_labels(labels, *n_labels);
            return 1;
        }

        if(*n_filled_labels + 1 >= *n_labels)
        {
            if(!(labels = realloc_labels(n_labels, n_filled_labels, labels)))
            {
                VERROR_MEM;
                return 1;
            }
        }
        
        code = {};
        arg = 0;
    }

    // (*i_buf)++; // starts from the 0

    return 0;
}
 
