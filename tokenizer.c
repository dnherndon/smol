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
        }
        // "Normal" numbers
        if (isdigit(*scanner)){
            char* p = scanner;
            while(isdigit(*scanner)) scanner ++;
            current = current->next = create_token(CONSTANTS, p, scanner);
            continue;
        }
        // KEYWORDS
        scanner++;
    }
    current = current->next = create_token(END, scanner, scanner);
    return head.next;
};