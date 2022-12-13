# smol
A small (smol) C Compiler

This is the beginnings of a small C compiler. It is not yet completely functional and still in work. The compiler takes a `.c` file input and will output NASM syntax x86 assembly code. The `-o`option will allow you to direct the output of the compiler. To link the file, simply use your favorite full compiler toolchain or a linker of your choice. I tpyically use `clang`. The goal of this program is not to be cross-platform, and it has thus not been thoroughly tested outside of linux/unix-like environments (AKA MS Windows).

At this moment, it is easier to describe what smol can do, rather than what it cannot. However, some limitations include:
- No type checking (all types are the same for now)
- No symbol table for functions (assembler will catch some issues. Sorry, assembler)
- No variables (or symbol table)

As of now, smol is able to compile integer return value main functions such as the following:

```c
void eight(){
    return 8;
}
int main(){
    return 3 * (2 + 1) + eight();
}
```
smol Currently Supports:
- +
- -
- unary -
- *
- /
- %
- ==
- !=
- zero parameter (nullary) function defs
- zero parameter (nullary) function calls

smol will also recognize a variety of syntactical and lexical errors (for portions of C it currently supports). This functionality is not yet complete.
