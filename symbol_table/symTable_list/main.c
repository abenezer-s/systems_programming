#include "symTable_list.h"

void main(void){

    int i,f,v,r, re, val1, val2, val3, val4;
    i = 5;
    f = 6;
    v = 7;
    r = 8;
    re = 12;
// symtable new
    SymTable_T s = SymTable_new();

//put
    SymTable_put(s, "key1", &i);
    SymTable_put(s, "key2", &f);
    SymTable_put(s, "key3", &v);
    SymTable_put(s, "key4", &r);

    SymTable_put(s, "key1", &i);
    SymTable_put(s, "key2", &f);
    SymTable_put(s, "key3", &v);
    SymTable_put(s, "key4", &r);

    printf("\n");
   // SymTable_put(s, "key3", &v);
//conatains
int con = SymTable_contains(s, "key5");
if(con == 1){
    printf("contains (%d)\n", *(int *)(SymTable_get(s,"key2")));
}else{
    printf("dosnt contain\n");
}

printf("\n");
//get
    if(SymTable_get(s, "key1") != NULL){
        val1 = *(int *)SymTable_get(s, "key1");
        printf("found key value1: %d\n", val1);
    }else{
    printf("key not found\n");
    }
    if(SymTable_get(s, "key2") != NULL){
        val2 = *(int *)SymTable_get(s, "key2");
       printf("found key value2: %d\n", val2);
    }else{
    printf("key not found\n");
    }
    if(SymTable_get(s, "key3") != NULL){
       val3 = *(int *)SymTable_get(s, "key3");
       printf("found key value3: %d\n", val3);
    }else{
    printf("key not found\n");
    }
    if(SymTable_get(s, "key4") != NULL){
       val4 = *(int *)SymTable_get(s, "key4");
       printf("found key value4: %d\n", val4);
    }else{
    printf("key not found\n");
    }
printf("\n");
//replace  
    
    if( (SymTable_replace(s, "key5", &re)) != NULL){
        printf("replaced binding(%d)\n", *(int *)(SymTable_get(s,"key4")));
    }else{
    printf("key not found. no binding replaced.\n");
    }
//
printf("\n");
////length
    printf("no. of bindings: %d\n", SymTable_getLength(s));
printf("\n");
//remove
    
    if(SymTable_remove(s, "key1") != NULL){
       printf("removed, key1: %d\n", val1);
    }else{
    printf("key not found\n");
    }
    if(SymTable_remove(s, "key2") != NULL){
       printf("removed, key2: %d\n", val2);
    }else{
    printf("key not found\n");
    }
    if(SymTable_remove(s, "key3") != NULL){
       printf("removed, key3: %d\n", val3);
    }else{
    printf("key not found\n");
    }
    if(SymTable_remove(s, "key4") != NULL){
        printf("removed, key4: %d\n", val4);
    }else{
    printf("key not found\n");
    }
printf("\n");
    if(val1,val2,val3,val4){printf("values of removed: %d, %d, %d, %d\n", val2, val3, val1, val4);}
printf("\n");

//length
    printf("no. of bindings: %d\n", SymTable_getLength(s));
printf("\n");
//put 
    SymTable_put(s, "key4", &r);
    SymTable_put(s, "key5", &re);
printf("\n");
//length
    printf("no. of bindings: %d\n", SymTable_getLength(s));
//get
    if(SymTable_get(s, "key4") != NULL){
       int val4 = *(int *)SymTable_get(s, "key4");
       printf("found key, value4: %d\n", val4);
    }else{
    printf("key not found\n");
    }
//length
    
// free 
printf("\n");
    printf("no. of bindings: %d\n", SymTable_getLength(s));
    SymTable_free(s);
   

}
