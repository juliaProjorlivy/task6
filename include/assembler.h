#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <stdio.h>
// #include "commands.h"
#define REG_MASK 0x3

#define OP_MASK 0x1f

int asm_for_single_line(const char *line, struct codes *command);

struct codes *assembler(char **lines, size_t n_lines);

#endif