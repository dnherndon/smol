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

// Does this string match that string
// Returns true if yes
static bool match(char* this, char* that){
    return strncmp(this, that, strlen(that)) == 0;
};

TOKEN* create_token(lexicalElement type, char* beginning, char* end, int lineNumber){
    // This statement allocates zero initialized (calloc vs. malloc)
    // memory. It allocates 1 block the size of the TOKEN data structure.
    // token is then a pointer to this new spot in memory.
    TOKEN* token = calloc(1, sizeof(TOKEN));
    // The lexicalElement has been given to us by the function callee
    token->lexElem = type;
    // The location of the token is given by where the token starts
    token->location = beginning;
    // The length of the token is simply the ending subtracted from the beginning
    token->length = end - beginning;
    // Token line number
    token->tokenLineNumber = lineNumber;
    // We then return the ppinter to this spot in memory
    return token;
};

// Prints the current token
void print_token(TOKEN* token){
    char* tmp = token->location;
    char* present_token = calloc(1, token->length);
    for(int i = 0; i < token->length; i++){
        present_token[i] += tmp[i];
    }
    printf("%s", present_token);
    free(present_token);
}

// Identifier or Keyword
// This function checks to see if the character string
// is a keyword or just an identifier. It does this by
// comparing to every possible keyword in the C language.
// Not exactly efficient, but there is really no other way to
// do this.
keywordKind is_keyword(char* start, char* end){
    char* keywords [] = {"auto", "break", "case", "char", "const", "continue",
                         "default", "do", "double", "else", "enum", "extern",
                         "float", "for", "goto", "if", "inline", "int", "long",
                         "register", "restrict", "return", "short", "signed",
                         "sizeof", "static", "struct", "switch", "typedef",
                         "union", "unsigned", "void", "volatile", "while",
                         "_Aligna", "_Alignof", "_Atomic", "_Bool", "_Complex",
                         "_Generic", "_Imaginary", "_Noreturn", "_Static_assert",
                         "_Thread_local"};
        
        keywordKind currentKeyword = 0;

        for(int i = 0; i < (sizeof(keywords)/sizeof(keywords[i])); i++){
            // If the string we are checking is not the length,
            // then it cannot be the keyword
            if (strlen(keywords[i]) != (end - start)){
                continue;
            }
            else if (strncmp(start, keywords[i], strlen(keywords[i])) == 0) {
                currentKeyword += i;
                return currentKeyword;
            }
        }
        currentKeyword = NOTKWD;
        return currentKeyword;
    }
// Punctuators
punctuatorKind is_punctuator(char* start, char** startloc){
    char* punctuators [] = {"...", "->", "++", "--", "<<", ">>", "<=", ">=", "==", "!=",
                            "&&", "||", "*=", "/=", "%=", "+=", "-=", "<<=", ">>=", 
                            "&=", "^=", "|=", "##",
                           "[", "]", "(", ")", "{", "}", ".",  
                           "&", "*", "+", "-", "~", "!", 
                           "/", "%", "<", ">",  "^", "|",  
                           "?", ":", ";", "=", ",", "#"};
        
        punctuatorKind currentPunctuator = 0;

        for(int i = 0; i < (sizeof(punctuators)/sizeof(punctuators[i])); i++){
            if (strncmp(start, punctuators[i], strlen(punctuators[i])) == 0){
                *startloc += (strlen(punctuators[i]));
                currentPunctuator += i;
                return currentPunctuator;
            }
        }
        currentPunctuator = NOTPUNCT;
        return currentPunctuator;
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

TOKEN* tokenizer (char* scanner, long int file_size){
    // Create an empty TOKEN structure called head.
    // This points to the beginning of our linked list
    TOKEN head = {};
    // Create a pointer to the current token and initialize it
    // to the memory location of head.
    TOKEN* current = &head;
    char *end_address = scanner + file_size;
    // Line number counter
    int lineNumber = 1;
    while (scanner < end_address){
        /*
         ********************
         *    WHITESPACE    *
         * ******************
         */
        // If the item is a space, skip it and move on
        // This is placed first since it is likely the most common
        // occurrence in a source code file
        if(*scanner == ' '){
            scanner++;
            continue;
        }
        else if (*scanner == '\t'){
            scanner++;
            continue;
        }
        else if (*scanner == '\n'){
            scanner++;
            lineNumber++;
            continue;
        }
        else if (*scanner == '\v'){
            scanner++;
            continue;
        }
        else if (*scanner == '\f'){
            scanner++;
            continue;
        }
        else if (*scanner == '\r'){
            scanner++;
            continue;
        }
        /*
         ******************
         *    COMMENTS    *
         * ****************
         */
        // COMMENTS - INLINE
        // If scanner points to "//"
        if (match(scanner,"//")){
            // Move the scanner down 2 spots to move over '//'
            scanner += 2;
            //char* p = scanner;
            // Now just increment the scanner until end of line is detected
            while (!match(scanner,"\n")){
                scanner++;
            }
            //current = current->next = create_token(COMMENT, p, scanner);
            continue;
        }
        // COMMENTS - BLOCK
        // If scanner points to "/*" it is the beginning of a block comment
        if (match(scanner, "/*")){
            scanner += 2;
            //char* p = scanner;
            // Increment the scanner the block comment is closed
            while(!match(scanner, "*/")){
                scanner++;
                if (scanner == end_address){
                    printf("Unterminated block comment\n");
                    exit(1);
                    break;
                }
            }
            //current = current->next = create_token(COMMENT, p, scanner);
            // Move beyond the "*/"
            scanner += 2;
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
            current = current->next = create_token(CONSTANTS, p, scanner, lineNumber);
            current->constType = INTEGERS;
            current->integerType = HEX;
            continue;
        }
        // "Normal" numbers
        if (isdigit(*scanner)){
            char* p = scanner;
            while(isdigit(*scanner)) scanner ++;
            current = current->next = create_token(CONSTANTS, p, scanner, lineNumber);
            current->constType = INTEGERS;
            current->integerType = DIGIT;
            current->constantVal = strtoul(p,&p,10);
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
            keywordKind type = 0;
            type = is_keyword(p, scanner);
            if (type == NOTKWD){
                current = current->next = create_token(IDNTFR, p, scanner, lineNumber);
            }
            else{
                current = current->next = create_token(KEYWRD, p, scanner, lineNumber);
                current->keywdType = type;
            }
            continue;
        }
        /*
         *********************
         *    PUNCTUATORS    *
         * *******************
         */
        char* p = scanner;
        punctuatorKind tmpPunct = is_punctuator(scanner, &scanner);
        if (tmpPunct != NOTPUNCT){
            current = current->next = create_token(PUNCTR, p, scanner, lineNumber);
            current->punctType = tmpPunct;
            continue;
        }
        
        unexpected_token_error(scanner);
        // KEYWORDS
        scanner++;
    }
    current = current->next = create_token(END, scanner, scanner, lineNumber);
    return head.next;
};