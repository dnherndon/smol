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

// Forward declarations and prototypes
void generateFunction(NODE* node);
void generateExpression(NODE* node);
void generateStatement(NODE* node);
void generateDeclaration(NODE* node);
void generateVariable(NODE* node);
int alignStack(int numVars);

int alignStack(int numVars)
{
    if (numVars != 0){
        int temp = ((numVars + 1)*8) / 16;
        int stackSize = (temp * 16);
        return stackSize;
    }
    return 0;
}

FILE *out;
symbolTable* codegenTable;

void emitCode(const char* format, ...){
    va_list args;
    va_start(args, format);
    vfprintf(out, format, args);
    va_end(args);
}

void generateFunction(NODE* node){
    if(node->left->kind == NODE_FUNCDEC){
        generateFunction(node->left);
    }
    // For functions, traverse the tree pre-order
    switch (node->kind){
    case NODE_FUNCDEC:
        if (match(node->symbolName, "main")){
            emitCode("global main\n");
        }
        emitCode("%s:\n", node->symbolName);
        symTblEntry* entry = searchScope(codegenTable, node->symbolName);
        enterScope(&codegenTable, &entry->value->scope);
        emitCode("    push rbp\n");
        emitCode("    mov rbp, rsp\n");
        if (codegenTable->count > 0){
            emitCode("    sub rsp, %d\n", alignStack(codegenTable->count));
        }
        // For function body, traverse the tree post-order
        if ((node->right->kind == NODE_STATEMENT) || (node->right->kind == NODE_DECLARATION)){
            generateStatement(node->right);
        }
        if (codegenTable->count > 0){
            emitCode("    add rsp, %d\n", alignStack(codegenTable->count));
        }
        emitCode("    pop rbp\n");
        exitScope(&codegenTable);
        emitCode("    ret\n");
        return;
    }
}

void generateStatement(NODE* node){
    if (node->right->kind == NODE_STATEMENT){
        generateStatement(node->right);
    } else if (node->right->kind == NODE_DECLARATION){
        generateStatement(node->right);
    }
    generateExpression(node->left);
    return;
}

void generateExpression(NODE* node){
    if (node == NULL){
        return;
    }
    switch (node->kind){
    case NODE_INT:
        emitCode("    mov rax, %d\n", node->constantVal);
        return;
    case NODE_NEGATE:
        generateExpression(node->left);
        emitCode("    neg rax\n");
        return;
    case NODE_FUNCALL:
        emitCode("    mov rax, 0\n");
        emitCode("    call %s\n", node->symbolName);
        return;
    case NODE_ASSIGN:
        generateExpression(node->right);
        emitCode("    mov ");
        generateVariable(node->left);
        emitCode(", rax\n");
        return;
    case NODE_RETURN:
        generateExpression(node->left);
        return;
    case NODE_VAR:
        emitCode("    mov rax, ");
        generateVariable(node);
        emitCode("\n");
        return;
    case NODE_DECLASS:
        if (node->left->kind == NODE_ASSIGN){
            generateExpression(node->left);
        }
        return;
    }

    generateExpression(node->right);
    emitCode("    push rax\n");
    generateExpression(node->left);
    emitCode("    pop rcx\n");

    switch (node->kind){
    case NODE_ADD:                          // +
        emitCode("    add rax, rcx\n");
        return;
    case NODE_SUB:                          // -
        emitCode("    sub rax, rcx\n");
        return;
    case NODE_MUL:                          // *
        emitCode("    imul rax, rcx\n");
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
    case NODE_EQUIV:
        emitCode("    cmp rax, rcx\n");
        emitCode("    sete al\n");
        emitCode("    movzx rax, al\n");
        return;
    case NODE_NEQUIV:
        emitCode("    cmp rax, rcx\n");
        emitCode("    setne al\n");
        emitCode("    movzx rax, al\n");
        return;
    }
    printf("Invalid node: %d\n", node->kind);
    exit(1);
}

void generateVariable(NODE* node){
    symTblEntry* entry;
    switch(node->kind){
    case NODE_VAR:
        entry = searchScope(codegenTable, node->symbolName);
        emitCode("[rbp - %d]", (entry->value->offset)*8);
        return;
    }
}

int code_generator(NODE* node, FILE* outputFile, symbolTable* table){
    out = outputFile;
    codegenTable = table;
    //printf("Here: %d\n", node->right->kind);
    //printf("Here: %d\n", node->right->right->kind);
    //printf("Here: %d\n", node->right->right->right->kind);
    //printf("Here: %d\n", node->right->right->right->right->kind);
    //printf("Here: %d\n", node->right->right->right->right->right->kind);
    //printf("Here: %d\n", node->right->right->right->right->right->left->kind);
    generateFunction(node);
    return 1;
}