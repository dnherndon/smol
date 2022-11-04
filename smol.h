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

typedef enum{
    DIGIT,
    HEX,
    OCTAL
}integerKind;

typedef enum{
    INTEGERS
}constantKind;

typedef struct TOKEN TOKEN;

struct TOKEN{
    TOKEN* next;                // Pointer to the next item in linked list
    lexicalElement type;        // The type of token (lexical element)
    int length;                 // The length of the token
    char* location;             // Location of the token

    constantKind constType;
    integerKind integerType;
};

TOKEN* tokenizer(char* scanner);

/*
 ************************
 *    ERROR HANDLING    *
 * **********************
 */
char* tokenizer_error(TOKEN* token);
void unexpected_token_error(char* scanned);

/*
 ****************
 *    PARSER    *
 * **************
 */
typedef struct NODE NODE;

typedef enum{
    ND_INTEGERS,

}nodeKind;

struct NODE{
    NODE* next;
    nodeKind kind;
    int64_t constantVal;
};
