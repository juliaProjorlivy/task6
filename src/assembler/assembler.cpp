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


elem_t *label_arg(struct label *labels, const char *str_command, size_t n_filled_labels)
{
    for(size_t i_label = 0; i_label < n_filled_labels; i_label++)
    {
        if(!strcmp((labels + i_label)->name, str_command))
        {
            return ((elem_t *)(&((labels + i_label)->ip)));
        }
    }

    return NULL;
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

    if((arg = label_arg(labels, str_label, *n_filled_labels)) == NULL)
    {
        VERROR("incorrect argument %s is given to %s", str_label, commands[code->op].str);
        return 0;
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

struct label *fill_labels(char **lines, size_t n_lines, size_t *n_labels, size_t *n_filled_labels)
{
    size_t i_buf = 0;
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

    for(size_t i_line = 0; i_line < n_lines; i_line++)
    {
        if(*n_filled_labels + 1 >= *n_labels)
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
        }
        char line[label_length] = {};
        char colon[max_length] = {};
        int len_line = 0;

        if(sscanf(lines[i_line], "%s%n", line, &len_line) <= 0)
        {
            VERROR("troubles reading the line");
            return NULL;
        }
        sscanf(lines[i_line] + len_line + 1, "%s", colon);

        if(!strcmp(colon, ":"))
        {
            char *str_label = (char *)calloc(sizeof(char), label_length);
            strncpy(str_label, line, (size_t)len_line);
            (labels + *n_filled_labels)->name = str_label;
            (labels + *n_filled_labels)->ip = (ssize_t)i_buf;
            (*n_filled_labels)++;
        }
        else
        {
            i_buf += sizeof(codes);
            elem_t digit = 0;
            if(sscanf(lines[i_line] + len_line + 1, ELEM_PRINT_SPEC, &digit))
            {
                i_buf += sizeof(elem_t);
            }
        }
    }
     
    return labels;
}

#define CASE_JUMP(NAME)                                                 \
        case NAME:                                                      \
            if(!jump_has_arg(arg, code, line, labels, &n_filled_labels))\
            {                                                           \
                return 1;                                               \
            }                                                           \
            fill_buf(buf, i_buf, (char *)arg, sizeof(elem_t));          \
            break                             

// int jump_arg(elem_t *arg, struct codes *code, const char *line, struct label *labels, size_t *n_filled_labels)
// convert line into the struct codes
void fill_buf(char *buf, size_t *i_buf, char *arg, size_t size)
{
    for(size_t i = 0; i < size; i++)
    {
        (*i_buf)++;
        *(buf + *i_buf) = *(arg + i);
    }
}

int asm_for_single_line(char *buf, size_t *i_buf, elem_t *arg, const char *line, struct codes *code, struct label *labels, size_t n_filled_labels, size_t *i_code)
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
        if((arg = label_arg(labels, str_command, n_filled_labels)) != NULL)
        {
            return 0;
        }
        VERROR("no such command as %s", str_command);
        return 1;
    }

    fill_buf(buf, i_buf, (char *)(code), sizeof(codes));
    // *buf = *((char *)code);
    // buf += sizeof(codes);

    (*i_code)++;
    line += len_com + 1;
    switch(code->op)
    {
        case PUSH:
            if(!push_has_arg(arg, code, line)) // if not then there is some error
            {
                return 1;
            }
            fill_buf(buf, i_buf, (char *)arg, sizeof(elem_t));
            break;
        case POP:
            if(!pop_has_arg(arg, code, line))
            {
                return 1;
            }
            if(!code->reg)
            {
                fill_buf(buf, i_buf, (char *)arg, sizeof(elem_t));
            }
            break;
        CASE_JUMP(JMP);
        CASE_JUMP(JA);
        CASE_JUMP(JAE);
        CASE_JUMP(JB);
        CASE_JUMP(JBE);
        CASE_JUMP(JNE);
        CASE_JUMP(JE);
        CASE_JUMP(CALL);
        default:
            break;
    }

    return 0;
}

//                      arr of ptrs of lines | number of lines
struct codes *assembler(char *buf, size_t *i_buf, char **lines, size_t *n_lines)
{
    size_t n_labels = 10;
    size_t n_filled_labels = 0;
    struct label *labels = fill_labels(lines, *n_lines, &n_labels, &n_filled_labels);
    if(labels == NULL)
    {
        VERROR_MEM;
        return NULL;
    }

    size_t i_code = 0;
    struct codes *all_codes = (struct codes *)calloc(sizeof(codes), *n_lines);
    if(all_codes == NULL)
    {
        VERROR_MEM;
        free_labels(labels, n_labels);
        return NULL;
    }

    elem_t arg = 0;
    for(size_t line_i = 0; line_i < *n_lines; line_i++)
    {
        if(asm_for_single_line(buf, i_buf, &arg, lines[line_i], all_codes + i_code, labels, n_filled_labels, &i_code))
        {
            VERROR("troubles assembling the line \"%s\"", lines[line_i]);
            free(all_codes);
            free_labels(labels, n_labels);
            return NULL;
        }
    }

    free_labels(labels, n_labels);

    (*n_lines) = i_code;    
    return all_codes;
}
 
