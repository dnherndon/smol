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
int symTblInsert(symbolTable* table, char* key, void* value)
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
            return 1;
        } else if (memcmp(entry->key, key, strlen(key)) == 0){
            return 0;
        }
    }
    printf("Out of room\n");
    exit(1);
    return 0;
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