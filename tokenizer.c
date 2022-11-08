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
#include "smol.h"

TOKEN* create_token(lexicalElement type, char* beginning, char* end){
    // This statement allocates zero initialized (calloc vs. malloc)
    // memory. It allocates 1 block the size of the TOKEN data structure.
    // token is then a pointer to this new spot in memory.
    TOKEN* token = calloc(1, sizeof(TOKEN));
    // The lexicalElement has been given to us by the function callee
    token->type = type;
    // The location of the token is given by where the token starts
    token->location = beginning;
    // The length of the token is simply the ending subtracted from the beginning
    token->length = end - beginning;
    // We then return the ppinter to this spot in memory
    return token;
};

// Does this string match that string
// Returns true if yes
static bool match(char* this, char* that){
    return strncmp(this, that, strlen(that)) == 0;
};

// 
void print_token(TOKEN* token){
    char* tmp = token->location;
    char* present_token = calloc(1, token->length);
    for(int i = 0; i < token->length; i++){
        present_token[i] += tmp[i];
    }
    printf("%s", present_token);
    free(present_token);
}

/*
 *******************************************************************************
 *                            TOKENIZER STRUCTURE                              *
 *******************************************************************************
 * The overall structure of the tokenizer is as follows:                       *
 *                                                                             *
 * 1.) The tokenizer takes in a pointer to a series of characters that         *
 *     is the entire source code.                                              *
 * 2.) The tokenizer looks at each character and *intelligently* decides       *
 *     what kind of token it is.                                               *
 * 3.) If it is white space, a comment, etc., it will skip over it             *
 * *****************************************************************************
 */

TOKEN* tokenizer (char* scanner){
    // Create an empty TOKEN structure called head.
    // This points to the beginning of our linked list
    TOKEN head = {};
    // Create a pointer to the current token and initialize it
    // to the memory location of head.
    TOKEN* current = &head;

    while (*scanner){
        // WHITESPACE
        // If the item is a space, skip it and move on
        // This is placed first since it is likely the most common
        // occurrence in a source code file
        if (isspace(*scanner)){
            scanner++;
            continue;
        }
        // COMMENTS - INLINE
        // If scanner points to "//"
        //if (match(scanner,"//")){
        //    // Move the scanner down 2 spots to move over '//'
        //    printf("%c\n", *scanner);
        //    scanner += 2;
        //    // Now just increment the scanner until end of line is detected
        //    while (!match(scanner,"\n")){
        //        printf("%c\n", *scanner);
        //        getchar();
        //        scanner++;
        //    }
        //    continue;
        //}
        /*
         ******************
         *    COMMENTS    *
         * ****************
         */
        // Block Comments
        // If scanner points to "/*" it is the beginning of a block comment
        if (match(scanner, "/*")){
            scanner += 2;
            char* p = scanner;
            // Increment the scanner the block comment is closed
            while(!match(scanner, "*/")){
                scanner++;
                if (!*scanner){
                    
                }
            }
            current = current->next = create_token(COMMENT, p, scanner);
            continue;
        }
        /*
         *******************
         *    CONSTANTS    *
         * *****************
         */
        // Hex Numbers
        if ((match(scanner, "0x") && isxdigit(scanner[2])) || (match(scanner, "0X") && isxdigit(scanner[2]))){
            char* p = scanner;
            scanner += 2;
            while(isxdigit(*scanner)) scanner++;
            current = current->next = create_token(CONSTANTS, p, scanner);
            current->constType = INTEGERS;
            current->integerType = HEX;
            continue;
        }
        // "Normal" numbers
        if (isdigit(*scanner)){
            char* p = scanner;
            while(isdigit(*scanner)) scanner ++;
            current = current->next = create_token(CONSTANTS, p, scanner);
            current->constType = INTEGERS;
            current->integerType = DIGIT;
            continue;
        }
        /*
         **********************************
         *    KEYWORDS AND IDENTIFIERS    *
         * ********************************
         */
        // When we start scanning we do not know if it is a keyword
        // or an identifier
        if((*scanner >= 'a' && *scanner <= 'z')
        || (*scanner >= 'A' && *scanner <= 'Z')
        || (*scanner == '_')){
            char* p = scanner;
            while(isalnum(*scanner) || (*scanner == '_')) scanner++;
            
            current = current->next = create_token(IDNTFR, p, scanner);
            continue;
        }
        unexpected_token_error(scanner);
        // KEYWORDS
        scanner++;
    }
    current = current->next = create_token(END, scanner, scanner);
    return head.next;
};