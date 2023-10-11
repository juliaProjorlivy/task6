#include "commands.h"
#include "file_func_runner.h"
#include "runner.h"
#include "verror.h"
#include <stdlib.h>

struct codes *get_ptrs_from_file(const char *file_name, size_t *n_com)
{
    FILE *file = fopen(file_name, "rb");
    if(file == NULL)
    {
        VERROR_FOPEN(file_name);
        return NULL;
    }

    if(fread(n_com, sizeof(size_t), 1, file) <= 0)
    {
        VERROR_FWRITE(file_name);
        return NULL;
    }

    struct codes *all_codes = (struct codes *)calloc(sizeof(codes), *n_com);
    if(all_codes == NULL)
    {
        VERROR("memory allocation failire");
        return NULL;
    }

    if(fread(all_codes, sizeof(codes), *n_com, file) <= 0)
    {
        VERROR_FWRITE(file_name);
        return NULL;
    }

    close_file(file, file_name);

    return all_codes;
}
