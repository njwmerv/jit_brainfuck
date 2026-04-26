#inlucde "stack.h"

struct Node {
	void* data;
	struct Node* next;
};

struct Stack {
	Node* base;
	Node* curr;
};

Stack* Stack_Create() {
	Stack* stack = malloc(sizeof(*stack));
	stack->base = NULL;
	stack->curr = NULL;
	return stack;
}

void* Stack_Peek(Stack* stack) {
	assert(stack != NULL);
	assert(stack->base != NULL);

	return stack->curr->data;
}

int Stack_Push(Stack* stack, void* data) {
	assert(stack != NULL);

	Node* n = malloc(sizeof(*n));
	if (n == NULL) return 0;
	n->data = data;
	n->next = stack->curr;

	if (stack->base == NULL) stack->base = n;

	stack->curr = n;

	return 1;
}

void* Stack_Pop(Stack* stack) {
	assert(stack != NULL);
	assert(stack->base != NULL);

	Node* top = stack->curr;
	void* ret_val = top->data;

	if (top == stack->base) stack->base = NULL;

	stack->curr = top->next;

	free(top);

	return ret_val;
}

