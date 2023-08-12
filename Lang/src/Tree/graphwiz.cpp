#include "graphwiz.hpp"
#include "settings.hpp"
#include "../binarytree.hpp"
#include "../frontend.hpp"

char* inttoa(int n, char* s) {
    int i = 0;
    int sign = 0;
 
    if ((sign = n) < 0)
        n = -n;
    i = 0;
    do {
        s[i++] = (char) (n % 10) + '0';
    } while ((n /= 10) > 0);    
    if (sign < 0)
        s[i++] = '-';
    s[i] = '\0';
    reverse(s);
    return s;
}

void reverse(char* s) {
    int i = 0, j = 0;
    char c = ' ';

    for (i = 0, j = (int) strlen(s)-1; i<j; i++, j--) {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}

void graph_dump (struct tree* myTree) {
    static int num = 0;
    char name_of_file[40] = "TreeDump/image";                // char name_of_file[20] = "txt/image";
    char im_num[5] = {};
    strcat (name_of_file, inttoa(num, im_num));
    strcat (name_of_file, ".txt\0");

    char pic_name[40] = "TreeDump/image";
    strcat (pic_name, im_num);
    strcat (pic_name, ".png\0");

    num++;

    FILE* output = fopen (name_of_file,"w+");

    myTree->output = output;
    if  (myTree->output == nullptr) {
        myTree->error = ERROR_GRAPHVIZ_FILE_NOT_OPEN;
        printf ("File didn't open\n");
        return;
    }

    fprintf (myTree->output, "digraph TB\n{\n");
    fprintf (myTree->output, "node [shape=record fontname=Arial];\n");
    fprintf (myTree->output, "edge [dir=both, style=\"\", constraint=false, color = darkred]");
    fprintf (myTree->output, "rankdir = TB;\n");
    fprintf (myTree->output, "splines = \"ortho\"\n");
    
    fprintf (myTree->output, "head [label = \"HEAD\", style = filled, fillcolor = \"#d01234\"];\n");
    allNodeInGraphwiz (myTree, myTree->head);

    fprintf (myTree->output, "}\n");
    fclose (myTree->output);
    myTree->output = nullptr;

    compileDot (name_of_file, pic_name);
}

void allNodeInGraphwiz (struct tree* myTree, struct node* node) {
    static int xxxmanera = -1;
            xxxmanera++;
            printf("node->type = %d\n", xxxmanera);

    switch ((int) node->type_of_value) {
        case ERROR:
            fprintf (myTree->output, "node%p [label = \"{<f1> value = %s|  <f2> addr = %p| {<f3> left = %p |<f4> right = %p}| <f6> ERROR}\", shape=record, style = filled, fillcolor = \"#%x\"];\n",
                    node, "error", node, node->lefty, node->righty, ERROR_COLOR);
            break;
        
        case EMPTY:
            fprintf (myTree->output, "node%p [label = \"{<f1> value = %s|  <f2> addr = %p| {<f3> left = %p |<f4> right = %p}| <f6> EMPTY}\", shape=record, style = filled, fillcolor = \"#%x\"];\n",
                    node, "empty", node, node->lefty, node->righty, EMPTY_COLOR);
            break;
        case OP:
            fprintf (myTree->output, "node%p [label = \"{<f1> value = %d|  <f2> addr = %p| {<f3> left = %p |<f4> right = %p}| <f6> OP(%s)}\", shape=record, style = filled, fillcolor = \"#%x\"];\n",
                    node, node->value.operation, node, node->lefty, node->righty, convertCodeToOp (node->value.operation), OP_COLOR);
            break;

        case VAL:
            fprintf (myTree->output, "node%p [label = \"{<f1> value = %f|  <f2> addr = %p| {<f3> left = %p |<f4> right = %p}| <f6> VAL}\", shape=record, style = filled, fillcolor = \"#%x\"];\n",
                    node, node->value.number, node, node->lefty, node->righty, VAL_COLOR);
        break;

        case VAR:
            fprintf (myTree->output, "node%p [label = \"{<f1> value = %s|  <f2> addr = %p| {<f3> left = %p |<f4> right = %p}| <f6> VAR}\", shape=record, style = filled, fillcolor = \"#%x\"];\n",
                    node, node->value.text, node, node->lefty, node->righty, VAR_COLOR);
            break;

        case DECL_VAR_HEAD:
            fprintf (myTree->output, "node%p [label = \"{<f1> value = %s|  <f2> addr = %p| {<f3> left = %p |<f4> right = %p}| <f6> DECL_VAR_HEAD}\", shape=record, style = filled, fillcolor = \"#%x\"];\n",
                    node, node->value.text, node, node->lefty, node->righty, DECL_VAR_COLOR);
            break;

        case DECL_FUNC_HEAD:
            fprintf (myTree->output, "node%p [label = \"{<f1> value = %s|  <f2> addr = %p| {<f3> left = %p |<f4> right = %p}| <f6> DECL_FUNC_HEAD}\", shape=record, style = filled, fillcolor = \"#%x\"];\n",
                    node, "decl_func_head", node, node->lefty, node->righty, DECL_FUNC_COLOR);
            break;

        case FUNC_INFO:
            fprintf (myTree->output, "node%p [label = \"{<f1> value = %s|  <f2> addr = %p| {<f3> left = %p |<f4> right = %p}| <f6> FUNC_INFO}\", shape=record, style = filled, fillcolor = \"#%x\"];\n",
                    node, "func_info", node, node->lefty, node->righty, FUNC_INFO_COLOR);
            break;

        case FUNC_ARGS:
            fprintf (myTree->output, "node%p [label = \"{<f1> value = %s|  <f2> addr = %p| {<f3> left = %p |<f4> right = %p}| <f6> FUNC_ARGS}\", shape=record, style = filled, fillcolor = \"#%x\"];\n",
                    node, "func_args", node, node->lefty, node->righty, FUNC_ARGS_COLOR);
            break; 

        case FUNC_NAME:
            fprintf (myTree->output, "node%p [label = \"{<f1> value = %s|  <f2> addr = %p| {<f3> left = %p |<f4> right = %p}| <f6> FUNC_NAME}\", shape=record, style = filled, fillcolor = \"#%x\"];\n",
                    node, node->value.text, node, node->lefty, node->righty, FUNC_NAME_COLOR);
            break;

        case FUNC_CALL: 
            fprintf (myTree->output, "node%p [label = \"{<f1> value = %s|  <f2> addr = %p| {<f3> left = %p |<f4> right = %p}| <f6> FUNC_CALL}\", shape=record, style = filled, fillcolor = \"#%x\"];\n",
                    node, "func_call", node, node->lefty, node->righty, FUNC_CALL_COLOR);
            break;

        case EXPR_HEAD:
            fprintf (myTree->output, "node%p [label = \"{<f1> value = %s|  <f2> addr = %p| {<f3> left = %p |<f4> right = %p}| <f6> EXPR_HEAD}\", shape=record, style = filled, fillcolor = \"#%x\"];\n",
                    node, "expr_head", node, node->lefty, node->righty, EXPR_HEAD_COLOR);
            break;

        case LOGIC_OP:
            fprintf (myTree->output, "node%p [label = \"{<f1> value = %s|  <f2> addr = %p| {<f3> left = %p |<f4> right = %p}| <f6> LOGIC_OP}\", shape=record, style = filled, fillcolor = \"#%x\"];\n",
                    node, "logic_op", node, node->lefty, node->righty, LOGIC_OP_COLOR);
            break; 

        case STATM_OP:
            fprintf (myTree->output, "node%p [label = \"{<f1> value = %s|  <f2> addr = %p| {<f3> left = %p |<f4> right = %p}| <f6> STATM_OP}\", shape=record, style = filled, fillcolor = \"#%x\"];\n",
                    node, "statm_op", node, node->lefty, node->righty, STATM_OP_COLOR);
            break; 

        case MAIN:
            fprintf (myTree->output, "node%p [label = \"{<f1> value = %s|  <f2> addr = %p| {<f3> left = %p |<f4> right = %p}| <f6> MAIN}\", shape=record, style = filled, fillcolor = \"#%x\"];\n",
                    node, "main", node, node->lefty, node->righty, MAIN_COLOR);
            break;

        case RETURN:
            fprintf (myTree->output, "node%p [label = \"{<f1> value = %s|  <f2> addr = %p| {<f3> left = %p |<f4> right = %p}| <f6> RETURN}\", shape=record, style = filled, fillcolor = \"#%x\"];\n",
                    node, "return", node, node->lefty, node->righty, RETURN_COLOR);
            break;    
        case BREAK:
            fprintf (myTree->output, "node%p [label = \"{<f1> value = %s|  <f2> addr = %p| {<f3> left = %p |<f4> right = %p}| <f6> BREAK}\", shape=record, style = filled, fillcolor = \"#%x\"];\n",
                    node, "break", node, node->lefty, node->righty, BREAK_COLOR);
            break;

        case ASSIGN:
            fprintf (myTree->output, "node%p [label = \"{<f1> value = %s|  <f2> addr = %p| {<f3> left = %p |<f4> right = %p}| <f6> ASSIGN}\", shape=record, style = filled, fillcolor = \"#%x\"];\n",
                    node, "=", node, node->lefty, node->righty, ASSIGN_COLOR);
            break;

        default:
            DBG("ERROR::Unnamed type in tree\n");
            break;
    };
    
    if (node->lefty != nullptr) {
        DBG("left ok")
        allNodeInGraphwiz (myTree, node->lefty);
    }
    if (node->righty != nullptr) {
        DBG("right ok")
        allNodeInGraphwiz (myTree, node->righty);
    }

    if (node->lefty != nullptr)
        fprintf (myTree->output, "node%p -> node%p[dir=both, style=filled, constraint=true];\n", node, node->lefty);
    
    if (node->righty != nullptr)
        fprintf (myTree->output, "node%p -> node%p[dir=both, style=filled, constraint=true];\n", node, node->righty);
}

void compileDot (char* name_of_file, char* pic_name) {
    char command[50] = "dot ";

    strcat (command, name_of_file);
    strcat (command, " -Tpng -o ");
    // strcat (command,"img/");
    strcat (command, pic_name);

    system(command);
}

char* convertCodeToOp (int codeOp) {
    switch (codeOp) {
        case T_DOLLAR:
            return "$";
            break;
        case T_ASSIGN:
            return "=";
            break;
        case T_PLUS:
            return "+";
            break;
        case T_MINUS:
            return "-";
            break;
        case T_MULTIPLY:
            return "*";
        case T_DIVISION:
            return "/";
            break;
        case T_IF:
            return "if";
            break;
        case T_ELSE:
            return "else";
            break;
        case T_WHILE:
            return "while";
            break;
        case T_FOR:
            return "for";
            break;
        case T_EQUAL:
            return "==";
            break;
        case T_NOT_EQUAL:
            return "!=";
            break;
        case T_RETURN:
            return "return";
            break;
        case T_VAR:
            return "var";
            break;
        case T_MAIN:
            return "main";
            break;
        case T_BREAK:
            return "break";
            break;
        case T_FUNCT:
            return "funct";
            break;
        case T_GREATER:
            return "\\>";
            break;
        case T_LESS:
            return "\\<";
            break;
        case T_GREATER_OR_EQ:
            return "\\>=";
            break;
        case T_LESS_OR_EQ:
            return "\\<=";
            break;
        case T_IN:
            return "in";
            break;
        case T_OUT:
            return "out";
            break;
        case T_SQRT:
            return "sqrt";
            break;
        default:
            return "error";
            break;
    }
}