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

#endif
