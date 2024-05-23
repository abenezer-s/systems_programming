
#include "symTable_list.h"

SymTable_T SymTable_new(void){
    SymTable_T new_symTable;
    new_symTable = malloc(sizeof(struct symbolTable));
    if(new_symTable == NULL){
        printf("failed allocating symboltable\n");
        return NULL;
    }
    new_symTable -> first = NULL;
    printf("new symbol table added\n");
    return new_symTable;
}

void *SymTable_free(SymTable_T oSymtable){
    struct Binding *curBinding, *nextBinding;
    if(oSymtable == NULL){
        printf("NULL input\n");
        return NULL;
    }
    for(curBinding = oSymtable -> first;
        curBinding != NULL; 
        curBinding = nextBinding){    

        nextBinding = curBinding -> next;
        free(curBinding->key);
        free(curBinding);
    }
    free(oSymtable);
    printf("freed osytbl\n");
}

// retrun legnth of a symbol table or 0 if symbol table does not exist
int SymTable_getLength(SymTable_T oSymtable){
    struct Binding *curBinding;
    int tables = 0;
    if(oSymtable == NULL){
        printf("NULL input\n");
        return 0;
    }
    for(curBinding = oSymtable -> first; curBinding != NULL; curBinding = curBinding -> next ){
        tables += 1;
    }
    return tables;

}

// Insert a new key value pair in a symbol table. Returns 1 succefull 0 other wise.
int SymTable_put(SymTable_T oSymtable, char *key, void *value){
    struct Binding *new_binding;
    char *ownKey;
    if(oSymtable == NULL || key == NULL || value == NULL){
        printf("NULL input(s)\n");
        return 0;
    }
    if(SymTable_contains(oSymtable, key)){
            printf("key exists\n");
            return 0;
        }  
    
    new_binding = malloc(sizeof(struct Binding));
    if(new_binding == NULL){
        printf("failed in symtable_put\n");
        return 0;
    }
    ownKey = malloc(sizeof(strlen(key) + 1));
    if(ownKey != NULL){
        strcpy(ownKey, key);
        printf("key owned.\n");
    }
    oSymtable->length += 1;
    new_binding->key = ownKey;
    new_binding->value = value;
    new_binding->next = oSymtable->first;
    oSymtable->first = new_binding;
    printf("    new binding added\n");
    //printf("key: %s\n value: %d\n", new_binding -> key, *((int*)new_binding -> value));
    return 1;

}

// Replace value of a binding with new value and return old value. Return 0 is unsuccessful.
void * SymTable_replace(SymTable_T oSymtable, char *key, void *newValue){
    struct Binding *curBinding;
    if(oSymtable == NULL || key == NULL || newValue == NULL){
        printf("NULL input(s)\n");
        return 0;
    }
    for(curBinding = oSymtable -> first; curBinding != NULL; curBinding = curBinding -> next ){
        if(strcmp(curBinding -> key, key) == 0 ){
            void *oldValue = curBinding -> value;
            curBinding -> value = newValue;
            return oldValue;
        }
    }
    return 0;
}

// eturn 1 if the argument symbol table contains the key or return 0 if if key doesn't exist.
int SymTable_contains(SymTable_T oSymtable, char *key){
    struct Binding *curBinding;
    if(oSymtable == NULL || key == NULL){
        printf("NULL input(s)\n");
        return 0;
    }
    for(curBinding = oSymtable -> first; curBinding != NULL; curBinding = curBinding -> next ){
        if(strcmp(curBinding -> key, key) == 0 ){
            return 1;
        }
    } 
    return 0;
}

void *SymTable_get(SymTable_T oSymtable, const char *key){
    struct Binding *curBinding;
    if(oSymtable == NULL || key == NULL){
        printf("NULL input(s)\n");
        return NULL;
    }
    for(curBinding = oSymtable->first;
        curBinding != NULL; 
        curBinding = curBinding->next){

        if(strcmp(curBinding->key, key) == 0 ){
            return curBinding->value;
        }
    } 
    return NULL;
}

void *SymTable_remove(SymTable_T oSymtable, char *key){
    struct Binding *curBinding, *nextBinding, *prevTable;
    void *removedValue;
    if(oSymtable == NULL || key == NULL){
        printf("NULL input(s)\n");
        return NULL;
    }
    for(curBinding = oSymtable -> first, prevTable = NULL; 
        curBinding != NULL; 
        prevTable = curBinding, curBinding = nextBinding){

        nextBinding = curBinding -> next;
        if(strcmp(curBinding -> key, key) == 0 ){
            if(oSymtable->length == 1){
                oSymtable->first = nextBinding;
                removedValue = curBinding -> value;
                free(curBinding->key);
                free(curBinding);
                oSymtable->length -= 1;
                return removedValue;
            
            } else{
            removedValue = curBinding -> value;
            free(curBinding->key);
            free(curBinding);
            prevTable->next = nextBinding;
            oSymtable->length -= 1;

            return removedValue;
            }
        }
    }

    return NULL;
}



void *SymTable_map(SymTable_T oSymtable,
   void (*pfApply)(char *pcKey, void *pvValue, void *pvExtra),
   void *pvExtra){
    if(oSymtable == NULL || pfApply == NULL || pvExtra == NULL){
        printf("invalid input(s)\n");
        return NULL;
    }
        struct Binding *curBinding;
        
        for(curBinding = oSymtable -> first ; curBinding != NULL ; curBinding = curBinding -> next){
            
           (*pfApply)(curBinding -> key, curBinding -> value, pvExtra);
        } 
       
   }    


