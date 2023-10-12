#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <stdio.h>
// #include "commands.h"

int asm_for_single_line(const char *line, struct codes *command);

struct codes *assembler(char **lines, size_t n_lines);

#endif
