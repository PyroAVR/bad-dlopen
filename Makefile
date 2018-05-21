%.o: %.c
	gcc -O0 -ggdb -c $< -o $@

all: module.o
	gcc -O0 -ggdb main.c -o main
