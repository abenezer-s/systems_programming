#include "symTable_hash.h"

enum {BucketCount = 509};
enum {FirstTier = 509 , SecondTier = 1021, ThirdTier =  2039, FourthTier = 4093, 
    FifthTier = 8191, SixthTier = 16381, SeventhTier = 32749, EighthTier = 65521};

int tierArray[7] = {[0] = 1};

enum {HASH_MULTIPLIER = 65599};
 
static int SymTable_hash(const char *key, int BucketCount)

/* Return a hash code for key that is between 0 and iBucketCount-1,
   inclusive.  Adapted from the COS 217 lecture notes. */

{
   int i;
   unsigned int uiHash = 0U;
   for (i = 0; key[i] != '\0'; i++)
      uiHash = uiHash * (unsigned int)HASH_MULTIPLIER
               + (unsigned int)key[i];
   return (int)(uiHash % (unsigned int)BucketCount);
}
// return a new symbole table with no bindings or return null if insufficient mem
SymTable_T SymTable_new(void){

    SymTable_T new_hashTable;
    new_hashTable = malloc(sizeof(struct hashTable));
    if(new_hashTable == NULL){
        printf("couldnt allocate mem for new_hashTable\n");
        return NULL;
    }
    int elements = (int)BucketCount;
    new_hashTable->array = (struct Binding **)calloc(elements, sizeof(void *));
    if(new_hashTable->array == NULL){
        printf("couldnt allocate mem for array\n");
        free(new_hashTable);
        return NULL;
    }
    new_hashTable->curBucketCount = elements;
    printf("curBuckCount: %d\n",(int)(new_hashTable -> curBucketCount));
    return new_hashTable;
}
// free a symbol table 
void SymTable_free(SymTable_T oSymtable){
    struct Binding *curBinding, *nextBinding;
    int i;
    for(i = 0; i < (oSymtable -> curBucketCount); i++){
        if(oSymtable -> array[i]){
            for(curBinding = oSymtable -> array[i]; curBinding != NULL; curBinding = nextBinding){
                nextBinding = curBinding -> next;
                free(curBinding->key);
                free(curBinding);
            }
        } 
    }
    printf("freed symtable\n");
    free(oSymtable -> array);
    free(oSymtable);
}
// eturn 1 if the argument symbol table contains the key or return 0 if if key doesn't exist
int SymTable_contains(SymTable_T oSymtable, const char *key){
    struct Binding *curBinding, *nextBinding;
    int i;
    for(i = 0; i < (oSymtable -> curBucketCount); i++){
        if(oSymtable -> array[i]){
            for(curBinding = oSymtable -> array[i]; 
                curBinding != NULL; 
                curBinding = nextBinding)
            {
                nextBinding = curBinding -> next;
                if(strcmp(curBinding->key, key) == 0){
                    return 1;
                }   
            }
        } 
    }
    return 0;
}
//return the lenght of a symbol table
int SymTable_getLength(SymTable_T oSymtable){
    
    int length = oSymtable->curBindingsCount;
    
    return length;    
}
// return 1 or 0 depending on wether the array was expanded.
int growHashTable(SymTable_T oSymtable){
    int i, curBucketSizeI, nextBucketSizeI, newBucketSize;
    void **grownArray;
// current buckect size
    for(i = 0; i < 8; i++){
        if(tierArray[i] == 1){
            curBucketSizeI = i;
            nextBucketSizeI = i + 1;
            tierArray[i] = 0;
            tierArray[i + 1] = 1;
            break;
        }
    }
   
    printf("nextBucketSizeI %d\n", nextBucketSizeI);
    switch(nextBucketSizeI){
        case 0: newBucketSize = (int) FirstTier;
                    break;
        case 1: newBucketSize = (int) SecondTier;
                  break;
        case 2: newBucketSize = (int) ThirdTier;
                  break;
        case 3: newBucketSize = (int) FourthTier;
                  break;
        case 4: newBucketSize = (int) FifthTier;
                  break;
        case 5: newBucketSize = (int) SixthTier;
                  break;
        case 6: newBucketSize = (int) SeventhTier;
                  break;
        case 7: newBucketSize = (int) EighthTier;
                    break;
    }

    grownArray = realloc(oSymtable -> array, sizeof(void *) * newBucketSize);
    if(growHashTable == NULL){
        printf("couldnt grow table\n");
        return 0;
    }
    oSymtable->prevBucketCount = oSymtable->curBucketCount;
    oSymtable->curBucketCount = newBucketSize;
    oSymtable->array = (struct Binding **)grownArray;
    printf("    GROWN \n newBucketSize:%d prev: %d\n\n", newBucketSize, oSymtable ->prevBucketCount);

    return 1;
}  

