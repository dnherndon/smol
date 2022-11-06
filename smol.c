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

int main(int argc, char** argv){
    if (argc != 2){
        printf("Incorrect number of arguments\n");
        return 0;
    }
    char* input_buffer = argv[1];
    // Call Tokenizer
    TOKEN* token = tokenizer(input_buffer);
    TOKEN* head_token = token;
    while(token->type != END) {
        printf("TOKEN: ");
        print_token(token);
        printf("\n");
        token = token->next;
    }
    // Call Parser
    NODE* ast_node = parse(head_token);
    // Call Code generator
    return 0;
}
