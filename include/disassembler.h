#ifndef DISASSEMBLER_H
#define DISASSEMBLER_H

const char *is_register(struct codes *code);

const char *is_command(struct codes *code);

char *dasm_for_single_code(char *buf, size_t *i_buf);

char **disasm(char *buf, size_t buf_size, size_t n_codes);

#endif
