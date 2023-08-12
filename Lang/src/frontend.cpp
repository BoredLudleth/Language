#include "frontend.hpp"
#include "lex_settings.hpp"

void readFile (struct prog_data* data, char* progFileName) {
    data->program = fopen (progFileName, "r+");

    int len_of_file = lenFile(data->program);
    
    data->text = (char*) calloc (len_of_file, sizeof(char));
    fread(data->text, sizeof(char), (size_t) len_of_file, data->program);

    data->tree = (struct tree*) calloc (1, sizeof (struct tree));
    treeCtor (data->tree);
}

void tokenInit (struct prog_data* data) {
    data->token = (struct token*) calloc (DEFAULT_TOKENS, sizeof (struct token));
    for (int i = 0; i < DEFAULT_TOKENS; i++) {
        data->token[i].t_type = POISON;
        data->token[i].value = {.number = POISON};
    }

    data->token_size = DEFAULT_TOKENS;  
    data->current_token = 0;
}

void tokenResize (struct prog_data* data) {
    DBG ("i already resized token array")
    int new_token_size = data->token_size * TOKEN_RESIZE_COEFFICIENT;

    data->token = (struct token*) realloc (data->token, new_token_size * sizeof (data->token[0]));

    for (int i = data->token_size; i < new_token_size; i++) {
        data->token[i].t_type = POISON;
        data->token[i].value = {.number = POISON};
    }

    data->token_size = new_token_size;
}
void tokenezation (struct prog_data* data) {
    char* tok = strtok (data->text, " ");

    while (tok != NULL) {
        if (data->current_token == data->token_size) {
            tokenResize (data);
        }

        if (valueTypeTok (&(data->token[data->current_token]), tok) == IN_COMMENT) {
            while (strcmp (tok, _END_COMMENT)) {
                tok = strtok (NULL, " \n\t");
            }
        } else {
            data->current_token++;
        }
        
        tok = strtok (NULL, " \n\t");
    }
}

