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

FILE *out;

void emitCode(const char* format, ...){
    va_list args;
    va_start(args, format);
    vfprintf(out, format, args);
    va_end(args);
}

void generateRecursive(NODE* node){
    if (node == NULL){
        printf("No code to generate for now...\n");
        exit(1);
    }
    switch (node->kind){
    case NODE_INT:
        emitCode("    mov rax, %d\n", node->constantVal);
        return;
    case NODE_NEGATE:
        generateRecursive(node->left);
        emitCode("    neg rax\n");
        return;
    case NODE_NULL:
        return;
    case NODE_ADD:
        break;
    case NODE_SUB:
        break;
    case NODE_MUL:
        break;
    case NODE_END:
        break;
    case NODE_EQUIV:
        break;
    case NODE_NEQUIV:
        break;
    case NODE_LOGAND:
        break;
    }

    
    generateRecursive(node->right);
    emitCode("    push rax\n");
    generateRecursive(node->left);
    emitCode("    pop rcx\n");

    switch (node->kind){
        case NODE_ADD:                          // +
        emitCode("    add rax, rcx\n");
        return;
    case NODE_SUB:                          // -
        emitCode("    sub rax, rcx\n");
        return;
    case NODE_MUL:                          // *
        emitCode("    mul rcx\n");
        return;
    case NODE_DIV:                          // /
        emitCode("    xor rdx, rdx\n");
        emitCode("    idiv rcx\n");
        return;
    case NODE_MOD:
        emitCode("    xor rdx, rdx\n");
        emitCode("    idiv rcx\n");
        emitCode("    mov rax, rdx\n");
        return;
    case NODE_INT:
        break;
    case NODE_NULL:
        break;
    case NODE_END:
        break;
    case NODE_EQUIV:
        emitCode("    cmp rax, rcx\n");
        emitCode("    sete al\n");
        emitCode("    movzx rax, al\n");
        return;
    case NODE_NEQUIV:
        emitCode("    cmp rax, rcx\n");
        emitCode("    setne al\n");
        emitCode("    movzx rax, al\n");
        break;
    case NODE_LOGAND:
        return;
    }

}

int code_generator(NODE* node, FILE* outputFile){
    out = outputFile;
    emitCode("global main\n");
    emitCode("main:\n");
    generateRecursive(node);
    emitCode("    ret\n");
    return 1;
}