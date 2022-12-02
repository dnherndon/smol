CFLAGS=
SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)
CC=clang
TEST_SRCS=$(wildcard test/*.c)
TESTS=$(TEST_SRCS:.c=)

smol: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^
	rm -f *.tmp

all: ./test/test
	./smol ./test/test.c > ./test/test.s
	nasm -f ELF64 ./test/test.s -o test.o
	$(CC) ./test.o

clean:
	rm -f smol *.out *.o *.asm *.tmp *.s
	rm -f ./test/*.s ./test/test

