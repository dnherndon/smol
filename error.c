#include "smol.h"

void unexpected_token_error(char* scanned){
    printf("Unexpected token: %c\n", *scanned);
    exit(1);
}