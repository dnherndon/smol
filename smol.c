#include "smol.h"

int main(int argc, char** argv){
    if (argc != 2){
        printf("Incorrect number of arguments\n");
        return 0;
    }
    char* input_buffer = argv[1];
    // Call Tokenizer
    TOKEN* tokens = tokenizer(input_buffer);
    while(tokens->type != END) {
        char* tmp = tokens->location;
        char* present_token = calloc(1, tokens->length);
        for(int i = 0; i < tokens->length; i++){
            present_token[i] += tmp[i];
        }
        printf("TOKEN:%s\n", present_token);
        free(present_token);
        tokens = tokens->next;
    }// Call Parser
    // Call Code generator
    return 0;
}