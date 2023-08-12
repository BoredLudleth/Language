#include "backend.hpp"

int translateTreeToAsm (struct node* myTree)  {
    char asm_file_name[MAX_FILE_NAME] = "";

    printf ("How do you want to name .asm file?\n");
    scanf ("%s", asm_file_name);

    FILE* asm_file = fopen (asm_file_name, "w+");

    if (asm_file == nullptr) {
        return TRANSLATE_ERROR_ASM_FILE_NOT_OPENED;
    }

    CHECK_ERRORS (translateProgramToAsm (myTree, asm_file));

    fclose (asm_file);

    return TRANSLATE_OK;
}

int translateProgramToAsm (struct node* myTree, FILE* asm_file) {
    fprintf (asm_file, "jump :main\n");

    CHECK_ERRORS (translateProgBodyToAsm (myTree, asm_file));

    fprintf (asm_file, "hlt\n");
    return TRANSLATE_OK;
}

int translateProgBodyToAsm (struct node* myTree, FILE* asm_file) {
    switch (myTree->type_of_value) {
        case EMPTY:
            if (myTree->lefty != nullptr) {
                CHECK_ERRORS (translateProgBodyToAsm (myTree->lefty, asm_file));
            }

            if (myTree->righty != nullptr) {
                CHECK_ERRORS (translateProgBodyToAsm (myTree->righty, asm_file));
            }

            break;
        case DECL_FUNC_HEAD:
            CHECK_ERRORS (translateDeclFuncToAsm (myTree, asm_file));
            break;
        case OP:
            if (myTree->value.operation == T_MAIN) {
                CHECK_ERRORS (translateMainToAsm (myTree, asm_file));
            }
            break;
        default:
            DBG ("ERROR::syntax error")
            return TRANSLATE_SYNTAX_ERROR;
            break;
    };
}

int translateDeclFuncToAsm (struct node* myTree, FILE* asm_file) {
    fprintf (asm_file, ":%s\n", myTree->lefty->value.text);
    
    CHECK_ERRORS (translateVarsDeclToAsm (myTree->righty->lefty, asm_file));
    CHECK_ERRORS (translateDeclFuncVarsToAsm (myTree->righty->lefty, asm_file));
    CHECK_ERRORS (translateCommandsToAsm (myTree->righty->righty, asm_file));

    return TRANSLATE_OK;
}

int translateVarsDeclToAsm (struct node* myTree, FILE* asm_file) {
    if (myTree->type_of_value == EMPTY) {
        if (myTree->lefty != nullptr) {
            CHECK_ERRORS (translateVarsDeclToAsm (myTree->lefty, asm_file));
        }

        if (myTree->righty != nullptr) {
            CHECK_ERRORS (translateVarsDeclToAsm (myTree->righty, asm_file));
        }
    } else {
        fprintf (asm_file, "var %s\n", myTree->value.text);
    }

    return TRANSLATE_OK;
}

int translateDeclFuncVarsToAsm (struct node* myTree, FILE* asm_file) {
    if (myTree->type_of_value == EMPTY) {
        if (myTree->righty != nullptr) {
            CHECK_ERRORS (translateDeclFuncVarsToAsm (myTree->righty, asm_file));
        }

        if (myTree->lefty != nullptr) {
            CHECK_ERRORS (translateDeclFuncVarsToAsm (myTree->lefty, asm_file));
        }
    } else {
        fprintf (asm_file, "pop %s\n", myTree->value.text);
    }

    return TRANSLATE_OK;
}

int translateMainToAsm (struct node* myTree, FILE* asm_file) {
    fprintf (asm_file, ":main\n");

    CHECK_ERRORS (translateCommandsToAsm (myTree->righty, asm_file));

    return TRANSLATE_OK;
}

int translateCommandsToAsm (struct node* myTree, FILE* asm_file) {
    if (myTree->type_of_value == EMPTY) {
        if (myTree->lefty != nullptr) {
            CHECK_ERRORS (translateCommandsToAsm (myTree->lefty, asm_file));
        }

        if (myTree->righty != nullptr) {
            CHECK_ERRORS (translateCommandsToAsm (myTree->righty, asm_file));
        }
    } else if (myTree->type_of_value == OP) {
        CHECK_ERRORS (translateOpToAsm (myTree, asm_file));
    } else if (myTree->type_of_value == FUNC_NAME) {
        CHECK_ERRORS (translateFuncCallToAsm (myTree, asm_file));
    }

    return TRANSLATE_OK;
}

