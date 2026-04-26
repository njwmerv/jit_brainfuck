#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/mman.h>
#include "stack.h"

#define BUF_SIZE sysconf(_SC_PAGESIZE) * 10
#define TAPE_SIZE 30000

bool is_brainfuck(char c) {
	return c == '>' || c == '<' || c == '+' || c == '-' || c == '[' || c == ']' || c == ',' || c == '.';
}

char* read_file(FILE* file) {
	fseek(file, 0, SEEK_END);
	long len = ftell(file);
	fseek(file, 0, SEEK_SET);

	char* content = malloc(len + 1);
	if (content) {
		fread(content, 1, len, file);
		content[len] = '\0';
	}

	char* read = content;
	char* write = content;
	while (*read != '\0') {
		if (is_brainfuck(*read)) {
			*write = *read;
			write++;
		}
		read++;
	}
	*write = '\0';

	size_t new_len = (write - content) + 1;
	char* new_cont = realloc(content, new_len);

	if (new_cont) content = new_cont;

	return content;
}

int main(int argc, char* argv[]) {
	if (argc != 2) {
		printf("ERROR: Expected only argument %s [filename]\n", argv[0]);
		return -1;
	}

	// READ INPUT	
	FILE* file = fopen(argv[1], "r");

	if (file == NULL) {
		printf("ERROR: Could not open file: %s\n", argv[1]);
		return -1;
	}

	char* content = read_file(file);

	//////////
	// PARSE INPUT

	int brack = 0;
	char* c = content;
	while (*c) {
		if (c == '[') brack++;
		else if (c == ']' && brack <= 0) {
			printf("ERROR: Unmatched \']\'\n");
			return -1;
		}
		else if (c == ']') brack--;
		c++;
	}
	if (brack != 0) {
		printf("ERROR: Unmatched \'[\'\n");
		return -1;
	}

	void* buffer = mmap(NULL, BUF_SIZE, PROT_EXEC | PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
	uint8_t* jit_memory = (uint8_t*) buffer;
	int jit_idx = 0;

	void* tape = calloc(TAPE_SIZE, 1);
	void* head = tape + TAPE_SIZE / 2;

	Stack* stack = Stack_Create();
	c = content;
	// push r12
	jit_memory[jit_idx++] = 0x41;
	jit_memory[jit_idx++] = 0x54;
	// mov r12, rdi
	jit_memory[jit_idx++] = 0x49;
	jit_memory[jit_idx++] = 0x89;
	jit_memory[jit_idx++] = 0xFC;
	while (*c) {
		switch (*c) {
			case '>':
				jit_memory[jit_idx++] = 0x49;
				jit_memory[jit_idx++] = 0xFF;
				jit_memory[jit_idx++] = 0xC4;
				break;
			case '<':
				jit_memory[jit_idx++] = 0x49;
				jit_memory[jit_idx++] = 0xFF;
				jit_memory[jit_idx++] = 0xCC;
				break;
			case '+':
  				jit_memory[jit_idx++] = 0x41;
				jit_memory[jit_idx++] = 0xFE;
				jit_memory[jit_idx++] = 0x04;
				jit_memory[jit_idx++] = 0x24;
				break;
			case '-':
				jit_memory[jit_idx++] = 0x41;
				jit_memory[jit_idx++] = 0xFE;
				jit_memory[jit_idx++] = 0x0C;
				jit_memory[jit_idx++] = 0x24;
				break;
			case '[':
				jit_memory[jit_idx++] = 0x41;
    				jit_memory[jit_idx++] = 0x80;
    				jit_memory[jit_idx++] = 0x3C;
    				jit_memory[jit_idx++] = 0x24;
    				jit_memory[jit_idx++] = 0x00;
    				jit_memory[jit_idx++] = 0x0F;
    				jit_memory[jit_idx++] = 0x84;

				int32_t* addr = malloc(sizeof(int32_t));
				*addr = jit_idx;
				Stack_Push(stack, addr);

    				jit_memory[jit_idx++] = 0x00;
    				jit_memory[jit_idx++] = 0x00;
				jit_memory[jit_idx++] = 0x00;
				jit_memory[jit_idx++] = 0x00;
				break;
			case ']':
				jit_memory[jit_idx++] = 0x41;
			    	jit_memory[jit_idx++] = 0x80;
    				jit_memory[jit_idx++] = 0x3C;
    				jit_memory[jit_idx++] = 0x24;
    				jit_memory[jit_idx++] = 0x00;
				jit_memory[jit_idx++] = 0x0F;
				jit_memory[jit_idx++] = 0x85;

				int jump_end = jit_idx + 4;
				int32_t* block_start = Stack_Pop(stack);

				int32_t backward_diff = (*block_start - 7) - jump_end;
				*(int32_t*)(&jit_memory[jit_idx]) = backward_diff;
				jit_idx += 4;

				int32_t forward_diff = jit_idx - (*block_start + 4);
				*(int32_t*)(&jit_memory[*block_start]) = forward_diff;

				free(block_start);
				break;
			case '.':
				jit_memory[jit_idx++] = 0xB8;
                		jit_memory[jit_idx++] = 0x01;
                		jit_memory[jit_idx++] = 0x00;
                		jit_memory[jit_idx++] = 0x00;
                		jit_memory[jit_idx++] = 0x00;
                		jit_memory[jit_idx++] = 0xBF;
                		jit_memory[jit_idx++] = 0x01;
                		jit_memory[jit_idx++] = 0x00;
                		jit_memory[jit_idx++] = 0x00;
                		jit_memory[jit_idx++] = 0x00;
                		jit_memory[jit_idx++] = 0x4C;
                		jit_memory[jit_idx++] = 0x89;
                		jit_memory[jit_idx++] = 0xE6;
                		jit_memory[jit_idx++] = 0xBA;
                		jit_memory[jit_idx++] = 0x01;
                		jit_memory[jit_idx++] = 0x00;
                		jit_memory[jit_idx++] = 0x00;
                		jit_memory[jit_idx++] = 0x00;
                		jit_memory[jit_idx++] = 0x0F;
                		jit_memory[jit_idx++] = 0x05;
                		break;
        		case ',':
                		jit_memory[jit_idx++] = 0xB8;
                		jit_memory[jit_idx++] = 0x00;
                		jit_memory[jit_idx++] = 0x00;
                		jit_memory[jit_idx++] = 0x00;
                		jit_memory[jit_idx++] = 0x00;
                		jit_memory[jit_idx++] = 0xBF;
                		jit_memory[jit_idx++] = 0x00;
                		jit_memory[jit_idx++] = 0x00;
                		jit_memory[jit_idx++] = 0x00;
                		jit_memory[jit_idx++] = 0x00;
                		jit_memory[jit_idx++] = 0x4C;
                		jit_memory[jit_idx++] = 0x89;
                		jit_memory[jit_idx++] = 0xE6;
                		jit_memory[jit_idx++] = 0xBA;
                		jit_memory[jit_idx++] = 0x01;
                		jit_memory[jit_idx++] = 0x00;
                		jit_memory[jit_idx++] = 0x00;
                		jit_memory[jit_idx++] = 0x00;
                		jit_memory[jit_idx++] = 0x0F;
                		jit_memory[jit_idx++] = 0x05;
                		break;
		}
		c++;
	}
	// pop r12
	jit_memory[jit_idx++] = 0x41;
	jit_memory[jit_idx++] = 0x5C;
	// ret
	jit_memory[jit_idx++] = 0xC3;

	//////////
	// EXECUTION
	printf("Beginning Execution of: %s...", argv[1]);
	void (*execute_jit)(void*) = (void (*)(void*)) buffer;

	execute_jit(head);

	printf("Execution Complete!\n");
	
	//////////
	// CLEANUP
	
	munmap(buffer, BUF_SIZE);

	free(content);

	free(tape);

	fclose(file);
	return 0;
}

