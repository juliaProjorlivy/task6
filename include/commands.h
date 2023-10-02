#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdio.h>

enum commands
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
};

enum commands d_commands[] = { PUSH, ADD, SUB, MUL, DIV, SQRT, COS, SIN, IN, OUT, HLT};

const char *str_commands[] = {"push", "add", "sub", "mul", "div", "sqrt", "cos", "sin", "in", "out", "hlt"};

size_t n_commands = sizeof(str_commands)/sizeof(const char *);

#endif