// return 1 if key exists in a given bucket
int inBucket(SymTable_T oSymtable, int index, char *key) {
    struct Binding *curBinding;
    for(curBinding = oSymtable->array[index]; 
        curBinding != NULL;
        curBinding = curBinding->next ){
        if(strcmp(curBinding -> key, key) == 0){
            return 1;
        }
    }
    return 0;
}

// Reallocates bindings when an array is grown.
int reallocateBindings(SymTable_T oSymtable){
    struct Binding *curBinding, *prevBinding, *nextBinding;
    int i;
    for(i = 0; i < (oSymtable->prevBucketCount); i++){
            if(oSymtable->array[i]){
            for(curBinding = oSymtable->array[i],prevBinding = NULL ; 
                curBinding != NULL; 
                prevBinding = curBinding, curBinding = nextBinding){

                int newHash = SymTable_hash(curBinding -> key, oSymtable -> curBucketCount);
                printf("new hash %d\n",newHash);

                nextBinding = curBinding->next;
                if(oSymtable -> array[newHash] != NULL){
                    if(!inBucket(oSymtable, newHash, curBinding->key)){
                            if(prevBinding != NULL){
                                prevBinding->next = nextBinding;
                        
                            } else {
                                oSymtable->array[i] = nextBinding;
                            }

                            curBinding->next = oSymtable -> array[newHash];
                            oSymtable->array[newHash] = curBinding; 
                            printf("new allocation at index: %d\n",newHash);         
                    }else{
                            printf("exists in BUCKET\n");         
                    }

                } else {
                        if(prevBinding != NULL){
                                prevBinding->next = nextBinding;
                        
                            } else {
                                oSymtable->array[i] = nextBinding;
                            }
                        oSymtable -> array[newHash] = curBinding;
                        printf("new allocation at index (NULL): %d\n",newHash);         
                    }
            } 
        }
    }
    return 0;
}

// Insert a new key value pair in a symbol table. Grows hash table if neccessary.
int SymTable_put(SymTable_T oSymtable, char *key, void *value){
    struct Binding *curBinding, *new_binding;
    int increased, reallocated, hash;
    char *ownKey;
    
    if(oSymtable != NULL && key != NULL && value != NULL){

        if(oSymtable->curBucketCount == oSymtable->curBindingsCount && 
            oSymtable->curBucketCount == (int)EighthTier){
                    printf("hash table FULL\n");
                    return 0;
                }

        if(!SymTable_contains(oSymtable, key)){
                ownKey = malloc(strlen(key) + 1);
                strcpy(ownKey, key);
                printf("owned key.\n");

            if(oSymtable->curBucketCount == oSymtable -> curBindingsCount){
                
                printf("bindigs = buckets.\n");
                increased = growHashTable(oSymtable);
                if(increased){
                    printf("increased array.\n");
                    reallocated = reallocateBindings(oSymtable);
                    if(reallocated){
                        printf("reallocated.\n");
                        }
                }
            }  

            new_binding = malloc(sizeof(struct Binding));
            if(new_binding == NULL){
             printf("failed to allo mem for new binding\n");
             return 0;
            }
            oSymtable -> curBindingsCount += 1;
            //printf("curbindings: %d\n", oSymtable -> curBindingsCount);
            
            hash = SymTable_hash(ownKey, oSymtable -> curBucketCount);

            new_binding->key = ownKey;
            new_binding->value = value; 
            new_binding->next = oSymtable -> array[hash];
            oSymtable->array[hash] = new_binding;
            printf("new binding added\n");
            printf("hash: %d\n\n", hash);

            return 1;
                
        } else {
            printf("key exists\n");
            }
    }  else{
        printf("null argument(s)\n\n");
        return 0;
    }  
}

