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

void generateRecursive(NODE* node){
    switch (node->kind){
    case NODE_INT:
        printf("    mov rax, %d\n", node->constantVal);
        return;
    case NODE_NULL:
        break;
    case NODE_ADD:
        break;
    case NODE_MUL:
        break;
    case NODE_END:
        break;
    }

    generateRecursive(node->right);
    printf("    push rax\n");
    generateRecursive(node->left);
    printf("    pop rbx\n");

    switch (node->kind){
    case NODE_ADD:
        printf("    add rax, rbx\n");
        return;
    case NODE_MUL:
        printf("    mul rbx\n");
        return;
    case NODE_INT:
        break;
    case NODE_NULL:
        break;
    case NODE_END:
        break;
    }

}

int code_generator(NODE* node){
    printf("global main\n");
    printf("main:\n");
    generateRecursive(node);
    printf("    ret\n");
    return 1;
}