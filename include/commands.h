#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdio.h>

typedef enum
{
    NOT_COMMAND = 0,
    PUSH = 1,
    ADD,
    SUB,
    MUL,
    DIV,
    SQRT,
    COS,
    SIN,
    IN,
    OUT,
    HLT,
}command_t;

command_t d_commands[] = { PUSH, ADD, SUB, MUL, DIV, SQRT, COS, SIN, IN, OUT, HLT};

const char *str_commands[] = {"push", "add", "sub", "mul", "div", "sqrt", "cos", "sin", "in", "out", "hlt"};

size_t n_commands = sizeof(str_commands)/sizeof(const char *);

#endif
