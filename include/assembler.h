#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <stdio.h>

#define REG_MASK 0x7
#define OP_MASK 0x1f

static const size_t N_LABELS = 10;
static const ssize_t label_length = 20;

struct label
{
    char *name;
    ssize_t ip;
};

int fill_register(struct codes *code, char *str_register);

int fill_command(struct codes *code, char *str_command);

int label_arg(elem_t *arg, struct label *labels, const char *str_command, size_t n_filled_labels);

int push_has_arg(elem_t *arg, struct codes *code, const char *line);

int pop_has_arg(elem_t *arg, struct codes *code, const char *line);

int jump_has_arg(elem_t *arg, struct codes *code, const char *line, struct label *labels, size_t *n_filled_labels);

int fill_empty_labels(struct label *labels, size_t i_label, size_t n_labels);

struct label *fill_labels(char **lines, size_t n_lines, size_t *n_labels, size_t *n_filled_labels);

int asm_for_single_line(char *buf, size_t *i_buf, elem_t *arg, const char *line, struct codes *code, struct label *labels, size_t n_filled_labels);

int assembler(char *buf, size_t *i_buf, char **lines, size_t n_lines);

void free_labels(struct label *labels, size_t n_in_labels);

#endif
