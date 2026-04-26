CC = gcc
NAME := bf_jit

all: $(NAME)

$(NAME): main.c stack.o
	$(CC) main.c -o $(NAME) stack.o

stack.o:
	$(CC) stack.c -c

.PHONY: clean
clean:
	rm -f stack.o bf_jit