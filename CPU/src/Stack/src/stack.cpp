#include "stackoperations.hpp"
#include "stack.hpp"

void cpuInit (struct cpu* cpu) {
    stackCtor (&cpu->stack);
    stackCtor (&cpu->functstack);
}

void stackCtor (struct stack* stack) {
    stack->filled = 0;
    stack->lengthStack = DEFAULT_STACK_LENGTH;
    stack->data = (type*) calloc (stack->lengthStack * sizeof(type) + 2 * sizeof(int), 1); //two extras for canaries

    int* temp_p = (int*) (stack->data);
    temp_p[0] = CANARY_VALUE;
    stack->data = (type*) (temp_p + 1);
    temp_p = (int*) (stack->data) + (stack->lengthStack);
    temp_p[0] = CANARY_VALUE;

    stack->canary_1 = CANARY_VALUE;
    stack->canary_2 = CANARY_VALUE; 

    stack->error  = 0;

    for (int i = 0; i < stack->lengthStack; i++) {
        stack->data[i] = POISON;
    }

    if (stack->data == nullptr) {
        stack->error = ERR_NULL_DATA;
    }

    ASSERT(stack)
}

void stackDtor (struct stack* stack) {
    int* temp_p = (int*) stack->data;
    stack->data = (type*) (temp_p - 1);
    free (stack->data);

    stack->filled      = POISON;
    stack->lengthStack = POISON;

    stack->canary_1 = POISON;
    stack->canary_2 = POISON; 

    stack->error = POISON;
}

void stackDump (struct stack* stack) {
    void* temp_p = stack->data - 1;

    printf ("STACK DUMP\n");
    printf ("Canary1: %d\n", *((int*) temp_p));

    for (int i = 0; i < stack->lengthStack; i++) {
        printf ("%d - ", i);
        printf (TYPE_SPECIFIER, stack->data[i]);
        printf ("\n");
    }

    temp_p = (int*) (stack->data + stack->lengthStack);

    printf ("Canary2: %d\n", *((int*) temp_p));
}

void stackPush (struct stack* stack, type value) {
    if(stackCheck (stack)) {
        exit(0);
    }

    if (stack->filled == stack->lengthStack) {
        stack->error = ERR_STACK_IS_LITTLE;
    }

    ASSERT(stack)

    stack->data[stack->filled] = value;
    stack->filled++;

    ASSERT(stack)
}

type stackPop (struct stack* stack) {
    if (stack->filled <= 0) {
        stack->error = ERR_SIZE_OUT_LEFTRANGE;
    } 

    ASSERT(stack)

    stack->filled--;
    type value = stack->data[stack->filled];
    stack->data[stack->filled] = POISON;

    ASSERT(stack)

    return value;
}

int stackCheck (struct stack* stack) {
    if (stack->lengthStack == stack->filled) {
        stackResizeUp (stack);
    }
    if ((stack->lengthStack > 4 * stack->filled) && (stack->filled != 0)) {
        stackResizeDown (stack);
    }

    void* temp_p = stack->data - 1;
    if (*((int*) temp_p) != stack->canary_1) {
        stack->error = ERR_CANERY_CHANGED;
    }

    temp_p = (int*) (stack->data + stack->lengthStack);

    if (*((int*) temp_p) != stack->canary_2) {
        stack->error = ERR_CANERY_CHANGED;
    }

    switch (stack->error) {
        case NO_ERRORS:
            return 0;
            break;

        case ERR_STACK_IS_LITTLE:
            printf ("STACK::Resizing...\n");
            stackResizeUp (stack);
            stack->error = NO_ERRORS;
            return 0;
            break;

        case ERR_NULL_DATA:
            printf("ERROR:STACK::Data has nullptr\n");
            return 1;
            break;

        case ERR_CANERY_CHANGED:
            printf("ERROR:STACK::Out of stack range canery-changed\n");
            return 1;
            break;

        case ERR_SIZE_OUT_LEFTRANGE:
            printf("ERROR:STACK::It's non-changeble part of memory, you're out left range\n");
            break;

        case ERR_DIVIDE_ON_ZERO:
            printf("ERROR:STACK::Division on zero\n");
            break;

        default:
            printf ("ERROR:STACK::Undefined error\n");
            return 1;
            break;
    };
    return 1;
}

void stackResizeUp (struct stack* stack) {
    stack->lengthStack *= 2;

    type* array = (type*) calloc (stack->lengthStack * sizeof(type) + 2 * sizeof(int), 1); //two extras for canaries

    int* temp_p = (int*) (array);
    temp_p[0] = CANARY_VALUE;
    array = (type*) (temp_p + 1);
    temp_p = (int*) (array) + (stack->lengthStack);
    temp_p[0] = CANARY_VALUE;

    for (int i = 0; i < stack->lengthStack; i++) {
        if (i < stack->filled) {
            array[i] = stack->data[i];
            continue;
        }
        array[i] = POISON;
    }

    int* tmp = (int*) (stack->data - 1);
    stack->data = array;
    free (tmp);
}

