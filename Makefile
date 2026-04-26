CC = gcc
CFLAGS = -Wall -Wextra
NAME := bf_jit

all: $(NAME)

$(NAME): main.c stack.o
	$(CC) main.c -o $(NAME) stack.o $(CFLAGS)

stack.o:
	$(CC) stack.c -c $(CFLAGS)

.PHONY: clean
clean:
	rm -f stack.o bf_jit