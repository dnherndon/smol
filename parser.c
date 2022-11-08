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

NODE* create_constant_node(nodeKind kind, TOKEN* token){
    NODE* node = calloc(1, sizeof(NODE));

    node->kind = kind;
    if (token->integerType == INTEGERS) {
        node->constantVal = strtoul(token->location, &token->location, 10);
    }
    else if (token->integerType == HEX){
        token->location += 2;
        node->constantVal = strtoul(token->location, &token->location, 16);
    }
    return node;
}

NODE* parse(TOKEN* token){
    NODE head = {};
    NODE* curr_node = &head;
    while(token->lexElem != END){
        if (token->lexElem == CONSTANTS){
            curr_node = curr_node->next = create_constant_node(ND_INTEGERS, token);
            token = token->next;
            continue;    
        }
        if (token->lexElem == KEYWRD){
            //curr_node = curr_node->next = create_keyword_node(ND_)
            token = token->next;
            continue;
        }
        token = token->next;
    }
    return head.next;
}
