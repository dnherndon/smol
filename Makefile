smol: *.c
	clang *.c -o smol

assemble: out.asm
	nasm -f elf64 out.asm -o out

all: ./test/test.c
	clang *.c -o smol
	./smol ./test/test.c > out.asm
	nasm -f elf64 out.asm -o out.o
	clang out.o -o test.out

test: smol
	./test.sh

clean:
	rm -f smol
	rm -f *.out
	rm -f *.o
	rm -f *.asm

clean_test:
	rm -f tmp* tmp.* *.o *.exe

