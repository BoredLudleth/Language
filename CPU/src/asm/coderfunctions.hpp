#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAXNUMBEROFFILENAME 160

#define MAXTOKENS 1000

#define NUMBEROFLABELS 10

#define NUMBEROFTEXTLABELS 10

#define LENGTHOFTEXTLABEL 160

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

typedef float type;

struct registers 
{
    const int ax_value = 1;
    const char* name_ax = "ax";
    type ax = 0;

    const int bx_value = 2;
    const char* name_bx = "bx";
    type bx = 0;

    const int cx_value = 3;
    const char* name_cx = "cx";
    type cx = 0;

    const int dx_value = 4;
    const char* name_dx = "dx";
    type dx = 0;
};

struct textregs
{
    char reg_name[LENGTHOFTEXTLABEL] = "\0";
    int reg_value = -1;
};

struct inputOutputFiles 
{
    FILE* input;
    FILE* binfile;
    FILE* output;

    char* commands;
    char* allProgramm;
    char* ProgrammCoded;

    int    length_input;
    float* commandsValue;
    int    countlines;
    int    nbytes;

    int currentLine = 1;
    int numberOfErrors = 0;
    int lineOfError = 0;

    int labels[NUMBEROFLABELS];
    struct registers reg;
    struct textregs* t_reg;
};

struct token
    {
        char* str;
        type val;
        int codedCommand;
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

char* inttoa(int n, char* s);

void reverse(char* s);

void CoderInit (struct inputOutputFiles* p_files);

int lenFile(FILE *text);

void countLines (struct inputOutputFiles* p_files);

void PreCoding (struct inputOutputFiles* p_files);

void Coding (struct inputOutputFiles* p_files);

int checkNumber (char* st);

void OutputResults (struct inputOutputFiles* p_files);

void skipSpaces (struct inputOutputFiles* p_files);

void Destructor (struct inputOutputFiles* p_files);
