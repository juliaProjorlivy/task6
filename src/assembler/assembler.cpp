#include "assembler.h"
#include "commands.h"
#include "verror.h"
#include <stdlib.h>

command_t d_commands[] = { PUSH, ADD, SUB, MUL, DIV, SQRT, COS, SIN, IN, OUT, HLT};
const char *str_commands[] = {"push", "add", "sub", "mul", "div", "sqrt", "cos", "sin", "in", "out", "hlt"};
size_t n_commands = sizeof(str_commands)/sizeof(const char *);
const int max_lenght = 6;

static command_t str_to_digit(const char *line)
{
    for(size_t i = 0; i < n_commands; i++)
    {
        printf("str_command = %s\n", *(str_commands + i));
        if(!strcmp(line, *(str_commands + i)))
        {
            return d_commands[i];
        }
    }
    return NOT_COMMAND;
}

int convert_file(const char *file_name, const char *bc_file_name)
{
    FILE *file = fopen(file_name, "r");  // open file to conver it to binary-code file
    if(file == NULL)
    {
        VERROR_FOPEN(file_name);
        return 1;
    }

    FILE *bc_file = fopen(bc_file_name, "w");
    if(bc_file == NULL)
    {
        VERROR_FOPEN(bc_file_name);
        close_file(file, file_name);
        return 1;
    }

    char *data_commands = (char *)calloc(sizeof(char), max_lenght);
    int x = 0;

    while(fscanf(file, "%s", data_commands) != EOF)
    {
        command_t command = str_to_digit((const char *)data_commands);

        if(fprintf(bc_file, "%d", command) < 0)
        {
            VERROR_FILL_FILE(bc_file_name);
            close_file(file, file_name);
            close_file(bc_file, bc_file_name);
            free(data_commands);
            return 1;
        }

        if(command == PUSH || command == IN)
        {
            fscanf(file, "%d", &x);

            if(fprintf(bc_file, " %d", x) < 0)
            {
                VERROR_FILL_FILE(bc_file_name);
                close_file(file, file_name);
                close_file(bc_file, bc_file_name);
                free(data_commands);
                return 1;
            }
        }

        if(fprintf(bc_file, "\n") < 0)
        {
            VERROR_FILL_FILE(bc_file_name);
            close_file(file, file_name);
            close_file(bc_file, bc_file_name);
            free(data_commands);
            return 1;
        }
    }

    close_file(file, file_name);
    close_file(bc_file, bc_file_name);
    free(data_commands);

    return 0;
}

int main()
{
    convert_file("to_calculate.txt", "to_calculate_bc.txt");
    return 0;
}


