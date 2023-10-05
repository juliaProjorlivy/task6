#include "verror.h"

void verror(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
}

void close_file(FILE *file, const char *file_name)
{
    if(fclose(file) == EOF)
    {
        VERROR_FCLOSE(file_name);
    }
}
