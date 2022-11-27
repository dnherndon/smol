smol: *.c
	$(CC) *.c -o smol

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
	rm -f smol
	rm -f *.out
	rm -f *.o
	rm -f *.asm

clean_test:
	rm -f tmp* tmp.* *.o *.exe

