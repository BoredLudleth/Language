#ifndef STACK_HPP
#define STACK_HPP

#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <math.h>

#include "stackoperations.hpp"

#define TYPE_SPECIFIER "%f"
#define CANARY_VALUE 58

#define DEFAULT_STACK_LENGTH 10
#define POISON 0xBAD

#define MAXFILENAME 100

#define DBG printf("FILE:%s FUNC:%s LINE:%d\n", __FILE__, __FUNCTION__, __LINE__);

#define ASSERT(stack) if (stackCheck(stack)) { \
                        exit(0);               \
                      }

struct registers 
{
    type ax = 0;

    type bx = 0;

    type cx = 0;

    type dx = 0;
};

struct stack 
{
    float* allProgramm;
    int    sizeOfProgramm;
    FILE*  inputFile;

    type*  data;                     // значение
    int    filled;                       // указывает на первый свободный
    int    lengthStack;
    int    error;
    int    canary_1;
    int    canary_2;

    int cur = 0;

    struct registers regs;
};


enum commands {
    STACKERROR = 0,
    STACKPUSH  = 1,
    STACKPOP   = 2,
    STACKADD   = 3,
    STACKSUB   = 4,
    STACKMUL   = 5,
    STACKDIV   = 6,
    STACKOUT   = 7,
    STACKPRINT = 8,
    STACKDUMP  = 9,
    STACKHLT   = 10, 
    STACKJUMP  = 11,
    STACKJB    = 12,
    STACKJBE   = 13,
    STACKJA    = 14,
    STACKJAE   = 15,
    STACKJE    = 16,
    STACKJNE   = 17,
    STACKRPUSH = 18,
    STACKRPOP  = 19, 
    STACKCALL  = 20,
    STACKRET   = 21,
    STACKIN    = 22,
    STACKSQRT  = 23
};





struct cpu
{
    struct stack stack;
    struct stack functstack;
};

enum Errors
{
    NO_ERRORS               = 0,
    ERR_STACK_IS_LITTLE     = 1,
    ERR_NULL_DATA           = 2,
    ERR_CANERY_CHANGED      = 4,
    ERR_SIZE_OUT_LEFTRANGE  = 8,
    ERR_DIVIDE_ON_ZERO      = 16
};

void cpuInit (struct cpu* mycpu);

void stackCtor (struct stack* stack);

void stackDtor (struct stack* stack);

void stackDump (struct stack* stack);

void stackPush (struct stack* stack, type value);

void stackRead (struct cpu* mycpu, struct stack* stack);

type stackPop (struct stack* p_stack);

int stackCheck (struct stack* stack);

void stackResizeUp (struct stack* stack);

void stackResizeDown (struct stack* stack);

int lenFile(FILE *text);

#endif
