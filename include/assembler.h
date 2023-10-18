#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <stdio.h>

#define REG_MASK 0x3
#define OP_MASK 0x1f

struct lable
{
    char *name;
    ssize_t ip;
};
static const size_t n_lables = 10;

int asm_for_single_line(const char *line, struct codes *code, struct lable **lables, size_t *n_in_lables, size_t *i_code);

struct codes *assembler(char **lines, size_t *n_lines);

int pop_has_arg(struct codes *code, const char *line);

int push_has_arg(struct codes *code, const char *line);

int jump_has_arg(struct codes *code, const char *line, struct lable **lables, size_t *i_in_lables);

int is_register(struct codes *code, char *str_register);

int is_command(struct codes *code, char *str_command);

int is_lable(struct codes *code, struct lable **lables, const char *str_command, size_t *i_in_lables);

int fill_lables(struct lable **lables, char **lines, size_t n_lines, size_t *i_in_lables);

#endif