#include "smol.h"

NODE* create_node(nodeKind kind, TOKEN* token, TOKEN** tokenloc){
    NODE* node = calloc(1, sizeof(NODE));

    node->kind = kind;
    printf("%s\n", token->location);
    node->constantVal = strtoul(token->location, &token->location, 10);

    return node;
}

NODE* parse(TOKEN* token){
    NODE head = {};
    NODE* curr_node = &head;
    while(token->type != END){
        if (token->type == CONSTANTS){
            curr_node = curr_node->next = create_node(ND_INTEGERS, token, &token);
            token = token->next;
        }
        token = token->next;
    }
    return head.next;
}
