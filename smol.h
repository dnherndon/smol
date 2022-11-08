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
void print_token(TOKEN* token);

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

NODE* parse(TOKEN* token);