int translateOpToAsm (struct node* myTree, FILE* asm_file) {
    switch (myTree->value.operation) {
        case T_IN:
            fprintf (asm_file, "in\npop %s\n", myTree->lefty->value.text);
            break;
            
        case T_OUT:
            fprintf (asm_file, "push %s\nout\n", myTree->lefty->value.text);
            break;

        case T_WHILE:
            CHECK_ERRORS (translateWhileToAsm (myTree, asm_file));
            break;

        case T_ASSIGN:
            CHECK_ERRORS (translateRightValueToAsm (myTree->righty, asm_file));
            fprintf (asm_file, "pop %s\n", myTree->lefty->value.text);
            break;

        case T_IF:
            CHECK_ERRORS (translateIfToAsm (myTree, asm_file));
            break;

        case T_VAR:
            CHECK_ERRORS (translateAnnounceVarToAsm (myTree, asm_file));
            break;

        case T_RETURN:
            CHECK_ERRORS (translateReturnToAsm (myTree, asm_file));
            break;

        default:
            DBG ("ERROR:: current token has strange op_type")
            return TRANSLATE_ERROR_STRANGE_OP_CODE;
            break;
    };

    return TRANSLATE_OK;
}

int translateRightValueToAsm (struct node* myTree, FILE* asm_file) {
    if ((myTree->type_of_value == OP && myTree->value.operation != FUNC_NAME) && myTree->lefty != nullptr) {
        CHECK_ERRORS (translateRightValueToAsm (myTree->lefty, asm_file));
    }

    if ((myTree->type_of_value == OP && myTree->value.operation != FUNC_NAME) && myTree->righty != nullptr) {
        CHECK_ERRORS (translateRightValueToAsm (myTree->righty, asm_file));
    }

    switch (myTree->type_of_value) {
        case OP:
            CHECK_ERRORS (translateOperationToAsm (myTree, asm_file));
            break;

        case VAL:
            fprintf (asm_file, "push %f\n", myTree->value.number);
            break;

        case VAR:
            fprintf (asm_file, "push %s\n", myTree->value.text);
            break;

        case FUNC_NAME:
            CHECK_ERRORS (translateFuncCallToAsm (myTree, asm_file));
            break;
        
        default:
            DBG ("ERROR::problem with right value")
            return TRANSLATE_ERROR_RIGHT_VALUE;
            break;
    };

    return TRANSLATE_OK;
}

int translateOperationToAsm (struct node* myTree, FILE* asm_file) {
    switch (myTree->value.operation) {
        case T_PLUS:
            fprintf (asm_file, "add\n");
            break;

        case T_MINUS:
            fprintf (asm_file, "sub\n");
            break;

        case T_MULTIPLY:
            fprintf (asm_file, "mul\n");
            break;

        case T_DIVISION:
            fprintf (asm_file, "div\n");
            break;

        case T_SQRT:
            fprintf (asm_file, "sqrt\n");
            break;

        default:
            DBG ("ERROR::strange operation type")
            return TRANSLATE_ERROR_STRANGE_OP_CODE;
            break;
    };

    return TRANSLATE_OK;
}

int translateFuncCallToAsm (struct node* myTree, FILE* asm_file) {
    CHECK_ERRORS (translateCallArgsToAsm (myTree->lefty, asm_file));
    CHECK_ERRORS (translateCallArgsToAsm (myTree->lefty, asm_file));

    fprintf (asm_file, "call :%s\n", myTree->value.text);

    CHECK_ERRORS (translatePopCallArgsToAsm (myTree->lefty, asm_file));

    fprintf (asm_file, "push ax\n");

    return TRANSLATE_OK;
}

int translatePopCallArgsToAsm (struct node* myTree, FILE* asm_file) {
    if (myTree->type_of_value == EMPTY) {
        if (myTree->righty != nullptr) {
            CHECK_ERRORS (translatePopCallArgsToAsm (myTree->righty, asm_file));
        }

        if (myTree->lefty != nullptr) {
            CHECK_ERRORS (translatePopCallArgsToAsm (myTree->lefty, asm_file));
        }
    } else {
        fprintf (asm_file, "pop %s\n", myTree->value.text);
    }

    return TRANSLATE_OK;
}

int translateCallArgsToAsm (struct node* myTree, FILE* asm_file) {
    if (myTree->type_of_value == EMPTY) {
        if (myTree->lefty != nullptr) {
            CHECK_ERRORS (translateCallArgsToAsm (myTree->lefty, asm_file));
        }

        if (myTree->righty != nullptr) {
            CHECK_ERRORS (translateCallArgsToAsm (myTree->righty, asm_file));
        }
    } else {
        fprintf (asm_file, "push %s\n", myTree->value.text);
    }

    return TRANSLATE_OK;
}

int translateWhileToAsm (struct node* myTree, FILE* asm_file) {
    static int whileCount = 0;
    whileCount++;

    char whileCountToChar[MAX_COUNT_LENGTH] = "";
    char whileLabel[MAX_LABEL_LENGTH] = ":while";
    char exitLabel[MAX_LABEL_LENGTH] = ":while_ex";
    char doWhileLabel[MAX_LABEL_LENGTH] = ":do_while";

    strcat (whileLabel, inttoa (whileCount, whileCountToChar));
    strcat (exitLabel, whileCountToChar);
    strcat (doWhileLabel, whileCountToChar);

    fprintf (asm_file, "%s\n", whileLabel);

    CHECK_ERRORS (translateConditionToAsm (myTree->lefty, doWhileLabel, asm_file));
    
    fprintf (asm_file, "jump %s\n", exitLabel);
    fprintf (asm_file, "%s\n", doWhileLabel);

    CHECK_ERRORS (translateCommandsToAsm (myTree->righty, asm_file));

    fprintf (asm_file, "jump %s\n", whileLabel);
    fprintf (asm_file, "%s\n", exitLabel);

    return TRANSLATE_OK;
}

