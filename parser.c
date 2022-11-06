#include "smol.h"

NODE* create_node(nodeKind kind, TOKEN* token){
    NODE* node = calloc(1, sizeof(NODE));

    node->kind = kind;
    if (token->integerType == INTEGERS) {
        node->constantVal = strtoul(token->location, &token->location, 10);
    }
    else if (token->integerType == HEX){
        token->location += 2;
        node->constantVal = strtoul(token->location, &token->location, 16);
    }
    printf("%ld\n", node->constantVal);
    return node;
}

NODE* parse(TOKEN* token){
    NODE head = {};
    NODE* curr_node = &head;
    while(token->type != END){
        if (token->type == CONSTANTS){
            curr_node = curr_node->next = create_node(ND_INTEGERS, token);
            token = token->next;
            continue;    
        }
        token = token->next;
    }
    return head.next;
}
