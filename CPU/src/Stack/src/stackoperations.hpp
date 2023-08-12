#ifndef STACKOPERATIONS_HPP
#define STACKOPERATIONS_HPP

typedef float type;

#include "stack.hpp"

#define EPSILON 1e-5

int cmpType (type a, type b);

void push(struct stack* stack, type value);

void pushr(struct stack* stack, int indeficator);

void pushv (struct stack* stack, int varIdentity);

type pop (struct stack* stack);

type popr (struct stack* stack, int indeficator);

void popv (struct stack* stack, int varIdentity);

void add (struct stack* stack);

void sub (struct stack* stack);

void mul (struct stack* stack);

void div (struct stack* stack);

void out (struct stack* stack);

void print (struct stack stack);

void dump (struct stack stack);

void jump (struct stack* stack, int i);

void jb (struct stack* stack, int i);

void jbe (struct stack* stack, int i);

void ja (struct stack* stack, int i);

void jae (struct stack* stack, int i);

void je (struct stack* stack, int i);

void jne (struct stack* stack, int i);

void call (struct cpu* mycpu, int i);

void in (struct cpu* mycpu);

void ret (struct cpu* cpu);

void sqr (struct cpu* cpu);

void hlt (struct stack* stack);

#endif
