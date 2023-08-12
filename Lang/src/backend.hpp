#ifndef BACKEND_HPP
#define BACKEND_HPP

#include "binarytree.hpp"
#include "frontend.hpp"

#define MAX_LABEL_LENGTH 15
#define MAX_COUNT_LENGTH 15

#define TRANSLATE_OK 0
#define TRANSLATE_ERROR_ASM_FILE_NOT_OPENED 1
#define TRANSLATE_SYNTAX_ERROR 2
#define TRANSLATE_ERROR_STRANGE_OP_CODE 3
#define TRANSLATE_ERROR_RIGHT_VALUE 4
#define TRANSLATE_ERROR_CONDITION 5

#define CHECK_ERRORS(x) if (x != TRANSLATE_OK) {    \
                            return x;               \
                        }

int translateTreeToAsm (struct node* myTree);

int translateProgramToAsm (struct node* myTree, FILE* asm_file);

int translateProgBodyToAsm (struct node*myTree, FILE* asm_file);

int translateDeclFuncToAsm (struct node* myTree, FILE* asm_file);

int translateVarsDeclToAsm (struct node* myTree, FILE* asm_file);

int translateDeclFuncVarsToAsm (struct node* myTree, FILE* asm_file);

int translateMainToAsm (struct node* myTree, FILE* asm_file);

int translateCommandsToAsm (struct node* myTree, FILE* asm_file);

int translateOpToAsm (struct node* myTree, FILE* asm_file);

int translateRightValueToAsm (struct node* myTree, FILE* asm_file);

int translateOperationToAsm (struct node* myTree, FILE* asm_file);

int translateFuncCallToAsm (struct node* myTree, FILE* asm_file);

int translatePopCallArgsToAsm (struct node* myTree, FILE* asm_file);

int translateCallArgsToAsm (struct node* myTree, FILE* asm_file);

int translateWhileToAsm (struct node* myTree, FILE* asm_file);

int translateIfToAsm (struct node* myTree, FILE* asm_file);

int translateConditionToAsm (struct node* myTree, char* label, FILE* asm_file);

int translateAnnounceVarToAsm (struct node* myTree, FILE* asm_file);

int translateReturnToAsm (struct node* myTree, FILE* asm_file);

void encryptError (int error_code);

#endif
