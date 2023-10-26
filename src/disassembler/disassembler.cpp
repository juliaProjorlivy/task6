#include "commands.h"
#include "disassembler.h"
#include "verror.h"
#include <stdlib.h>

int has_arg(struct codes *code)
{
    command_t com = (command_t)(code->op);

    return ((com == POP && code->to_ram) || (com == PUSH || com == JMP || com == JA || com == JAE || com == JB || com == JBE || com == JE || com == JNE));
}

const char *is_register(struct codes *code)
{
    for(size_t reg_i = 0; reg_i < n_registers; reg_i++) // if it is one of registers:
    {
        if(code->reg == registers[reg_i].d)
        {
            return registers[reg_i].str;
        }
    }

    return NULL;
}

const char *is_command(struct codes *code)
{
    for(size_t com_i = 1; com_i < n_commands; com_i++)
    {
        if(code->op == commands[com_i].d)
        {
            return commands[com_i].str;
        }
    }

    return NULL;
}

char *dasm_for_single_code(char *buf, size_t *i_buf)
{
    struct codes code = *((codes *)(buf + *i_buf));
    *i_buf += sizeof(codes);
    const char *str_command = is_command(&code);
    if(str_command == NULL)
    {
        VERROR("no such command as %zu", code.op);
        return NULL;
    }

    char *line = (char *)calloc(sizeof(char), max_length);
    if(line == NULL)
    {
        VERROR("memory allocation failure");
        return NULL;
    }
    char *initial_line = line; // ptr on the beginning of the line

    int com_len = 0;
    if(sprintf(line, "%s %n", str_command, &com_len) <= 0)
    {
        VERROR("failed to fill the line");
        free(line);
        return NULL;
    }
    line += com_len;
    if(code.reg)
    {
        const char *str_register = is_register(&code);
        if(str_register == NULL)
        {
            VERROR("no such register as %zu", code.reg);
            free(line);
            return NULL;
        }
        if(code.to_ram)
        {
            line++;
            if(sprintf(line, "[") <= 0)
            {
                VERROR("failed to fill the line");
                return NULL;
            }
        }

        if(sprintf(line, "%s%n", str_register, &com_len) <= 0)
        {
            VERROR("failed to fill the line");
            return NULL;
        }

        if(code.to_ram)
        {
            line++;
            if(sprintf(line, "[") <= 0)
            {
                VERROR("failed to fill the line");
                return NULL;
            }
        }
        line += com_len;
    }

    else if(has_arg(&code))
    {
        if(code.to_ram)
        {
            line++;
            if(sprintf(line, "[") <= 0)
            {
                VERROR("failed to fill the line");
                return NULL;
            }
        }
        if(sprintf(line, ELEM_PRINT_SPEC "%n", *((elem_t *)(buf + *i_buf)), &com_len) <= 0)
        {
            VERROR("failed to fill the line");
            free(line);
            return NULL;
        }
        if(code.to_ram)
        {
            line++;
            if(sprintf(line, "[") <= 0)
            {
                VERROR("failed to fill the line");
                return NULL;
            }
        }
        *i_buf += sizeof(elem_t);
        line += com_len;
    }

    if(sprintf(line, "\n") <= 0)
    {
        VERROR("failed to fill the line");
        free(line);
        return NULL;
    }

    return initial_line;
}

char **disasm(char *buf, size_t buf_size, size_t n_codes)
{
    size_t i_buf = 0;
    size_t i_code = 0;
    char **lines = (char **)calloc(sizeof(const char *), n_codes);
    if(lines == NULL)
    {
        VERROR("memory allocation failure");
        return NULL;
    }

    while(i_buf < buf_size)
    {
        lines[i_code] = dasm_for_single_code(buf, &i_buf);

        if(lines[i_code] == NULL)
        {
            VERROR("some troubles in mini_dasm");
            free(lines);
            return NULL;
        }
        i_code++;
    }

    return lines;
}




