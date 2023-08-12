#ifndef GRAPHWIZ_HPP
#define GRAPHWIZ_HPP

#include "../binarytree.hpp"
#include "settings.hpp"

enum graphwiz_errors {
    ERROR_GRAPHVIZ_FILE_NOT_OPEN = 1,
};

char* inttoa(int n, char* s);

void reverse(char* s);

void graph_dump (struct tree* myTree);

void compileDot (char* name_of_file, char* pic_name);

void allNodeInGraphwiz (struct tree* myTree, struct node* node);

char* convertCodeToOp (int codeOp);

#endif
