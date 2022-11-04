smol: *.c
	gcc *.c -o smol

smol.o: smol.c
	nasm -f elf64 smol.c

test: smol
	./test.sh

clean:
	rm -f smol.o smol tmp.* tmp*

clean_test:
	rm -f tmp* tmp.* *.o *.exe

