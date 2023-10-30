#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "stack_func.h"

char *to_lower(const char *line, char *new_line);

static const int max_length = 30;

#define DEF_REG(NAME, reg_code) NAME = reg_code,
typedef enum
{
    #include "def_reg.txt"
} reg_t;
#undef DEF_REG

#define DEF_CMD(NAME, command_code, code) NAME = command_code,
typedef enum
{
    #include "def_cmd.txt"
} command_t;
#undef DEF_CMD

struct codes
{
    unsigned char to_ram : 1;
    unsigned char reg    : 3;
    unsigned char op     : 5; // operation
};

struct str_digit
{
    int d;
    const char str[max_length];
};


#define DEF_CMD(NAME, command_code, code) {(NAME), #NAME},
static struct str_digit commands[] = 
{
#include "def_cmd.txt" 
{0, ""}
};
#undef DEF_CMD

#define DEF_REG(NAME, reg_code) {reg_code, #NAME}, 
static struct str_digit registers[] = 
{
#include "def_reg.txt" 
{0, ""}
};
#undef DEF_REG

static const size_t n_commands = sizeof(commands)/sizeof(commands[0]) - 1;
static const size_t n_registers = sizeof(registers)/sizeof(registers[0]) - 1;

#endif
