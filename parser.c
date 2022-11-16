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

int consume_token(TOKEN** token){
    *token = (*token)->next;
    return 1;
}

int type_specifier(TOKEN** token){
    if ((*token)->lexElem == KEYWRD){
        if((*token)->keywdType == VOID){
            consume_token(token);
            return 1;
        }
        else if ((*token)->keywdType == CHAR){
            consume_token(token);
            return 1;
        }
        else if ((*token)->keywdType == SHORT){
            consume_token(token);
            return 1;
        }
        else if ((*token)->keywdType == INT){                               
            consume_token(token);
            return 1;
        }
        else if ((*token)->keywdType == LONG){
            consume_token(token);
            return 1;
        }
        else if ((*token)->keywdType == FLOAT){
            consume_token(token);
            return 1;
        }
        else if ((*token)->keywdType == DOUBLE){
            consume_token(token);
            return 1;
        }
        else if ((*token)->keywdType == SIGNED){
            consume_token(token);
            return 1;
        }
        else if ((*token)->keywdType == UNSIGNED){
            consume_token(token);
            return 1;
        }
        else{
            printf("Not a valid type specifier: ");
            print_token(*token);
            printf("\n");
            return 0;
        }
    }
    else{
        printf("Expected type specifier, got: ");
        print_token(*token);
        printf("\n");
        return 0;
    }
}

int declaration_specifier(TOKEN** token){
    //storage_class_specifier();
    int tmp = type_specifier(token);
    //type_qualifer();
    return tmp;
}

int function_definition(TOKEN** token){
    // C standard says we can have multiple declaration specifiers
    for (;;){
        int tmp = declaration_specifier(token);
        if(tmp == 0) break;
    }
    return 1;
}


NODE* parse(TOKEN* token){
    NODE head = {};
    NODE* curr_node = &head;
    while(token->lexElem != END){
        // C says that the program starts with either 
        // a function-definition or a declaration
        // function-definitions and declarations both begin with declaration-specifiers
        function_definition(&token);

        token = token->next;
    }
    return head.next;
}
