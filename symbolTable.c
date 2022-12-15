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

unsigned long int hashingFunction(char* str, int length)
{
    unsigned long int hash = 0xcbf29ce484222325;
    for (int i = 0; i < length; i++){
        hash ^= str[i];
        hash *= 0x100000001b3;
    }
    return hash;
}
symbolTable* createSymbolTable(int size)
{
    symbolTable* table = calloc(1, sizeof(symbolTable));
    table->count = 0;
    table->size = size;
    table->entry = calloc(size, sizeof(symTblEntry));
    return table;
}
symTblEntry* symTblInsert(symbolTable* table, char* key, SYMBOL* value)
{
    if (table->count == table->size){
        printf("Symbol table is out of room\n");
        exit(1);
    }
    unsigned long int hash = hashingFunction(key, strlen(key));

    for (int i = 0; i < table->size; i++){
        symTblEntry* entry = &table->entry[(hash + i) % table->size];
        if (entry->key == NULL){
            entry->key = key;
            entry->value = value;
            table->count++;
            return entry;
        } else if (memcmp(entry->key, key, strlen(key)) == 0){
            return NULL;
        }
    }
    printf("Out of room\n");
    exit(1);
    return NULL;
}

symTblEntry* symTblGet(symbolTable* table, char* key)
{
    unsigned long int hash = hashingFunction(key, strlen(key));

    for (int i = 0; i < table->size; i++){
        symTblEntry* entry = &table->entry[(hash + i) % table->size];
        if (entry->key == NULL){
            return NULL;
        }
        if (memcmp(entry->key, key, strlen(key)) == 0){
            return entry;
        }
    }

    return NULL;
}

void symTblDelete(symbolTable* table, char* key)
{
    symTblEntry* entry = symTblGet(table, key);

    entry->key = NULL;
    entry->value = NULL;
}

void enterScope(symbolTable** currentScope, symbolTable** nextScope)
{
    (*nextScope)->previous = *(currentScope);
    (*currentScope) = (*nextScope);
    (*currentScope)->scopeDepth++;
    return;
}

void exitScope(symbolTable** currentScope)
{  
    if ((*currentScope)->previous != NULL){
        (*currentScope) = (*currentScope)->previous;
    }
}