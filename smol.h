/*   smol is a small C compiler
 *   Copyright (C) 2022  D. Nigh Herndon
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdarg.h>
#include <string.h>
#include <stdbool.h>

/*
 *******************
 *    TOKENIZER    *
 * *****************
 */

// The different types of lexical elements, AKA Tokens
typedef enum{
    KEYWRD,
    IDNTFR,
    CONSTANTS,
    STRLIT,
    PUNCTR,
    HDRNM,
    PPNUM,
    COMMENT,
    END
}lexicalElement;



 // ***** KEYWORDS *****

typedef enum{
    AUTO, BREAK, CASE, CHAR, CONST, CONTINUE,
    DEFAULT, DO, DOUBLE, ELSE, ENUM, EXTERN,
    FLOAT, FOR, GOTO, IF, INLINE, INT, LONG,
    REGISTER, RESTRICT, RETURN, SHORT, SIGNED,
    SIZEOF, STATIC, STRUCT, SWITCH, TYPEDEF,
    UNION, UNSIGNED, VOID, VOLATILE, WHILE,
    _ALIGNA, _ALIGNOF, _ATOMIC, _BOOL, _COMPLEX,
    _GENERIC, _IMAGINARY, _NORETURN, _STATIC_ASSERT,
    _THREAD_LOCAL, NOTKWD
}keywordKind;


 // ***** CONSTANTS *****

// The different types of constants
typedef enum{
    INTEGERS,
    FLOATINGs,
    ENUMS,
    CHARS
}constantKind;
// The different types of integer constants
typedef enum{
    DIGIT,
    HEX,
    OCTAL
}integerKind;

 // ***** PUNCTUATORS *****
 typedef enum{
    ELIPSES, DEREFERENCE, INCREMENT, DECREMENT, LSHFT, RSHFT, LE, GE, EQUIV, NOTEQUIV,
    LOGAND, LOGOR, TMSEQUAL, DIVEQUAL, PERCEQUAL, PLUSEQUAL, MINUSEQUAL, LSHFTEQUAL, RSHFTEQUAL,
    BITANDEQUAL, BITXOREQUAL, BITOREQUAL, DBLPOUND,
    LBRACK, RBRACK, LPAR, RPAR, LBRACE, RBRACE, DOT, 
    BITAND, STAR, PLUS, MINUS, BITNOT, EXCLAMATION, 
    FWDSLSH, PERCENT, LT, GT, BITXOR, BITOR,
    QUESTION, COLON, SEMICOLON, EQUAL, COMMA, POUND,
    NOTPUNCT 
 }punctuatorKind;

// ***** The Token Data Structure *****

typedef struct TOKEN TOKEN;

struct TOKEN{
    TOKEN* next;                // Pointer to the next item in linked list
    TOKEN* last;                // Pointer to the last item in linked list
    lexicalElement lexElem;     // The type of token (lexical element)
    int length;                 // The length of the token
    char* location;             // Location of the token
    int tokenLineNumber;        // Token line number
    char* tokenLinePos;         // Current line initial position
    char* tokenContent;         // Contains the string of the token

// ***** KEYWORDS *****
    keywordKind keywdType;

 // ***** CONSTANTS *****
    constantKind constType;
    integerKind integerType;
    unsigned long constantVal;

// ***** PUNCTUATORS *****
    punctuatorKind punctType;
};

// ***** Tokenizer Function Prototypes *****
TOKEN* tokenizer(char* scanner, long int file_size);
void print_token(TOKEN* token);
// Does this string match that string
// Returns true if yes
static bool match(char* this, char* that){
    return strncmp(this, that, strlen(that)) == 0;
};

/*
 **********************
 *    SYMBOL TABLE    *
 * ********************
 */
typedef struct SYMBOL SYMBOL;
typedef struct symbolTable symbolTable;
typedef struct symTblEntry symTblEntry;
typedef enum{
    SYM_NULL,
    SYM_FUNC,
    SYM_VAR
}symbolKind;

struct SYMBOL{
    symbolKind kind;        // Kind of symbol
    char* name;             // String name of symbol
    int offset;             // Local stack offset
    symbolTable* scope;     // For funcs, enter scope
};

struct symTblEntry{
    char* key;
    SYMBOL* value;
};

struct symbolTable{
    symTblEntry* entry;
    symbolTable* previous;
    int size;                   // Table size
    int count;                  // Number of times in table
    int scopeDepth;
    int totalLocalVars;
};
// This hashing function computes the 64 bit FNV-1a hash of a string
unsigned long int hashingFunction(char* str, int length);

// This function creates a symbol table of a specified size and
// returns a pointer to the the table
symbolTable* createSymbolTable(int size);

// This inserts a value into the given table using the key
symTblEntry* symTblInsert(symbolTable* table, char* key, SYMBOL* value);

// This retrieves the pointer to an entry in the given table
// using the key
symTblEntry* symTblGet(symbolTable* table, char* key);

// This deletes the entry in the table with the key
void symTblDelete(symbolTable* table, char* key);

//
void enterScope(symbolTable** currentScope, symbolTable** nextScope);
void exitScope(symbolTable** currentScope);
symTblEntry* searchScope(symbolTable* currentScope, char* symbolName);
void symbolTableDump(symbolTable* currentScope);
/*
 ****************
 *    PARSER    *
 * **************
 */
typedef struct NODE NODE;

typedef enum{
    NODE_NULL,
    NODE_ADD,       // +
    NODE_SUB,       // -
    NODE_NEGATE,    // - (unary)
    NODE_MUL,       // *
    NODE_DIV,       // /
    NODE_MOD,       // %
    NODE_INT,       // Integer
    NODE_EQUIV,     // ==
    NODE_NEQUIV,    // !=
    NODE_LOGAND,    // &&
    NODE_FUNCALL,   // Function Call
    NODE_FUNCDEC,   // Function Declaration
    NODE_STATEMENT,
    NODE_IDENTIFIER,
    NODE_VAR,
    NODE_ASSIGN,
    NODE_DECLARATION,
    NODE_RETURN,
    NODE_END
}nodeKind;

struct NODE{
    nodeKind kind;
    NODE* left;
    NODE* right;
    int constantVal;
    char* symbolName;
};

NODE* parse(TOKEN* token, symbolTable* table);

/*
 ***********************
 *    CODE GENERATOR   *
 * *********************
 */
int code_generator(NODE* node, FILE* outputFile, symbolTable* table);

/*
 ************************
 *    ERROR HANDLING    *
 * **********************
 */
char* tokenizer_error(TOKEN* token);
void unexpected_token_error(char* scanned);
void expected_error(TOKEN** token);
void errorAt(TOKEN** token, const char* format, ...);