int translateIfToAsm (struct node* myTree, FILE* asm_file) {
    static int ifCount = 0;
    ifCount++;

    char ifCountToChar[MAX_COUNT_LENGTH] = "";
    char ifLabel[MAX_LABEL_LENGTH] = ":if";
    char elseLabel[MAX_LABEL_LENGTH] = ":if_el";
    char exitLabel[MAX_LABEL_LENGTH] = ":if_ex";

    strcat (ifLabel, inttoa (ifCount, ifCountToChar));
    strcat (elseLabel, ifCountToChar);
    strcat (exitLabel, ifCountToChar);

    CHECK_ERRORS (translateConditionToAsm (myTree->lefty, ifLabel, asm_file));

    fprintf (asm_file, "jump %s\n", elseLabel);
    fprintf (asm_file, "%s\n", ifLabel);
    
    CHECK_ERRORS (translateCommandsToAsm (myTree->righty->lefty, asm_file));
    
    fprintf (asm_file, "jump %s\n", exitLabel);
    fprintf (asm_file, "%s\n", elseLabel);                  // else

    if (myTree->righty->righty != nullptr) {
        CHECK_ERRORS (translateCommandsToAsm (myTree->righty->righty, asm_file));
    }

    fprintf (asm_file, "%s\n", exitLabel);

    return TRANSLATE_OK;
}

int translateConditionToAsm (struct node* myTree, char* label, FILE* asm_file) {
    if (myTree->lefty != nullptr) {
        CHECK_ERRORS (translateRightValueToAsm (myTree->lefty, asm_file));
    } else {
        DBG ("ERROR::problem with condition")
        
        return TRANSLATE_ERROR_CONDITION;
    }

    if (myTree->righty != nullptr) {
        CHECK_ERRORS (translateRightValueToAsm (myTree->righty, asm_file));
    } else {
        DBG ("ERROR::problem with condition")
        
        return TRANSLATE_ERROR_CONDITION;
    }

    switch (myTree->value.operation) {
        case T_GREATER:
            fprintf (asm_file, "ja %s\n", label);
            break;

        case T_GREATER_OR_EQ:
            fprintf (asm_file, "jae %s\n", label);
            break;

        case T_LESS:
            fprintf (asm_file, "jb %s\n", label);
            break;

        case T_LESS_OR_EQ:
            fprintf (asm_file, "jbe %s\n", label);
            break;

        case T_EQUAL:
            fprintf (asm_file, "je %s\n", label);
            break;

        default:
            DBG ("ERROR:: it isn't condition")
            return TRANSLATE_ERROR_CONDITION;
            break;
    };

    return TRANSLATE_OK;
}

int translateAnnounceVarToAsm (struct node* myTree, FILE* asm_file) {
    if (myTree->lefty->type_of_value == OP && myTree->lefty->value.operation == T_ASSIGN) {
        fprintf (asm_file, "var %s\n", myTree->lefty->lefty->value.text);
        CHECK_ERRORS (translateCommandsToAsm (myTree->lefty, asm_file));
    } else {
        fprintf (asm_file, "var %s\n", myTree->lefty->value.text);
    }

    return TRANSLATE_OK;
}

int translateReturnToAsm (struct node* myTree, FILE* asm_file) {
    if (myTree->lefty != nullptr) {
        CHECK_ERRORS (translateRightValueToAsm (myTree->lefty, asm_file));
        fprintf (asm_file, "pop ax\n");
    }

    fprintf (asm_file, "ret\n");

    return TRANSLATE_OK;
}

void encryptError (int error_code) {
    switch (error_code) {
        case TRANSLATE_OK:
            printf ("Translation is ok, no errors\n");
            break;

        case TRANSLATE_ERROR_ASM_FILE_NOT_OPENED:
            printf ("Translation has problems. Error asm-file didn't opened\n");
            break;

        case TRANSLATE_SYNTAX_ERROR:
            printf ("Translation has problems. Error in syntax.\n");
            break;
            
        case TRANSLATE_ERROR_STRANGE_OP_CODE:
            printf ("Translation has problems. Error in strange op-code.\n");
            break;
            
        case TRANSLATE_ERROR_RIGHT_VALUE:
            printf ("Translation has problems. Error in right value.\n");
            break;
            
        case TRANSLATE_ERROR_CONDITION:
            printf ("Translation has problems. Error in condition.\n");
            break;
            
        default:
            printf ("Translation has problems. Unnamed error.\n");
            break;
    };
}
