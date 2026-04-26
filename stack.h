#ifndef STACK_H
#define STACK_H

#include <stdlib.h>
#include <assert.h>

typedef struct Node Node;

typedef struct Stack Stack;

Stack* Stack_Create();
int    Stack_Push(Stack* stack, void* data);
void*  Stack_Pop(Stack* stack);
void*  Stack_Peek(Stack* stack);

#endif

