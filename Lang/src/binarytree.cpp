#include "binarytree.hpp"

int treeCtor (struct tree* myTree) {
    if (myTree == nullptr) {
        myTree->error = ERROR_NULL_TREE_PTR;
        DBG("ERROR:TREE_HAS_NULLPTR");

        return ERROR_NULL_TREE_PTR;
    }


    myTree->head = nullptr;
    myTree->max_size = 0;
    myTree->cur_size = 0;
    myTree->error    = NO_ERRORS;

    return NO_ERRORS;
}

void treeDtor (struct tree* myTree) {
    treeDeleteSubTree (myTree->head);
    
    myTree->head = nullptr;
    myTree->max_size = POISON;
    myTree->cur_size = POISON;
    myTree->error    = POISON; 
}

void treeDeleteSubTree (struct node* node) {
    if (node->lefty != nullptr) {
        treeDeleteSubTree (node->lefty);
    }

    if (node->righty != nullptr) {
        treeDeleteSubTree (node->righty);
    }

    node->value = {.operation = POISON};
    node->type_of_value = TREE_FREE;

    free (node);
    node = nullptr;
}
struct node* treeAddNode (struct tree* myTree, _t node_value, size_t node_type, char* text, struct node* left_child, struct node* right_child) {
    if((node_type == VAL || node_type == VAR || node_type == EMPTY || node_type == ERROR) && (left_child != nullptr || right_child != nullptr)) {
        myTree->error = ERROR_NODE_HAS_TWO_CHILDREN;
        DBG("ERROR:NODE_CAN'T_HAVE_CHILDREN")

        return nullptr;
    }

    struct node* new_node = (struct node*) calloc (1, sizeof(struct node));

    if(new_node == nullptr) {   
        myTree->error = ERROR_NULL_NODE_PTR;
        DBG("ERROR:NODE_HAS_NULLPTR")

        return nullptr;
    }
    if(node_type == FUNC_NAME) {
        new_node->type_of_value = node_type;
        new_node->value.text = text;
        new_node->lefty = left_child;
        new_node->righty = right_child;
    } else if(node_type == EMPTY || node_type == ERROR) {
        new_node->type_of_value = node_type;
        new_node->value.operation = node_type;
        new_node->lefty  = nullptr;
        new_node->righty = nullptr;
    } else if(node_type == VAL) {
        new_node->value.number = node_value;
        new_node->type_of_value = node_type;
        new_node->lefty  = left_child;
        new_node->righty = right_child;
    } else if ((node_type == VAR) && text != nullptr) {
        new_node->type_of_value = node_type;
        new_node->value.text = text;
        new_node->lefty  = left_child;
        new_node->righty = right_child; 
    } else if ((node_type == OP)){
        new_node->value.operation = int (node_value);
        new_node->type_of_value = node_type;
        new_node->lefty  = left_child;
        new_node->righty = right_child; 
    } else if ((node_type == DECL_FUNC_HEAD)) {
        new_node->type_of_value = node_type;
        new_node->lefty  = left_child;
        new_node->righty = right_child; 
    }
    return new_node;
}

void treePrint (struct node* node, FILE* output) {
    switch (node->type_of_value) {
        case VAL:
            fprintf (output, "( %f ", node->value.number);
            break;

        case VAR:
        case FUNC_NAME:
            fprintf (output, "( %s ", node->value.text);
            break;

        case EMPTY:
            fprintf (output, "( EMPTY ");
            break;
        
        case OP:
            fprintf (output,  "( %s ", convertCodeToOp (node->value.operation));
            break;


        case DECL_FUNC_HEAD:
            fprintf (output, "( DECL_FUNC_HEAD ");
            break;
        default:
            DBG ("ERROR::problem with treePrint")
            break;
    };

    if (node->lefty != nullptr) {
        treePrint (node->lefty, output);
    }

    if (node->righty != nullptr) {
        treePrint (node->righty, output);
    }

    fprintf (output, ") ");

}
