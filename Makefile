CFLAGS=
SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)
CC=clang

smol: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^
	rm -f *.tmp

assemble: out.asm
	nasm -f elf64 out.asm -o out

all: ./test/test.c
	$(CC) *.c -o smol
	./smol ./test/test.c > out.asm
	nasm -f elf64 out.asm -o out.o
	$(CC) out.o -o test.out

test: smol
	./test.sh

clean:
	rm -f smol *.out *.o *.asm *.tmp

clean_test:
	rm -f tmp* tmp.* *.o *.exe

