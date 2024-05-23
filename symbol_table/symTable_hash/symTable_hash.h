#ifndef SYMTABLE_H
#define SYMTABLE_H

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

struct Binding{
    char *key;
    void *value;
    struct Binding *next;
};

struct hashTable{
    int prevBucketCount;
    int curBucketCount;
    int curBindingsCount;
    struct Binding **array;
};

typedef struct hashTable *SymTable_T;

SymTable_T SymTable_new(void);
void SymTable_free(SymTable_T oSymtable);
int SymTable_getLength(SymTable_T oSymtable);
int SymTable_put(SymTable_T oSymtable, char *key, void *value);
int SymTable_contains(SymTable_T oSymtable, const char *key);
void *SymTable_get(SymTable_T oSymtable, char *key);
void *SymTable_replace(SymTable_T oSymtable, const char *key,void *value);
void *SymTable_remove(SymTable_T oSymtable, const char *key);
void *SymTable_map(SymTable_T oSymtable, void (*pfApply)(const char *pcKey, void *pvValue, void *pvExtra), void *pvExtra);

#endif