#ifndef DISASSEMBLER_H
#define DISASSEMBLER_H

const char *is_register(struct codes *code);

const char *is_command(struct codes *code);

char *dasm_for_single_code(struct codes *code);

char **disasm(struct codes *all_codes, size_t n_codes);

#endif
