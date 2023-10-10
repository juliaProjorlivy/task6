#include "commands.h"
#include "disassembler.h"
#include "verror.h"
#include <string.h>
#include <stdlib.h>

static int digit_to_str(command_t command, const char **data_command)
{
    for(size_t i = 0; i < n_commands; i++) // TODO: return NULL vs NON-NULL instead of 0 vs 1
    {
        if(command == d_commands[i])
        {
            // printf("str_commads = %s\n", str_commands[i]);
            *data_command = str_commands[i];
            // printf("str_data_commads = %s\n", *data_command);
            return 0;
        }
    }

    VERROR("no such command as %d", command);
    return 1;
}

int convert_binary_file(const char *bc_file_name, const char *file_name)
{
    FILE *bc_file = fopen(bc_file_name, "rt");  // open file to conver it to binary-code file
    if(bc_file == NULL)
    {
        VERROR_FOPEN(bc_file_name);
        return 1;
    }

    FILE *file = fopen(file_name, "w");
    if(file == NULL)
    {
        VERROR_FOPEN(file_name);
        fclose(file);
        return 1;
    }

    command_t command = NOT_COMMAND;
    char **data_command = (char **)calloc(sizeof(char*), 1); // TODO: static!!!
    
    if(data_command == NULL)
    {
        VERROR("memory allocation problem %s", strerror(errno));
        close_file(file, file_name);
        close_file(bc_file, bc_file_name);
        return 1;
    }

    while(fscanf(bc_file, "%d", &command) != EOF)
    {
        digit_to_str(command, (const char **)data_command); // TODO: check return value
        // printf("data_command = %s\n", *data_command);

        if(fprintf(file, "%s", *data_command) < 0)
        {
            VERROR_FILL_FILE(file_name);
            close_file(file, file_name);
            close_file(bc_file, bc_file_name);
            free(data_command);
            return 1;
        }

        if(command == PUSH || command == IN) // TODO: remove IN
        { // IN does not have an argument
            int arg = 0;

            fscanf(bc_file, "%d", &arg);

            if(fprintf(file, " %d", arg) < 0)
            {
                VERROR_FILL_FILE(file_name);
                close_file(file, file_name);
                close_file(bc_file, bc_file_name);
                free(data_command);
                return 1;
            }
        }

        if(fprintf(file, "\n") < 0)
        {
            VERROR_FILL_FILE(file_name);
            close_file(file, file_name);
            close_file(bc_file, bc_file_name);
            free(data_command);
            return 1;
        }
    }

    close_file(file, file_name);
    close_file(bc_file, bc_file_name);
    free(data_command);

    return 0;
}

int main()
{
    convert_binary_file("to_calculate_bc.txt", "to_calculate_damb.txt");
    return 0;
}




