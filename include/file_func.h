#ifndef FILE_FUNC_H
#define FILE_FUNC_H

#include <stdio.h>

char **make_ptr_array(char *data, size_t *str_count);

char *get_data_from_file(const char *filename, size_t *data_size);

int write_file(char *buf, const char *file_name, size_t n_com, size_t file_size);

#endif
