#include "stackoperations.hpp"
#include "stack.hpp"

int cmpType (type a, type b) {
    if (abs(a - b) < EPSILON) {
        return 1;
    }

    return 0;
}

void push (struct stack* stack, type value)
{
    stackCheck (stack);

    stack->data[stack->filled] = value;
    stack->filled += 1;

    stackCheck (stack);
}

void pushr(struct stack* stack, int indeficator)
{
    stackCheck (stack);
    if (indeficator == 1)
    {
        push(stack, stack->regs.ax);
    } else if (indeficator == 2) {
        push(stack, stack->regs.bx);
    } else if (indeficator == 3) {
        push(stack, stack->regs.cx);
    } else if (indeficator == 4) {
        push(stack, stack->regs.dx);
    } else {
        printf ("Error: Link to non-existent register\n");
        exit (0);
    }

    stackCheck (stack);
}

void pushv (struct stack* stack, int varIdentity) {
    push (stack, stack->vars[varIdentity].value);
}

type pop (struct stack* stack) {
    return stackPop (stack);
}

type popr (struct stack* stack, int indeficator)
{
    stackCheck (stack);

    switch (indeficator)
    {
        case (1):
            stack->regs.ax = pop(stack);
            break;
        case (2):
            stack->regs.bx = pop(stack);
            break;
        case (3):
            stack->regs.cx = pop(stack);
            break;
        case (4):
            stack->regs.dx = pop(stack);
            break;
        default:
            printf ("Error: Link to non-existent register\n");
            exit (0);
            break;
    }

    stackCheck (stack);

    return 1;
}

void popv (struct stack* stack, int varIdentity) {
    stack->vars[varIdentity].value = pop (stack);
}

void add (struct stack* stack)
{
    type x = pop(stack);
    type y = pop(stack);

    push(stack, y + x);
}

void sub (struct stack* stack)
{
    type x = pop(stack);
    type y = pop(stack);

    push(stack, y - x);
}

void mul (struct stack* stack)
{
    type x = pop(stack);
    type y = pop(stack);

    push(stack, x * y);
}

void div (struct stack* stack)
{
    type znam = pop(stack);
    type chisl = pop(stack);

    if (znam == 0)
    {
        push(stack, chisl);
        push(stack, znam);
        printf("You can't divide on 0, operation didn't complete");
        stack->error = ERR_DIVIDE_ON_ZERO;
    } else {
        push(stack, chisl / znam);
    }
    stackCheck(stack);
}

void out (struct stack* stack)
{
    printf ("\n\nout:");
    printf (TYPE_SPECIFIER, pop(stack));
    printf ("\n\n\n");
}

void print (struct stack stack)
{
    printf ("print:\n");
    for (int i = 0; i < stack.filled; i++)
    {
        printf("%d - ", i);
        printf(TYPE_SPECIFIER, stack.data[i]);
        printf("\n");
    }
}

void dump (struct stack stack)
{
    printf("\nDUMP\n");
    printf("LENGTH_STACK: %d\n", stack.lengthStack);
    printf("SIZE: %d\n", stack.filled);
    printf("ERROR CODE: %d\n", (enum Errors) stack.error);

    switch(stack.error)
    {
    case 0:
        printf("NO_ERRORS\n");
        break;

    case 2:
        printf("ERR_NULL_DATA\n");
        break;

    case 4:
        printf("ERR_CANERY_CHANGED\n");
        break;

    case 8:
        printf("ERR_SIZE_OUT_LEFTRANGE\n");
        break;

    case 16:
        printf("ERR_SIZE_OUT_RIGHTRANGE\n");
        break;

    case 32:
        printf("ERR_DIVIDE_ON_ZERO\n");
        break;

    default:
        printf("STRANGE_ERROR\n");
        break;
    }

    printf("ADRESS OF STACK: %p\n", stack.data);
    
    printf("№  value  adress\n");

    printf("CANARY - %d - %p\n", *((int*) (stack.data) - 1), ((int*)(stack.data) - 1));

    for (int i = 0; i < stack.lengthStack; i++)
    {
        printf("%d - ", i);
        printf(TYPE_SPECIFIER, stack.data[i]);
        printf(" - %p\n", &(stack.data[i]));
    }

    printf("CANARY - %d - %p\n", *((int*) (stack.data) + stack.lengthStack), ((int*) (stack.data) + stack.lengthStack));
}

void jump (struct stack* stack, int i)
{
    stackCheck (stack);

    stack->cur = i;

    stackCheck (stack);
}

void jb (struct stack* stack, int i)
{
    stackCheck (stack);

    type a = pop (stack);
    type b = pop (stack);

    if (a - b > EPSILON)
    {
        jump(stack, i);
    }

    stackCheck (stack);
}

void jbe (struct stack* stack, int i)
{
    stackCheck (stack);

    type a = pop (stack);
    type b = pop (stack);

    if (a - b > EPSILON || abs(b - a) < EPSILON)
    {
        jump(stack, i);
    }
    
    stackCheck (stack);
}

void ja (struct stack* stack, int i)
{
    stackCheck (stack);

    type a = pop (stack);
    type b = pop (stack);

    if (b - a > EPSILON)
    {
        jump(stack, i);
    }
    
    stackCheck (stack);
}

void jae (struct stack* stack, int i)

{
    stackCheck (stack);

    type a = pop (stack);
    type b = pop (stack);

    if (b - a > EPSILON || abs(b - a) < EPSILON)
    {
        jump(stack, i);
    }
    
    stackCheck (stack);
}

void je (struct stack* stack, int i)
{
    stackCheck (stack);

    type a = pop (stack);
    type b = pop (stack);

    if (abs(b - a) < EPSILON)
    {
        jump(stack, i);
    }
    
    stackCheck (stack);
}

void jne (struct stack* stack, int i)
{
    stackCheck (stack);

    type a = pop (stack);
    type b = pop (stack);

    if (abs(b - a) > EPSILON)
    {
        jump(stack, i);
    }
    
    stackCheck (stack);
}

void call (struct cpu* cpu, int i)
{
    push (&cpu->functstack, cpu->stack.cur);
    jump (&cpu->stack, i - 1);
}

void ret (struct cpu* cpu)
{
    stackCheck (&cpu->stack);
    int a = (int) pop (&cpu->functstack);
    jump (&cpu->stack, a);
    stackCheck (&cpu->stack);
}

void in (struct cpu* cpu)
{
    type value = 0;
    stackCheck (&cpu->stack);
    scanf (TYPE_SPECIFIER, &value);
    push (&cpu->stack, value);
    stackCheck (&cpu->stack);
}

void sqr (struct cpu* cpu)
{
    stackCheck (&cpu->stack);
    type x = pop (&cpu->stack);
    if (x < 0)
    {
        printf ("ERROR: SQRT from number less 0");
        hlt (&cpu->stack);
    }
    x = sqrt(x);
    push (&cpu->stack, x);
    stackCheck (&cpu->stack);
}

void hlt (struct stack* stack)
{
    stackDtor(stack);

    exit(0);
}
