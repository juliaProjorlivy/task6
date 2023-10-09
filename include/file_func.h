#ifndef FILE_FUNC_H
#define FILE_FUNC_H

#include <stdio.h>
// #include <commands.h>

char *get_data_from_file(const char *filename, size_t *data_size);

struct codes *write_file(const char *file_name_src, const char *file_name_dest);

#endif