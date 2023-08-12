#ifndef FRONTEND_HPP
#define FRONTEND_HPP

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "tree_lib.hpp"


#define POISON 0xBAD
#define DEFAULT_TOKENS 1000
#define DEFAULT_LENGTH_OF_FILENAME 144
#define TOKEN_RESIZE_COEFFICIENT 2

#define OUT_COMMENT 0
#define IN_COMMENT 1

typedef float _t;

enum tokenType {
    NUMBER      = 0,
    OPERATION   = 1,
    VARIABLE    = 2,
    FUNCTION    = 3,
    OPEN_SCOPE  = 4,
    CLOSE_SCOPE = 5,
    TEXT        = 6
};

enum operatorType {
    T_DOLLAR    = 0,         //$
    T_ASSIGN        = 1,         //=
    T_PLUS          = 2,         //+
    T_MINUS         = 3,         //-
    T_MULTIPLY      = 4,         //*
    T_DIVISION      = 5,         ///
    T_IF            = 6,         //IF
    T_ELSE          = 7,         //ELSE
    T_WHILE         = 8,         //WHILE
    T_FOR           = 9,         //FOR
    T_EQUAL         = 10,        //==
    T_NOT_EQUAL     = 11,        //!=
    T_RETURN        = 12,        //RETURN
    T_VAR           = 13,        //VAR
    T_MAIN          = 14,        //MAIN
    T_BREAK         = 15,        //BREAK
    T_FUNCT         = 16,        //FUNCTION
    T_GREATER       = 17,
    T_LESS          = 18,
    T_GREATER_OR_EQ = 19,
    T_LESS_OR_EQ    = 20,
    T_IN            = 21,
    T_OUT           = 22,
    T_SQRT          = 23
};

// union value {
//     _t    number;
//     char* text;
//     int   operation;
// };

struct token {
    size_t t_type     = POISON;
    union value value = {.number = POISON};
};

struct prog_data {
    FILE* program       = nullptr;
    char* text          = nullptr;

    struct token* token = nullptr;
    int current_token   = 0;
    int token_size      = 0;

    struct tree* tree   = nullptr;
};

void readFile (struct prog_data* data, char* progFileName);

void tokenInit (struct prog_data* data);

void tokenResize (struct prog_data* data);

void tokenezation (struct prog_data* data);

int valueTypeTok (struct token* token, char* word);

int lenFile (FILE* text);

struct tree* readG (struct prog_data* data);

struct node* readFunct (struct prog_data* data);

struct node* getFuncName (struct prog_data* data);

struct node* getArgs (struct prog_data* data);

struct node* readMain (struct prog_data* data);

struct node* readCommands (struct prog_data* data);

struct node* readFunctCall (struct prog_data* data);

struct node* readReturn (struct prog_data* data);

struct node* readAnnounceVar (struct prog_data* data);

struct node* readWhile (struct prog_data* data);

struct node* readFor (struct prog_data* data);

struct node* readIf (struct prog_data* data);

struct node* readElse (struct prog_data* data);

struct node* readLogE (struct prog_data* data);

struct node* readCell (struct prog_data* data);

struct node* readNum (struct prog_data* data);

struct node* readVar (struct prog_data* data);

struct node* readE (struct prog_data* data);

struct node* readRightValue (struct prog_data* data);

struct node* getE (struct prog_data* data);

struct node* getT (struct prog_data* data);

struct node* getSq (struct prog_data* data);

struct node* getP (struct prog_data* data);

void free_mem (struct prog_data* data);

void readDollar (struct prog_data* data);

#endif
