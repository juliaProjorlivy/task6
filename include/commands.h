#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdio.h>
#include "stack_func.h"

typedef enum
{
    STACK = 0,
    REGISTER_A = 1,
    REGISTER_B = 2,
    REGISTER_C = 3,
    REGISTER_D = 4,
} has_reg; // TODO: rename

typedef enum
{
    NOT_COMMAND = 0,
    PUSH = 1,
    POP = 2, // TODO: enumerate =
    ADD = 3,
    SUB = 4,
    MUL = 5,
    DIV = 6,
    SQRT = 7,
    COS = 8,
    SIN = 9,
    IN = 10,
    OUT = 11,
    HLT = 12,
    JMP = 13,
    JA = 14,
    JAE = 15,
    JB = 16,
    JBE = 17,
    JNE = 18,
    JE = 19,
} command_t;

struct codes
{
    elem_t arg;
    unsigned char reg : 2;
    unsigned char op : 5; // operation
    unsigned char has_arg : 1;
};

static has_reg d_registers[] = {REGISTER_A, REGISTER_B, REGISTER_C, REGISTER_D};
static const char *str_registers[] = {"rax", "rbx", "rcx", "rdx"};
static const size_t n_registers = sizeof(str_registers)/sizeof(str_registers[0]);

static command_t d_commands[] = {PUSH, POP, ADD, SUB, MUL, DIV, SQRT, COS, SIN, IN, OUT, HLT, JMP, JA, JAE, JB, JBE, JNE, JE};
static const char *str_commands[] = {"push", "pop", "add", "sub", "mul", "div",
                                    "sqrt", "cos", "sin", "in", "out", "hlt",
                                    "jmp", "ja", "jae", "jb", "jbe", "jne", "je"};
static size_t n_commands = sizeof(str_commands)/sizeof(str_commands[0]);
static const int max_length = 50;

#endif
