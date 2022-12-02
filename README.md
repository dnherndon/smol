# smol
A small (smol) C Compiler

This is the beginnings of a small C compiler. It is not yet completely functional and still in work. The compiler takes a .c file input and will output NASM syntax x86 assembly code. As of now, -o is not supported and the program output must be redirected to a file in order to assemble. To link the file, simply use your favorite full compiler toolchain or a linker of your choice. I tpyically use CLANG. The goal of this program is not to be cross-platform, and it has thus not been tested outside of linux/unix-like environments (AKA MS Windows).

At this moment, it is easier to describe what smol can do, rather than what it cannot.

As of now, smol is able to compile integer return value main functions such as the following:

`int main(){
    return 3 * (2 + 1);
}`

smol will also recognize a variety of syntactical and lexical errors (for portions of C it currently supports). This functionality is not yet complete, and the error detection leaves something to be desired.
