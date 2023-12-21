# smol
A small (smol) C Compiler.

This is the beginnings of a small C compiler. It is not yet completely functional and still in work. The compiler takes a `.c` file input and will output NASM syntax x86 assembly code. The `-o` option will allow you to direct the output of the compiler. To link the file, simply use your favorite full compiler toolchain or a linker of your choice. I typically use `clang`. The goal of this program is not to be cross-platform, and it has thus not been thoroughly tested outside of linux/unix-like environments (AKA MS Windows). This project was meant to be a educational tool by which the author could come to understand how compilers work and are practically implemented.

The author has a great interest in compiler development and can often be founded browsing Matt Godbolt's [Compiler Explorer](https://godbolt.org/) for his current engineering job and for fun.

## Compiler Structure
The compiler consists of three main sections:
1. Tokenizer (or lexical analyzer)
2. Parser
3. Code generator

### Tokenizer
After file I/O is taken care of, the tokenizer will break the piece of code down into tokens also referred to as lexical elements. These lexical elements are laid out in the C11 Standard. These elements are:
* Keywords
* Identifiers
* Universal character names
* Constants
* String literals
* Punctuators
* Header names
* Preprocessing numbers
* Comments

The main job of the tokenizer is to identify whether or not a string of characters is a valid lexical element, and if it is, what type of lexical element it is. A number of other functions are assumed by the tokenizer as well. This includes items such as recording the location of the token for error generating, saving the content of the token for later use, or recording the value of a constant. The tokenizer will also identify what type of punctuator the punctuator is. The `TOKEN` data type is used to store all this information. The tokens are saved in a singly linked list that includes `next` and `last` pointers.
The tokenizer is capable of recognizing erroneous lexical elements and telling where they are located.

### Parser
Once the file has been tokenized, the head of the singly linked list is passed to the parser. The parser's job is to perform syntactical analysis on the list of tokens. The syntax of the langauge is laid out in the C11 standard as previously referenced. While more efficient methods exist and the time complexity of the implementation is *O(bad)* at best (that was a joke), the parser that has been implemented is a recursive descent parser. This parser was chosen for its relative simplicity.

A recursive descent parser is a top down parser. This means that it will begin at the highest level of the parse tree and work its way down. For example, in C, and thus in this parser, we begin at the translation unit which is the highest level of the syntax tree and work downard.

The parser's job is ultimately to create an Abstract Syntax Tree (AST) that is representative of the structure of the program that is being compiled. The vast majority of compiler errors are syntax errors which are also handled by the parser. When a syntactical element does not match the syntax definition in teh C11 standard, an error is thrown giving the line, the error, and potentially a suggestion about what was missing. The parser also handles other functionality such as symbol table creation and scope handling. The output of the parser is the head of the abstract syntax tree structure.

#### Symbol Table
As a brief comment, the symbol table is essentially implemented as a hash table. It handles scopes by creating unique symbol tables for each scope and entering and exiting them via rules of scope nesting in C11. The "lowest" scope always has visibility to all scopes above it and when searching to determine if a symbol is already defined, it can iterate to the highest level. The inverse is true of top level scopes.

### Code Generator
The final element of the compiler is the code generator. The code generator takes the head pointer of the abstract syntax tree and traverses the tree to generate NASM syntax x86 assembly. The code generator traverses the tree and generates code depending on the type of AST node it encounters. It begins at the highest syntactical element (the translation unit) and then traverses the tree pre-order or post-order depending on what section of the tree it is in.

The code generator's output is a text file containing assembly code that can be assembled and linked with a variety tools.
## Testing
In the testing folder, a number of `.c` files are included that the compiler can successfully compile and execute correctly and according to the C11 standard. These unit tests are driven by a script in the main folder that can run any time a change is made to the compiler. 
At the time of writing there are around 20 basic C programs that are included. At the time of writing all included unit tests pass.

## Optimzation
While the author finds compiler optimization fascinating and of great interest, for practical purposes no optimization has been included in this compiler. Using the `-O3` flag may as well write an apology letter to the stack (another joke).

## Functionality
At this moment, it is easier to describe what smol can do, rather than what it cannot. However, some limitations include:
- No rigorous type checking (all types are a QWORD)
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