void stackResizeDown (struct stack* stack) {
    stack->lengthStack /= 2;

    type* array = (type*) calloc (stack->lengthStack * sizeof(type) + 2 * sizeof(int), 1); //two extras for canaries

    int* temp_p = (int*) (array);
    temp_p[0] = CANARY_VALUE;
    array = (type*) (temp_p + 1);
    temp_p = (int*) (array) + (stack->lengthStack);
    temp_p[0] = CANARY_VALUE;

    for (int i = 0; i < stack->lengthStack; i++) {
        if (i < stack->filled) {
            array[i] = stack->data[i];
            continue;
        }
        array[i] = POISON;
    }

    int* tmp = (int*) (stack->data - 1);
    stack->data = array;
    free (tmp);
}


void stackRead (struct cpu* mycpu, struct stack* stack) {
    for (stack->cur = 0; stack->cur < stack->sizeOfProgramm; stack->cur++)
    {
        printf ("NC\n");
        if ((int) stack->allProgramm[stack->cur] == STACKPUSH)
        {
            stack->cur++;
            push(stack, stack->allProgramm[stack->cur]);
        } else if ((int) stack->allProgramm[stack->cur] == STACKPOP)
        {
            pop(stack);
        } else if ((int) stack->allProgramm[stack->cur] == STACKADD)
        {
            add(stack);
        } else if ((int) stack->allProgramm[stack->cur] == STACKSUB)
        {
            sub(stack);
        } else if ((int) stack->allProgramm[stack->cur] == STACKMUL)
        {
            mul(stack);
        } else if ((int) stack->allProgramm[stack->cur] == STACKDIV)
        {
            div(stack);
        } else if ((int) stack->allProgramm[stack->cur] == STACKOUT)
        {
            out(stack);
        } else if ((int) stack->allProgramm[stack->cur] == STACKPRINT)
        {
            print(*stack);
        } else if ((int) stack->allProgramm[stack->cur] == STACKDUMP)
        {
            dump(*stack);
        } else if ((int) stack->allProgramm[stack->cur] == STACKHLT)
        {
            hlt(stack);
        } else if ((int) stack->allProgramm[stack->cur] == STACKJUMP)
        {
            stack->cur++;
            jump(stack, (int) (stack->allProgramm[stack->cur] - 1));
        } else if ((int) stack->allProgramm[stack->cur] == STACKJB)
        {
            stack->cur++;
            jb(stack, (int) (stack->allProgramm[stack->cur] - 1));
        } else if ((int) stack->allProgramm[stack->cur] == STACKJBE)
        {
            stack->cur++;
            jbe(stack, (int) (stack->allProgramm[stack->cur] - 1));
        } else if ((int) stack->allProgramm[stack->cur] == STACKJA)
        {
            stack->cur++;
            ja(stack, (int) (stack->allProgramm[stack->cur] - 1));
        } else if ((int) stack->allProgramm[stack->cur] == STACKJAE)
        {
            stack->cur++;
            jae(stack, (int) (stack->allProgramm[stack->cur] - 1));
        } else if ((int) stack->allProgramm[stack->cur] == STACKJE)
        {
            stack->cur++;
            je(stack, (int) (stack->allProgramm[stack->cur] - 1));
        } else if ((int) stack->allProgramm[stack->cur] == STACKJNE)
        {
            stack->cur++;
            jne(stack, (int) (stack->allProgramm[stack->cur] - 1));
        } else if ((int) stack->allProgramm[stack->cur] == STACKRPUSH)
        {
            stack->cur++;
            int reg = (int) stack->allProgramm[stack->cur];
            pushr(stack, reg);
        } else if ((int) stack->allProgramm[stack->cur] == STACKRPOP)
        {
            stack->cur++;
            int reg = stack->allProgramm[stack->cur];
            popr(stack, reg);
        } else if ((int) stack->allProgramm[stack->cur] == STACKCALL) {
            stack->cur++;
            call (mycpu, (int) (stack->allProgramm[stack->cur] - 1));
        } else if ((int) stack->allProgramm[stack->cur] == STACKRET) {
            ret (mycpu);
        } else if ((int) stack->allProgramm[stack->cur] == STACKIN) {
            in (mycpu);
        } else if ((int) stack->allProgramm[stack->cur] == STACKSQRT) {
            sqr (mycpu);
        } else {
            printf ("current index: %d\ncurrent val: %f\n", stack->cur, stack->allProgramm[stack->cur]);
            printf("Undefined comand. Try again.\n");
            break;
        }

        print(*stack);
        printf("\n");
        printf ("stack::filled:%d\nsize:%d\n", mycpu->stack.filled, mycpu->stack.lengthStack);
        printf ("funkstack::filled:%d\nsize:%d\n", mycpu->functstack.filled, mycpu->functstack.lengthStack);
    }

    stack->allProgramm -= stack->sizeOfProgramm;
    stackDtor (stack);
}

int lenFile(FILE *text) {
    fseek (text, 0, SEEK_END);
    int length = ftell (text);
    fseek (text, 0, SEEK_SET);

    return length;    
}
