#include "commands.h"
#include "file_func_runner.h"
#include "runner.h"
#include "verror.h"
#include <stdlib.h>

char *get_ptrs_from_file(const char *file_name, size_t *buf_size)
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
        return NULL;
    }

    char *buf = (char *)calloc(sizeof(char), *buf_size);
    if(buf == NULL)
    {
        VERROR("memory allocation failire");
        return NULL;
    }

    if(fread(buf, sizeof(char), *buf_size, file) <= 0)
    {
        VERROR_FWRITE(file_name);
        return NULL;
    }

    close_file(file, file_name);

    return buf;
}
