#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdio.h>
#include "stack_func.h"

typedef enum // enum for second parametr in push, pop, in, out functions
{
    STACK,
    REGISTER_A,
    REGISTER_B,
    REGISTER_C,
    REGISTER_D,
}most_bit;

typedef enum
{
    NOT_COMMAND = 0,
    PUSH = 1,
    POP,
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

struct codes
{
    unsigned char most : 4;
    unsigned char least : 4;
    unsigned char have_arg : 1;
    elem_t arg;
};

// most_bit d_most_bit[] = {STACK, REGISTER_A, REGISTER_B, REGISTER_C, REGISTER_D};
// const char *str_registers[] = {"rax", "rbx", "rcx", "rdx"};
// size_t n_registers = sizeof(str_registers)/sizeof(const char *);

// command_t d_commands[] = {PUSH, POP, ADD, SUB, MUL, DIV, SQRT, COS, SIN, IN, OUT, HLT};
// const char *str_commands[] = {"push", "pop", "add", "sub", "mul", "div", "sqrt", "cos", "sin", "in", "out", "hlt"};
// size_t n_commands = sizeof(str_commands)/sizeof(const char *);
// const int max_length = 10;

#endif
