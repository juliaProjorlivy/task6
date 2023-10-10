#ifndef FILE_FUNC_H
#define FILE_FUNC_H

#include <stdio.h>
// #include <commands.h>

char **make_ptr_array(char *data, size_t *str_count);

char *get_data_from_file(const char *filename, size_t *data_size);

int write_file(const char *file_name, struct codes *all_codes, size_t n_com);

#endif