int valueTypeTok (struct token* token, char* word) {
    if (!strcmp (word, _DOLLAR)) {
        token->t_type = OPERATION;
        token->value.operation = T_DOLLAR;
        return OUT_COMMENT;
    } else if (!strcmp (word, _ASSIGN)) {
        token->t_type = OPERATION;
        token->value.operation = T_ASSIGN;
        return OUT_COMMENT;
    } else if (!strcmp (word, _PLUS)) {
        token->t_type = OPERATION;
        token->value.operation = T_PLUS;
        return OUT_COMMENT;
    } else if (!strcmp (word, _MINUS)) {
        token->t_type = OPERATION;
        token->value.operation = T_MINUS;
        return OUT_COMMENT;
    } else if (!strcmp (word, _MULTIPLY)) {
        token->t_type = OPERATION;
        token->value.operation = T_MULTIPLY;
        return OUT_COMMENT;
    }  else if (!strcmp (word, _DIVISION)) {
        token->t_type = OPERATION;
        token->value.operation = T_DIVISION;
        return OUT_COMMENT;
    } else if (!strcmp (word, _IF)) {
        token->t_type = OPERATION;
        token->value.operation = T_IF;
        return OUT_COMMENT;
    } else if (!strcmp (word, _ELSE)) {
        token->t_type = OPERATION;
        token->value.operation = T_ELSE;
        return OUT_COMMENT;
    } else if (!strcmp (word, _WHILE)) {
        token->t_type = OPERATION;
        token->value.operation = T_WHILE;
        return OUT_COMMENT;
    } else if (!strcmp (word, _FOR)) {
        token->t_type = OPERATION;
        token->value.operation = T_FOR;
        return OUT_COMMENT;
    } else if (!strcmp (word, _EQUAL)) {
        token->t_type = OPERATION;
        token->value.operation = T_EQUAL;
        return OUT_COMMENT;
    }  else if (!strcmp (word, _NOT_EQUAL)) {
        token->t_type = OPERATION;
        token->value.operation = T_NOT_EQUAL;
        return OUT_COMMENT;
    } else if (!strcmp (word, _RETURN)) {
        token->t_type = OPERATION;
        token->value.operation = T_RETURN;
        return OUT_COMMENT;
    } else if (!strcmp (word, _VAR)) {
        token->t_type = OPERATION;
        token->value.operation = T_VAR;
        return OUT_COMMENT;
    } else if (!strcmp (word, _MAIN)) {
        token->t_type = OPERATION;
        token->value.operation = T_MAIN;
        return OUT_COMMENT;
    } else if (!strcmp (word, _FUNCT)) {
        token->t_type = OPERATION;
        token->value.operation = T_FUNCT;
        return OUT_COMMENT;
    } else if (!strcmp (word, _LEFT_SCOPE)) {
        token->t_type = OPEN_SCOPE;
        return OUT_COMMENT;
    } else if (!strcmp (word, _RIGHT_SCOPE)) {
        token->t_type = CLOSE_SCOPE;
        return OUT_COMMENT;
    } else if (!strcmp (word, _LEFT_FIGURE_SCOPE)) {
        token->t_type = OPEN_SCOPE;
        return OUT_COMMENT;
    } else if (!strcmp (word, _RIGHT_FIGURE_SCOPE)) {
        token->t_type = CLOSE_SCOPE;
        return OUT_COMMENT;
    } else if (!strcmp (word, _GREATER)) {
        token->t_type = OPERATION;
        token->value.operation = T_GREATER;
        return OUT_COMMENT;
    } else if (!strcmp (word, _LESS)) {
        token->t_type = OPERATION;
        token->value.operation = T_LESS;
        return OUT_COMMENT;
    } else if (!strcmp (word, _GREATER_OR_EQ)) {
        token->t_type = OPERATION;
        token->value.operation = T_GREATER_OR_EQ;
        return OUT_COMMENT;
    } else if (!strcmp (word, _LESS_OR_EQ)){
        token->t_type = OPERATION;
        token->value.operation = T_LESS_OR_EQ;
        return OUT_COMMENT;
    } else if (!strcmp (word, _NOT_EQUAL)) {
        token->t_type = OPERATION;
        token->value.operation = T_NOT_EQUAL;
        return OUT_COMMENT;
    } else if (!strcmp (word, _START_COMMENT)) {
        return IN_COMMENT;
    } else if (!strcmp (word, _ELSE)){
        token->t_type = OPERATION;
        token->value.operation = T_ELSE;
        return OUT_COMMENT;
    } else if (!strcmp (word,  _IN)) {
        token->t_type = OPERATION;
        token->value.operation = T_IN;
        return OUT_COMMENT;
    } else if (!strcmp (word, _OUT)) {
        token->t_type = OPERATION;
        token->value.operation = T_OUT;
        return OUT_COMMENT;
    } else if (!strcmp (word, _SQRT)) {
        token->t_type = OPERATION;
        token->value.operation = T_SQRT;
        return OUT_COMMENT;
    } else if (!atof (word)) {
        token->t_type = TEXT;
        token->value.text = word;
        return OUT_COMMENT;
    } else {
        token->t_type = NUMBER;
        token->value.number = atof(word);
        return OUT_COMMENT;
    }
}

int lenFile (FILE *text) {
    fseek (text, 0, SEEK_END);
    int length =  ftell (text);
    fseek (text, 0, SEEK_SET);

    return length;    
}

struct tree* readG (struct prog_data* data) {
    printf ("i entered readG\n");
    data->current_token = 0;

    data->tree->head = treeAddNode (data->tree, EMPTY, EMPTY, _EMPTY);

    struct node* head = data->tree->head;

    while (data->token[data->current_token].t_type == OPERATION && data->token[data->current_token].value.operation == T_FUNCT) {
        (data->current_token)++;

        data->tree->head->lefty = readFunct (data);
        data->tree->head->righty = treeAddNode (data->tree, EMPTY, EMPTY, _EMPTY);

        data->tree->head = data->tree->head->righty;
    }

    // data->tree->head->lefty = treeAddNode (data->tree, MAIN, OPERATION, _MAIN);
    data->tree->head->righty = readMain (data);

