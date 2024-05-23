
#ifndef SYMTABLE_H
#define SYMTABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct symbolTable *SymTable_T;

struct Binding{
    char *key;
    void *value;
    struct Binding *next;
};
struct symbolTable{
    int length;
    struct Binding *first;
};

SymTable_T SymTable_new(void);

void *SymTable_map(SymTable_T oSymtable,
   void (*pfApply)(char *pcKey, void *pvValue, void *pvExtra),
   void *pvExtra);

void *SymTable_remove(SymTable_T oSymtable, char *key);

int SymTable_contains(SymTable_T oSymtable, char *key);

void * SymTable_replace(SymTable_T oSymtable, char *key, void *newValue);

int SymTable_put(SymTable_T oSymtable, char *key, void *value);

int SymTable_getLength(SymTable_T oSymtable);

void *SymTable_free(SymTable_T oSymtable);

SymTable_T SymTable_new(void);

void *SymTable_get(SymTable_T oSymtable, const char *key);



#endif