#include "file_func_dasm.h"
#include "commands.h"
#include "verror.h"
#include <stdlib.h>

int write_file_dasm(const char *file_name, char **lines, size_t n_lines)
{
    FILE *file = fopen(file_name, "wt");
    if(file == NULL)
    {
        VERROR_FOPEN(file_name);
        return 1;
    }
    for(size_t i_line = 0; i_line < n_lines; i_line++)
    {
        if(fprintf(file, "%s", lines[i_line]) <= 0)
        {
            VERROR_FWRITE(file_name);
            return 1;
        }
    }

    return close_file(file, file_name);
}

char *get_ptrs_from_file(const char *file_name, size_t *buf_size, size_t *n_com) // the same as in file_func_runner.cpp maybe make only onle func_file?
{
    FILE *file = fopen(file_name, "rb");
    if(file == NULL)
    {
        VERROR_FOPEN(file_name);
        return NULL;
    }

    if(fread(buf_size, sizeof(size_t), 1, file) <= 0)
    {
        VERROR_FWRITE(file_name);
        close_file(file, file_name);
        return NULL;
    }

    if(fread(n_com, sizeof(size_t), 1, file) <= 0)
    {
        VERROR_FWRITE(file_name);
        close_file(file, file_name);
        return NULL;
    }

    char *buf = (char *)calloc(sizeof(char), *buf_size);
    if(buf == NULL)
    {
        VERROR("memory allocation failire");
        close_file(file, file_name);
        return NULL;
    }

    if(fread(buf, sizeof(char), *buf_size, file) <= 0)
    {
        VERROR_FWRITE(file_name);
        close_file(file, file_name);
        return NULL;
    }

    close_file(file, file_name);

    return buf;
}