    data->tree->head = head;
    printf ("i escaped from readG\n");
    
    return data->tree;
}

struct node* readFunct (struct prog_data* data) {
    printf ("i entered in readFunct\n");

    struct node* head = treeAddNode (data->tree, DECL_FUNC_HEAD, DECL_FUNC_HEAD);

    head->lefty = getFuncName (data);
    head->righty = treeAddNode (data->tree, EMPTY, EMPTY, _EMPTY);
    (data->current_token)++;
    if (data->token[data->current_token].t_type == OPEN_SCOPE) {
        (data->current_token)++;
        head->righty->lefty = getArgs (data);
    } else {
        DBG("ERROR::need open scope\n");
    }
    
    if (data->token[data->current_token].t_type == OPEN_SCOPE) {
        (data->current_token)++;
        head->righty->righty = readCommands (data);
    } else {
        DBG("ERROR::need open scope\n")
    }

    // if (data->token[data->current_token].t_type != CLOSE_SCOPE) {
    //     DBG("ERROR::need close scope\n")
    // } else {
    //     data->current_token++;
    // }

    printf ("i escaped from readFunct\n");

    return head;
}

struct node* getFuncName (struct prog_data* data) {
     return treeAddNode (data->tree, FUNC_NAME, FUNC_NAME, data->token[data->current_token].value.text);
}

struct node* getArgs (struct prog_data* data) {
    struct node* head = treeAddNode (data->tree, EMPTY, EMPTY, _EMPTY);
    struct node* s_head = head;

    while (data->token[data->current_token].t_type == VAR) {
        head->lefty = treeAddNode (data->tree, VAR, VAR, data->token[data->current_token].value.text);
        head->righty = treeAddNode (data->tree, EMPTY, EMPTY, _EMPTY);

        (data->current_token)++;

        head = head->righty;
    }

    if (data->token[data->current_token].t_type == CLOSE_SCOPE) {
        (data->current_token)++;
        return s_head;
    } else {
        printf ("ERROR::need close scope\n");
        return nullptr;
    }
}

struct node* readMain (struct prog_data* data) {
    printf ("i entered in readMain\n");

    struct node* head = nullptr;

    if (data->token[data->current_token].t_type != OPERATION && data->token[data->current_token].value.operation != MAIN) {
        DBG("ERROR::need main\n")
        printf ("%d\n", data->current_token);
    } else {
        head = treeAddNode (data->tree, T_MAIN, OP, _MAIN);
        data->current_token++;
    }
    if (data->token[data->current_token].t_type == OPEN_SCOPE) {
        data->current_token++;
        
        head->righty = readCommands (data);
    }
    printf ("i escaped from readMain\n");

    return head;
}

struct node* readCommands (struct prog_data* data) {
    printf ("CURRENT TOKEN IS %d\n", data->current_token);
    struct node* head = nullptr;

    printf ("i entered in readCommands\n");

    if (data->token[data->current_token].t_type != CLOSE_SCOPE) {
        head = treeAddNode (data->tree, EMPTY, EMPTY, _EMPTY);
        if (data->token[data->current_token].t_type == OPERATION) {
            switch (data->token[data->current_token].value.operation){
                case T_IN:
                    head->lefty = treeAddNode (data->tree, T_IN, OP);
                    data->current_token++;
                    head->lefty->lefty = readVar (data);
                    readDollar (data);
                    head->righty = readCommands (data);
                    break;

                case T_OUT:
                    head->lefty = treeAddNode (data->tree, T_OUT, OP);
                    data->current_token++;
                    head->lefty->lefty = readVar (data);
                    readDollar (data);
                    head->righty = readCommands (data);
                    break;

                case T_IF:
                    head->lefty = readIf (data);
                    head->righty = readCommands (data);
                    break;

                case T_WHILE:
                    head->lefty = readWhile (data);
                    head->righty = readCommands (data);
                    break;

                case T_FOR:
                    head->lefty = readFor (data);
                    head->righty = readCommands (data);
                    break;

                case T_VAR:
                    data->current_token++;
                    head->lefty = readAnnounceVar (data);
                    head->righty = readCommands (data);
                    break;

                case T_RETURN:
                    head->lefty = readReturn (data);
                    head->righty = readCommands (data);
                    break;

                default:
                    DBG("ERROR: this operation can't start an operation\n")
                    break;
            }
        } else if (data->token[data->current_token].t_type == VARIABLE || data->token[data->current_token].t_type == TEXT) {
            head = treeAddNode (data->tree, EMPTY, EMPTY, _EMPTY);
            if (data->token[data->current_token + 1].t_type == OPEN_SCOPE) {
                data->token[data->current_token].t_type = TEXT;
                head->lefty = readFunctCall (data);
                head->righty = readCommands (data);
                
            } else {
                data->token[data->current_token].t_type = VARIABLE;

                head->lefty = readE (data);
                head->righty = readCommands (data);
            }
        } else if (data->token[data->current_token].t_type == NUMBER) {
            DBG ("ERROR: number can't be left value\n")
        }
    } else {
        data->current_token++;
    }

