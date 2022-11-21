smol: *.c
	gcc *.c -o smol

assemble: out.asm
	nasm -f elf64 out.asm -o out

all: ./test/test.c
	./smol ./test/test.c > out.asm
	nasm -f elf64 out.asm -o out.o
	gcc -static out.o

test: smol
	./test.sh

clean:
	rm -f smol

clean_test:
	rm -f tmp* tmp.* *.o *.exe

