#ifndef FILE_FUNC_DASM_H
#define FILE_FUNC_DASM_H

#include <stdio.h>

int write_file_dasm(const char *flie_name, char **lines, size_t n_lines);

struct codes *get_ptrs_from_file(const char *file_name, size_t *n_com);

#endif