    printf ("i escaped from readCommands\n");

    return head;
}

struct node* readFunctCall (struct prog_data* data) {
    struct node* head = getFuncName (data);
    data->current_token++;

    if (data->token[data->current_token].t_type == OPEN_SCOPE) {
        data->current_token++;
        head->lefty = getArgs (data);
    }

    return head;
}

struct node* readReturn (struct prog_data* data) {
    struct node* head = treeAddNode (data->tree, T_RETURN, OP, _RETURN);
    (data->current_token)++;

    if (data->token[data->current_token].t_type == OPERATION && data->token[data->current_token].value.operation == T_DOLLAR) {
        data->current_token++;
        return head;
    } else {
        head->lefty = readRightValue (data);

        return head;
    }
}

struct node* readAnnounceVar (struct prog_data* data) {
    printf ("i entered readAnnounceVar\n");

    struct node* head = treeAddNode (data->tree, T_VAR, OP, _VAR);

    struct node* var = readVar (data);

    if (data->token[data->current_token].t_type == OPERATION && data->token[data->current_token].value.operation == T_ASSIGN) {
        head->lefty = treeAddNode (data->tree, T_ASSIGN, OP);
        data->current_token++;
        head->lefty->lefty = var;
        head->lefty->righty = readRightValue (data);
        printf ("i escaped from readAnnouceVar\n");
        return head;
    }

    head->righty = var;

    printf ("i escaped from readAnnouceVar\n");
    return head;
}

struct node* readWhile (struct prog_data* data) {
    printf ("i'm in readWhile\n");
    struct node* head = treeAddNode (data->tree, T_WHILE, OP, _WHILE);
    data->current_token++;

    if (data->token[data->current_token].t_type != OPEN_SCOPE) {
        DBG ("ERROR::need open scope")
    } else {
        data->current_token++;
        head->lefty = readLogE (data);
        if (data->token[data->current_token].t_type == CLOSE_SCOPE) {
            data->current_token++;
        } else {
            DBG ("ERROR::need close scope")
        }
    }

    if (data->token[data->current_token].t_type == OPEN_SCOPE) {
        data->current_token++;
        head->righty = readCommands (data);
    } else {
        DBG ("ERROR::need open scope")
    }

    printf ("i escaped readWhile\n");

    return head;
}

struct node* readFor (struct prog_data* data) {
    printf ("i'm in readFor\n");
    struct node* head = treeAddNode (data->tree, EMPTY, EMPTY);
    head->righty = treeAddNode (data->tree, T_WHILE, OP);           //here was T_FOR
    data->current_token++;

    if (data->token[data->current_token].t_type == OPEN_SCOPE) {
        data->current_token++;
        if (data->token[data->current_token].t_type == OPERATION && data->token[data->current_token].value.operation == T_DOLLAR) {
            data->current_token++;
        } else {
            head->lefty = readE (data);
        }
    } else {
        DBG ("ERROR:: nedd open scope")
    }

