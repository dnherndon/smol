CFLAGS=
SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)
CC=clang
TEST_SRCS=$(wildcard test/*.c)
TESTS=$(TEST_SRCS:.c=.exe)

smol: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^
	rm -f *.tmp

test: $(TESTS)
	./driver.sh

test/%.exe: smol
	./smol test/$*.c -o test/$*.s
	nasm -f elf64 test/$*.s -o test/$*.o
	$(CC) test/$*.o -o test/$*.exe
	rm test/*.s test/*.o

clean:
	rm -f smol *.out *.o *.asm *.tmp *.s
	rm -f ./test/*.s ./test/test ./test/*.exe

