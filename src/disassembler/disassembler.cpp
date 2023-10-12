#include "commands.h"
#include "disassembler.h"
#include "verror.h"
#include <stdlib.h>

const char *is_register(struct codes *code)
{
    for(size_t reg_i = 0; reg_i < n_registers; reg_i++) // if it is one of registers:
    {
        if(code->reg == d_registers[reg_i])
        {
            return str_registers[reg_i];
        }
    }

    return NULL;
}

const char *is_command(struct codes *code)
{
    for(size_t com_i = 0; com_i < n_commands; com_i++)
    {
        if(code->op == d_commands[com_i])
        {
            return str_commands[com_i];
        }
    }

    return NULL;
}

const char *mini_disasm(struct codes *code)
{
    const char *str_command = is_command(code);
    if(str_command == NULL)
    {
        VERROR("no such command as %zu", code->op);
        return NULL;
    }

    char *line = (char *)calloc(sizeof(char), max_length);
    if(line == NULL)
    {
        VERROR("memory allocation failure");
        return NULL;
    }

    int com_len = 0;
    if(sprintf(line, "%s %n", str_command, &com_len) <= 0)
    {
        VERROR("failed to fill the line");
        free(line);
        return NULL;
    }
    line += com_len;
    if(code->reg > 0)
    {
        const char *str_register = is_register(code);
        if(str_register == NULL)
        {
            VERROR("no such register as %zu", code->reg);
            free(line);
            return NULL;
        }

        if(sprintf(line, "%s", str_register) <= 0)
        {
            VERROR("failed to fill the line");
            return NULL;
        }
    }

    else if(code->has_arg)
    {
        if(sprintf(line, ELEM_PRINT_SPEC, code->arg) <= 0)
        {
            VERROR("failed to fill the line");
            free(line);
            return NULL;
        }
    }

    if(sprintf(line, "\n") <= 0)
    {
        VERROR("failed to fill the line");
        free(line);
        return NULL;
    }

    return (const char *)line;
}

const char **disasm(struct codes *all_codes, size_t n_codes)
{
    const char **lines = (const char **)calloc(sizeof(const char *), n_codes);
    if(lines == NULL)
    {
        VERROR("memory allocation failure");
        return NULL;
    }

    for(size_t i_code = 0; i_code < n_codes; i_code++)
    {
        lines[i_code] = mini_disasm(all_codes + i_code);

        if(lines[i_code] == NULL)
        {
            VERROR("some troubles in mini_dasm");
            free(lines);
            return NULL;
        }
    }

    return lines;
}