    if (data->token[data->current_token].t_type == OPERATION && data->token[data->current_token].value.operation == T_DOLLAR) {
        data->current_token++;
    } else {
        head->righty->lefty = readLogE (data);
    }

    struct node* lastCond = nullptr;

    if (data->token[data->current_token].t_type != CLOSE_SCOPE) {
        lastCond = readE (data);
    }

    if (data->token[data->current_token].t_type != CLOSE_SCOPE) {
        DBG ("ERROR::need close scope")
    } else {
        data->current_token++;
    }

    if (data->token[data->current_token].t_type != OPEN_SCOPE) {
        DBG ("ERROR::need open scope")
    } else {
        data->current_token++;
    }

    head->righty->righty = readCommands (data);

    struct node* findEmpty = head;
    while (findEmpty->righty != nullptr) {
        findEmpty = findEmpty->righty;
    }

    findEmpty->righty = treeAddNode (data->tree, EMPTY, EMPTY);
    findEmpty->righty->lefty = lastCond;

    return head;
}

struct node* readIf (struct prog_data* data) {
    printf ("i'm in readIf\n");
    struct node* head = treeAddNode (data->tree, T_IF, OP, _IF);
    data->current_token++;

    if (data->token[data->current_token].t_type != OPEN_SCOPE) {
        DBG("ERROR:: need open scope\n");
    } else {
        data->current_token++;
        head->lefty = readLogE (data);
        if (data->token[data->current_token].t_type == CLOSE_SCOPE) {
            data->current_token++;
        } else {
            DBG ("ERROR::need close scope")
        }
    }
    
    if (data->token[data->current_token].t_type == OPEN_SCOPE) {
        data->current_token++;
        head->righty = treeAddNode (data->tree, EMPTY, EMPTY);
        head->righty->lefty = readCommands (data);
    } else {
        DBG ("ERROR:: need open scope")
    }

    if (data->token[data->current_token].t_type == OPERATION  && data->token[data->current_token].value.operation == T_ELSE) {
        data->current_token++;
        head->righty->righty = readElse (data);
    }

    printf ("i escaped readIf\n");

    return head;
}

struct node* readElse (struct prog_data* data) {
    printf ("i'm in readElse\n");

    struct node* head = nullptr;

    if (data->token[data->current_token].t_type == OPEN_SCOPE) {
        data->current_token++;
        head = readCommands (data);
    } else {
        DBG ("ERROR:: need open scope")
    }

    printf ("i escaped readElse\n");

    return head;
}

struct node* readLogE (struct prog_data* data) {
    printf ("i entered readLogE\n");

    struct node* lefty = readRightValue (data);

    struct node* head = treeAddNode (data->tree, data->token[data->current_token].value.operation, OP);

    head->lefty = lefty;
    data->current_token++;

    head->righty = readRightValue (data);

    printf ("i escaped from readLogE\n");
    return head;
}

struct node* readCell (struct prog_data* data) {
    printf ("i am in readCell\n");
    printf ("i escaped from readCell\n");

    switch (data->token[data->current_token].t_type){
        case NUMBER:
            return readNum (data);
            break;
        case VARIABLE:
        case TEXT:
            if (data->token[data->current_token + 1].t_type == OPEN_SCOPE) {
                return readFunctCall (data);
            } else {
                return readVar (data);
            }
            break;
        case OPERATION :
            DBG("ERROR::can't be operation\n")
            break;
        default:
            DBG("ERROR::unknown type\n")
            break;
    }

    return nullptr;
}

struct node* readNum (struct prog_data* data) {
    printf("i am in readNum\n");
    printf ("i escaped readNum\n");
    data->current_token++;
    return treeAddNode (data->tree, data->token[data->current_token - 1].value.number, VAL);
}

struct node* readVar (struct prog_data* data) {
    printf ("i am in readVar\n");
    printf ("i escaped from readVar\n");
    data->current_token++;
    return treeAddNode (data->tree, VAR, VAR, data->token[data->current_token - 1].value.text);
}

struct node* readE (struct prog_data* data) {
    printf ("i am in readE\n");
    struct node* head = treeAddNode (data->tree, T_ASSIGN, OP);
    head->lefty = readVar (data);
    
