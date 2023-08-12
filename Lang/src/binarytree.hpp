#ifndef BINARYTREE_HPP
#define BINARYTREE_HPP

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "Tree/graphwiz.hpp"
#include "Tree/settings.hpp"

#define DBG(message) printf("%s in FILE:%s FUNC:%s LINE:%d\n", message, __FILE__, __PRETTY_FUNCTION__, __LINE__);

#define POISON 0xBAD

typedef float _t;         
#define TYPE_DESIG "%f"
#define TYPE_DESIG_FOR_PRINT "%5f"
#define MAX_FILE_NAME 40

#define MAX_NODES 10

union value {
    _t    number;
    char* text;
    int   operation;
};


enum node_type_tree
{
    ERROR          = -1,
    EMPTY          = 0,     // IT'S OPERATION TYPE
    //OP_HEAD        = 1,
    OP             = 2,     //op_number

    //VAL_HEAD       = 3,
    VAL            = 4,     //value

    //VAR_HEAD       = 5,
    VAR            = 6,     //variable

    DECL_VAR_HEAD  = 7,     // 

    DECL_FUNC_HEAD = 8,
    //FUNC_HEAD      = 17,
    FUNC_INFO      = 9,
    FUNC_ARGS      = 10,
    FUNC_NAME      = 11,
    FUNC_CALL      = 12,

    EXPR_HEAD      = 13,

    //LOGIC_OP_HEAD  = 14,
    LOGIC_OP       = 15,    // >, <, ==, etch

    //STATM_OP_HEAD  = 21,
    STATM_OP       = 22,    //if, else, while, for


    MAIN           = 16,    //main () {}
    RETURN         = 18,    //return 
    BREAK          = 19,    //break

    ASSIGN         = 20, 
    IF             = 21,
    SQRT           = 22
};

enum TypeOfValue {
    TREE_FREE      = -1,
    TREE_NUMBER    = 0,
    TREE_OPERATION = 1,
    TREE_VARIABLE  = 2
};

enum Errors {
    NO_ERRORS                   = 0,
    ERROR_NULL_TREE_PTR         = 1,
    ERROR_NULL_NODE_PTR         = 2,
    ERROR_NODE_HAS_TWO_CHILDREN = 3
};

struct node {
    union value value;
    size_t type_of_value = TREE_FREE;

    node* lefty  = nullptr;
    node* righty = nullptr;
};

struct tree {   
    struct node* head = nullptr;
    size_t max_size = 0;
    size_t cur_size = 0;
    size_t error  = NO_ERRORS;
    FILE* output = nullptr;
};


int treeCtor (struct tree* myTree);

void treeDtor (struct tree* myTree);

void treeDeleteSubTree (struct node* node);

struct node* treeAddNode (struct tree* myTree, _t node_value, size_t node_type = TREE_FREE, char* text = nullptr, struct node* left_child = nullptr, struct node* right_child = nullptr);

void treePrint (struct node* node, FILE* output);

// void printTree (struct tree* myTree, char* fileName);

// void reccursiveTreePrint (struct tree* myTree);

#endif