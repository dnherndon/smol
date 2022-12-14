## smol
A small (smol) C Compiler

This is the beginnings of a small C compiler. It is not yet completely functional and still in work. The compiler takes a `.c` file input and will output NASM syntax x86 assembly code. The `-o` option will allow you to direct the output of the compiler. To link the file, simply use your favorite full compiler toolchain or a linker of your choice. I typically use `clang`. The goal of this program is not to be cross-platform, and it has thus not been thoroughly tested outside of linux/unix-like environments (AKA MS Windows).

At this moment, it is easier to describe what smol can do, rather than what it cannot. However, some limitations include:
- No type checking (all types are a QWORD)
- Variables are only one type

As of now, smol is able to compile integer return value (recall everything is a QWORD, types are not enforced yet) functions such as the following:

```c
int eight(){
    int x;
    x = 8;
    return x;
}
int main(){
    int x;
    x = 0;
    int y;
    y = 3;
    x = y;
    return x * (2 + y) + eight();
}
```
smol Currently Supports:
- addition (+)
- subtraction (-)
- unary - (-5)
- multiplication (*)
- division (/)
- modulo (%)
- equivalent (==)
- not equivalent (!=)
- zero parameter (nullary) function defs
- zero parameter (nullary) function calls
- integer type (actually long int/QWORD) variables
- up to 16 symbols per scope

smol will also recognize a variety of syntactical and lexical errors (for portions of C it currently supports). This functionality is not yet complete.
