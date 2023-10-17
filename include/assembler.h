#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <stdio.h>

#define REG_MASK 0x3
#define OP_MASK 0x1f

int asm_for_single_line(const char *line, struct codes *command);

struct codes *assembler(char **lines, size_t n_lines);

int pop_has_arg(struct codes *code, const char *line);

int push_has_arg(struct codes *code, const char *line);

int is_register(struct codes *code, char *str_register);

#endif