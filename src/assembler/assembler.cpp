#include "assembler.h"
#include "commands.h"
#include "verror.h"
#include <stdlib.h>
#include <ctype.h>

static int is_register(struct codes *code, char *str_register)
{
    for(size_t reg_i = 0; reg_i < n_registers; reg_i++) // if it is one of registers:
    {
        if(!strcmp(str_register, str_registers[reg_i]))
        {
            code->most = d_most_bit[reg_i + 1]; 
            return 1;
        }
    }
    return 0;
}

static int is_push_or_in(struct codes *code, const char *line)
{
    if(code->least == PUSH || code->least == IN)
    {
        char *str_register = (char *)calloc(sizeof(char), max_length);
        if(str_register == NULL)
        {
            VERROR("memory allocation failure");
            return 0;
        }
        size_t arg = 0;

        if(sscanf(line, "%s", str_register) > 0) 
        {
            if(!is_register(code, str_register)) // if it's not a string than it's a digit
            {
                if(sscanf(line, "%d", &arg) == EOF)
                {
                    VERROR("cannot read the line");
                    free(str_register);
                    return 0;
                }
            }
            else
            {
                line += strlen(str_register) + 1;
                if(sscanf(line, "%d", &arg) == EOF)
                {
                    VERROR("no needed argument");
                    free(str_register);
                    return 0;
                }
            }
            code->arg = arg;
            code->have_arg = 1;
            free(str_register);
            return 1;
        }
        else
        {
            VERROR("no needed argument");
            free(str_register);
            return 0;
        }
    }

    return 0;
}

static int is_pop_or_out(struct codes *code, const char *line)
{
    if(code->least == POP || code->least == OUT)
    {
        char *str_register = (char *)calloc(sizeof(char), max_length);

        if(sscanf(line, "%s", str_register) > 0)
        {
            if(!is_register(code, str_register)) // if it is than code.most is already filled
            {
                VERROR("invalid argument");
                free(str_register);
                return 0;
            }
        }
        free(str_register);
        return 1;
    }

    return 0;
}

int mini_assembler(const char *line, struct codes *command)  // convert line into the struct codes
{
    char *str_command = (char *)calloc(sizeof(char), max_length); //free!
    if(str_command == NULL)
    {
        VERROR("memory allocation problem");
        return 1;
    }

    if(sscanf(line, "%s", str_command) == EOF)
    {
        VERROR("troubles reading the line");
        free(str_command);
        return 1;
    }

    for(size_t i = 0; i < n_commands; i++)
    {
        if(!(strcmp(str_command, str_commands[i]))) // comaring input command with what we have
        {
            command->least = d_commands[i]; // filling the bit field
            line += strlen(str_command) + 1;
            if(command->least == PUSH || command->least == IN)
            {
                if(!is_push_or_in(command, line)) // if not there is some error
                {
                    free(str_command);
                    return 1;
                }
            }
            else if(command->least == POP || command->least == OUT) // if not there is some error
            {
                if(!is_pop_or_out(command, line))
                {
                    free(str_command);
                    return 1;
                }
            }
        }
        else
        {
            VERROR("no such command as %s", str_command);
            free(str_command);
            return 1;
        }
    }
   
    free(str_command);
    return 0;
}

int assembler(struct codes *all_codes, char **lines, size_t n_lines)
{
    for(size_t i = 0; i < n_lines; i++)
    {
        if(mini_assembler(*(lines + i), all_codes + i))
        {
            return 1;
        }
    }

    return 0;
}


// int main()
// {
//     convert_file("to_calculate.txt", "to_calculate_bc.txt");
//     return 0;
// }


