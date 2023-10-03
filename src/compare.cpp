#include <string.h>
#include <stdio.h>
#include "commands.h"
#include "verror.h"

command_t str_to_digit(const char *line)
{
    for(size_t i = 0; i < n_commands; i++)
    {
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
        VERROR("file", file_name, "cannot be opened");
        return 1;
    }

    FILE *bc_file = fopen(bc_file_name, "w");
    if(bc_file == NULL)
    {
        VERROR("file", bc_file_name, "cannot be opened");
        fclose(file);
        return 1;
    }

    char *data_commands = 0;
    int x = 0;

    while(fscanf(file, "%s", data_commands) != EOF)
    {
        command_t command = str_to_digit((const char *)data_commands);

        if(fprintf(bc_file, "%d", command) < 0)
        {
            VERROR("failed to fill the", bc_file_name, "file");
            fclose(file);
            fclose(bc_file);
            return 1;
        }

        if(command == PUSH || command == IN)
        {
            fscanf(file, "%d", &x);

            if(fprintf(bc_file, " %d", x) < 0)
            {
                VERROR("faild ot fill the", bc_file_name, "file");
                fclose(file);
                fclose(bc_file);
                return 1;
            }
        }

        if(fprintf(bc_file, "\n") < 0)
        {
            VERROR("faild to fill the", bc_file_name, "file");
            fclose(file);
            fclose(bc_file);
            return 1;
        }
    }

    fclose(file);
    fclose(bc_file);

    return 0;
}
