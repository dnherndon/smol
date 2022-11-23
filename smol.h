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
    lexicalElement lexElem;     // The type of token (lexical element)
    int length;                 // The length of the token
    char* location;             // Location of the token


// ***** KEYWORDS *****
    keywordKind keywdType;

 // ***** CONSTANTS *****
    constantKind constType;
    integerKind integerType;

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
 ************************
 *    ERROR HANDLING    *
 * **********************
 */
char* tokenizer_error(TOKEN* token);
void unexpected_token_error(char* scanned);
void expected_error(TOKEN** token);

/*
 ****************
 *    PARSER    *
 * **************
 */
typedef struct NODE NODE;

typedef enum{
    TRANSLATION_UNIT,
    EXTERNAL_DECLARATION,
    FUNCTION_DEFINITION,
    DECLARATION_SPECIFIER,
    STORAGE_CLASS_SPECIFIER,
    TYPE_SPECIFIER, 
    STRUCT_OR_UNION_SPECIFIER,
    STRUCT_OR_UNION,
    STRUCT_DECLARATION,
    SPECIFIER_QUALIFIER,
    STRUCT_DECLARATOR_LIST,
    STRUCT_DECLARATOR,
    DECLARATOR,
    POINTER,
    TYPE_QUALIFIER,
    DIRECT_DECLARATOR,
    CONSTANT_EXPRESSION,
    CONDITIONAL_EXPRESSION,
    LOGICAL_OR_EXPRESSION,
    LOGICAL_AND_EXPRESSION,
    INCLUSIVE_OR_EXPRESSION,
    EXCLUSIVE_OR_EXPRESSION,
    AND_EXPRESSION,
    EQUALITY_EXPRESSION,
    RELATIONAL_EXPRESSION,
    SHIFT_EXPRESSION,
    ADDITIVE_EXPRESSION,
    MULTIPLICATIVE_EXPRESSION,
    CAST_EXPRESSION,
    UNARY_EXPRESSION,
    POSTFIX_EXPRESSION,
    PRIMARY_EXPRESSION,
    CONSTANT, 
    EXPRESSION,
    ASSIGNMENT_EXPRESSION,
    ASSIGNMENT_OPERATOR, 
    UNARY_OPERATOR,
    TYPE_NAME,
    PARAMETER_TYPE_LIST,
    PARAMETER_LIST, 
    PARAMETER_DECLARATION,
    ABSTRACT_DECLARATOR, 
    DIRECT_ABSTRACT_DECLARATOR,
    ENUM_SPECIFIER, 
    ENUMERATOR_LIST,
    ENUMERATOR,
    TYPEDEF_NAME,
    DECLARATION,
    INIT_DECLARATOR,
    INITIALIZER,
    INITIALIZER_LIST,
    COMPOUND_STATEMENT,
    STATEMENT,
    LABELED_STATEMENT,
    EXPRESSION_STATEMENT,
    SELECTION_STATEMENT,
    ITERATION_STATEMENT,
    JUMP_STATEMENT
}nodeKind;

struct NODE{
    NODE* next;
    nodeKind kind;
    int64_t constantVal;
};

NODE* parse(TOKEN* token);
