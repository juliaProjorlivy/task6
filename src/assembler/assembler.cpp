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


int label_arg(elem_t *arg, const char *str_command, struct labels *Labels)
{
    for(size_t i_label = 0; i_label < Labels->n_filled; i_label++)
    {
        if(!strcmp((*(Labels->all_labels) + i_label)->name, str_command))
        {
            *arg = (elem_t )((*(Labels->all_labels) + i_label)->ip);
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
            VERROR("incorrect argument is given to %s", commands[code->op].str);
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
        VERROR("no needed argument is given to %s", commands[code->op].str);
        return 0;
    }

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

int jump_has_arg(elem_t *arg, struct codes *code, const char *line, struct labels *Labels)
{
    char str_label[max_length] = {};

    if(sscanf(line, "%s", str_label) == EOF)
    {
        VERROR("no needed argument is given for %s", commands[code->op].str);
        return 0;
    }

    if(!label_arg(arg, str_label, Labels))
    {
        *arg = -1;
        return 1;
    }

    return 1;
}

void free_labels(struct labels *Labels, size_t n_in_labels)
{
    for(size_t i = 0; i < n_in_labels; i++)
    {
        free((*(Labels->all_labels) + i)->name);
    }
    free(*(Labels->all_labels));
}

int fill_empty_labels(struct labels *Labels)
{
    size_t i_label = Labels->n_filled;
    for(; i_label < Labels->n_labels; i_label++)
    {
        char *name = (char *)calloc(sizeof(char), label_length);
        if(name == NULL)
        {
            VERROR_MEM;
            free_labels(Labels, i_label);
            return 1;
        }
        (*(Labels->all_labels))[i_label].name = name;
        (*(Labels->all_labels))[i_label].ip = -1;
    }

    return 0;
}

struct label **realloc_labels(struct labels *Labels)
{
    (Labels->n_labels) *= 2;
    struct label *lbls = (struct label *)realloc(*(Labels->all_labels), (Labels->n_labels) *  sizeof(label));

    if(lbls == NULL)
    {
        VERROR_MEM;
        return NULL;
    }
    *(Labels->all_labels) = lbls;
    
    if(fill_empty_labels(Labels))
    {
        VERROR_MEM;
        return NULL;
    }

    return Labels->all_labels;
}

#define CASE_JUMP(NAME)                                                 \
        case NAME:                                                      \
            if(!jump_has_arg(arg, code, line, Labels))                  \
            {                                                           \
                return 1;                                               \
            }                                                           \
            *((codes *)(buf + *i_buf)) = *code;                         \
            (*i_buf) += sizeof(codes);                                  \
            *((elem_t *)(buf + *i_buf)) = *arg;                         \
            (*i_buf) += sizeof(elem_t);                                 \
            return 0


int asm_for_single_line(char *buf, size_t *i_buf, elem_t *arg, const char *line, struct codes *code, struct labels *Labels)
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
        if(label_arg(arg, str_command, Labels))
        {
            return 0;
        }

        if((line[len_com + 1] == ':') && (isspace(line[len_com + 2]) || line[len_com + 2] == 0))
        {
            strncpy((*(Labels->all_labels) + Labels->n_filled)->name, line, (size_t)len_com);
            (*(Labels->all_labels) + Labels->n_filled)->ip = *((ssize_t *)(i_buf));
            (Labels->n_filled)++;
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
int assembler(char *buf, size_t *i_buf, char **lines, size_t n_lines, struct labels *Labels)
{
    *(i_buf) = 0;
   
    struct codes code = {};

    elem_t arg = 0;
    for(size_t line_i = 0; line_i < n_lines; line_i++)
    {
        if(asm_for_single_line(buf, i_buf, &arg, lines[line_i], &code, Labels))
        {
            VERROR("troubles assembling the line \"%s\"", lines[line_i]);
            free_labels(Labels, Labels->n_labels);
            return 1;
        }

        if(Labels->n_filled + 2 >= Labels->n_labels)
        {
            // printf("nlab = %zu, nfill = %zu, labels = %p\n", *n_labels, *n_filled_labels, labels);
            if(!(realloc_labels(Labels)))
            {
                VERROR_MEM;
                return 1;
            }
            // printf("nlab = %zu, nfill = %zu, labels = %p\n", *n_labels, *n_filled_labels, labels);
        }
        
        code = {};
        arg = 0;
    }

    return 0;
}
 
