#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAXNUMBEROFFILENAME 160
#define NUMBEROFLABELS 10

#define DBG printf("FILE:%s FUNC:%s LINE:%d\n", __FILE__, __FUNCTION__, __LINE__);

#define DEBUG

#if defined (DEBUG)

#define NEWASSERT(condition)                                                            \
    if (condition) {                                                                 \
        printf("Problem with in file %s in function %s, condition %s in line %d\n",     \
               __FILE__, __FUNCTION__, #condition, __LINE__);                           \
        exit(1);                                                                        \
    }
#else
    #define NEWASSERT(condition) ;
#endif

struct inputOutputFiles 
{
    FILE* input;
    FILE* output;
    char* commands;
    int length_input;
    int* allProgramm;
    char* ProgrammCoded;
    int labels[NUMBEROFLABELS];
};

enum commands
{
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
    STACKSQRT   = 23
};

void CoderInit (struct inputOutputFiles* p_files);

int lenFile(FILE *text);

char* inttoa(int n, char* s);

void reverse(char* s);

void PreCoding (struct inputOutputFiles* p_files);

void Coding (struct inputOutputFiles* p_files);

void OutputResults (struct inputOutputFiles* p_files);

void Destructor (struct inputOutputFiles* p_files);