//returns the value of key argument if it exists or returns null.
void *SymTable_get(SymTable_T oSymtable, char *key){
    struct Binding *curBinding;
    int i, iterated = 0;
    if(oSymtable == NULL || key == NULL){
        printf("NULL input(s)\n");
        return NULL;
    }

    for(i = 0; i < (oSymtable -> curBucketCount); i++){
        if(oSymtable->array[i] != NULL){
            for(curBinding = oSymtable->array[i]; curBinding != NULL; curBinding = curBinding->next){
                if(strcmp((oSymtable->array[i]->key), key) == 0){

                    return (void *)(oSymtable->array[i]->value);
                } 
            }
        } 
    }
    return NULL;
}

// Removes a binding and retuns it value.
void *SymTable_remove(SymTable_T oSymtable, const char *key){
    struct Binding *curBinding, *nextBinding, *prevBinding;
    int i;

    if(oSymtable == NULL || key == NULL){
        printf("NULL input(s)\n");
        return NULL;
    }
    for(i = 0; i < (oSymtable -> curBucketCount); i++){
        if(oSymtable->array[i] != NULL){
            for(curBinding = oSymtable -> array[i], prevBinding = NULL;
                curBinding != NULL; 
                prevBinding = curBinding, curBinding = nextBinding){

                nextBinding = curBinding->next;
                if(strcmp(curBinding->key, key) == 0){
                    void *oldVal = curBinding->value;
                    if(prevBinding != NULL){
                        prevBinding->next = nextBinding;
                        free(curBinding->key);
                        free(curBinding);
                    } else {
                        free(curBinding->key);
                        free(curBinding);
                        oSymtable->array[i] = nextBinding;
                    }
                    printf("removed a binding\n");
                    oSymtable->curBindingsCount -= 1;
                    //printf("curBindings: %d\n", oSymtable->curBindingsCount);
                    return (void *)oldVal;          
                }
            }
        }
    }
    return NULL; 
} 

// Replace value of a binding with new value and return old value.
void *SymTable_replace(SymTable_T oSymtable, const char *key, void *value){
   
    struct Binding *curBinding, *nextBinding;
    int i;
    if(oSymtable == NULL || key == NULL || value == NULL){
        printf("NULL input(s)\n");
        return NULL;
    }
    for(i = 0; i < (oSymtable -> curBucketCount); i++){
        if(oSymtable->array[i] != NULL){
            for(curBinding = oSymtable -> array[i];
                curBinding != NULL; 
                curBinding = nextBinding){

                nextBinding = curBinding->next;
                if(strcmp(curBinding->key, key) == 0){
                    void *oldVal = curBinding->value;
                    curBinding -> value = value;
                    printf("replaced a binding (%s)\n\n", curBinding->key);
                    //printf("curBindings: %d\n", oSymtable->curBindingsCount);
                    return (void *)oldVal;          
                }
            }
        }
    }
    return NULL; 
} 
// Map function to apply a function to bindings of a symbol table
void *SymTable_map(SymTable_T oSymtable, void (*pfApply)(const char *pcKey, void *pvValue, void *pvExtra), void *pvExtra){
    struct Binding *curBinding;
    int i;
    unsigned int length = 0;
    if(oSymtable == NULL || pfApply == NULL || pvExtra == NULL){
        printf("NULL input(s)\n");
        return NULL;
    }
    for(i = 0; i < (oSymtable -> curBucketCount); i++){
        if(oSymtable -> array[i]){
            for(curBinding = oSymtable -> array[i]; curBinding != NULL; curBinding = curBinding -> next){
                (*pfApply)(curBinding->key, curBinding->value, pvExtra);
            }
        } 
    }   
}