    if (data->token[data->current_token].t_type == OPERATION && data->token[data->current_token].value.operation == T_ASSIGN) {
        data->current_token++;
        head->righty = readRightValue (data);
    } else {
        DBG("ERROR::must be =\n")
    }

    printf ("i escaped from readE\n");

    return head;
}

struct node* readRightValue (struct prog_data* data) {
    printf ("i'm in readRightValue\n");

    struct node* head = getE (data);

    if (data->token[data->current_token].t_type == OPERATION && data->token[data->current_token].value.operation == T_DOLLAR) {
        printf ("i escaped from readRightValue\n");
        data->current_token++;
        return head;
    }

    if (data->token[data->current_token].t_type != NUMBER && data->token[data->current_token].t_type != VARIABLE &&
        (data->token[data->current_token].t_type == OPERATION && (data->token[data->current_token].value.operation != T_DIVISION ||
        data->token[data->current_token].value.operation != T_MULTIPLY || data->token[data->current_token].value.operation != T_PLUS ||
        data->token[data->current_token].value.operation != T_MINUS))) {
        printf ("i escaped from readRightValue\n");
        return head;
    }

    printf ("i escaped from readRightValue\n");
    return head;

}

struct node* getE (struct prog_data* data) {
    printf ("i'm in getE\n");

    struct node* head = nullptr;
    struct node* lefty = getT (data);
    head = lefty;


    while (data->token[data->current_token].t_type == OPERATION && (data->token[data->current_token].value.operation == T_PLUS || data->token[data->current_token].value.operation == T_MINUS)) {
        head = treeAddNode (data->tree, data->token[data->current_token].value.operation, OP);
        data->current_token++;
        struct node* righty = getT (data);
        head->lefty = lefty;
        head->righty = righty;
        lefty = head;
    }

    printf ("i escaped getE\n");

    return head;
}

struct node* getT (struct prog_data* data) {
    printf ("i'm in getT\n");

    struct node* head = nullptr;
    struct node* lefty = getSq (data);
    head = lefty;

    while (data->token[data->current_token].t_type ==  OPERATION && (data->token[data->current_token].value.operation == T_MULTIPLY || data->token[data->current_token].value.operation == T_DIVISION)) {
        head = treeAddNode (data->tree, data->token[data->current_token].value.operation, OP);
        data->current_token++;
        struct node* righty = getSq (data);
        head->lefty = lefty;
        head->righty = righty;
        lefty = head;
    }


    printf ("i escaped from getT\n");
    return head;
}

struct node* getSq (struct prog_data* data)  {
    printf ("i'm in getSq %d\n", data->current_token);
    struct node* head = nullptr;
    if (data->token[data->current_token].t_type == OPERATION && data->token[data->current_token].value.operation == T_SQRT) {
        head = treeAddNode (data->tree, T_SQRT, OP);
        data->current_token++;
        head->lefty = getE (data);
    } else {
        head = getP (data);
    }
    printf ("i'm escaped getSq %d\n", data->current_token);

    return head;
}

struct node* getP (struct prog_data* data) {
    printf ("i'm in getP %d\n", data->current_token);
    struct node* head = nullptr;
    if (data->token[data->current_token].t_type == OPEN_SCOPE) {
        data->current_token++;
        head = getE (data);
        if (data->token[data->current_token].t_type == CLOSE_SCOPE) {
            data->current_token++;
        } else {
            DBG("ERROR::need close scope\n")
        }
    } else {
        head = readCell (data);
    }
    printf ("i'm escaped getP %d\n", data->current_token);

    return head;
}

void free_mem (struct prog_data* data) {
    data->current_token = POISON;
    fclose (data->program);
    free (data->text);
    free (data->token);
    data->token_size = POISON;
    treeDtor (data->tree);
}

void readDollar (struct prog_data* data) {
    if (data->token[data->current_token].t_type == OPERATION && data->token[data->current_token].value.operation == T_DOLLAR) {
        data->current_token++;
    } else {
        DBG ("ERROR:: need dollar")
    }
}
