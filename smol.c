/*    smol is a small C compiler
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
    }
    // Call Parser
    // Call Code generator
    return 0;
}
