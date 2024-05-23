#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <assert.h>
#include "str_module.h"
int chr_compare(const char *s1, const char *s2, size_t srcI, size_t srcJ);
int match(void);
char* str_search( char *s1, const char *s2);
char st1[7] = {'z','a','z','c','a','j','\0'};
char st2[4] = {'c','a','j','\0'};
char st3[7] = "dogcat";

//void main(void){
//     
//    
//
//    printf("first str_search: %s\n", str_search(st1, st2)); 
//    
//    }
    char *firstOcc;
    size_t srcI, mPos, srcJ;

// returns the lenght of a chararcter array

    size_t str_getLength(const char s[]){
        
        size_t i, n = 0;

        for(i = 0; s[i] != '\0' ; i++ ){
                n++;
        }
        return n;
    }
    
// copies the elements of s2 to s1 returns a char pointer to s1
    
    char * str_copy(char s1[], const char s2[]){
        
        size_t i;
        size_t sizeS1 = str_getLength(s1);
        assert(sizeS1 >= (str_getLength(s2)+1));
        for(i = 0 ; i <= str_getLength(s2) ; i++){
            s1[i] = s2[i];
        }
        s1[i] = '\0';

        return s1;
    }                   

//copies n elements of s2 to s1 returns a char pointer to s1
 
    char * str_ncopy(char s1[], const char s2[], size_t n){
        
        size_t i;
        size_t sizeS1 = str_getLength(s1);
        assert(sizeS1 >= (n + 1));
        for(i = 0 ; i < n ; i++){
            s1[i] = s2[i];
        }
        if (sizeS1 > (n + 1)) {
            for(size_t j = i; j < (sizeS1 - n); j++){
                s1[j] = '\0';
            }
        }

        s1[i] = '\0';
        return s1;
    } 

//copies elements of s2 to s1 starting from the end of s1 returns a char pointer to s1

    char * str_concat(char *s1, const char *s2){

        size_t i, endofS1, sizeS1;
        
        assert(sizeof(s1) >= (str_getLength(s1) + str_getLength(s2)) + 1);
        endofS1 = (str_getLength(s1));
        
        for(i = 0; i < str_getLength(s2); i++){
            
            s1[(endofS1 + i)] = s2[i];
        }
        s1[(endofS1 + i)] = '\0';
        return s1;
    }
//copies n elements of s2 to s1 starting from the end of s1 returns a char pointer to s1

    char * str_nconcat(char *s1, const char *s2, size_t n){

        size_t i, endofS1, sizeS1;
        
        assert(sizeof(s1) >= (str_getLength(s1) + str_getLength(s2)) + 1);
        endofS1 = (str_getLength(s1));
        
        for(i = 0 ; i < n ; i++){
            
            s1[(endofS1 + i)] = s2[i];
        }
        s1[(endofS1 + i)] = '\0';
        return s1;
    }  

//compares corresponding elements of s2 to s1 
//returns 1, -1, 0 depening on whether s1 is >,<, or = s2
    int str_compare(const char *s1, const char *s2){
        size_t i;
        int iS1, iS2;

        for(i = 0; i < str_getLength(s1); i++){
            iS1 = s1[i];
            iS2 = s2[i];
            if(iS1 > iS2){
                return 1;
            } else if(iS1 < iS2){
                return -1;
            } else {
            
               if (i == (str_getLength(s1) - 1)){         
                    if (str_getLength(s1) > str_getLength(s2))
                        return 1;
                    else if (str_getLength(s1) < str_getLength(s2))
                        return -1;
                    else
                        return 0;
                }
                }
             }
        }
//compares n corresponding elements of s2 to s1 
//returns 1, -1, 0 depening on whether s1 is >,<, or = s2
    int str_ncompare(const char *s1, const char *s2, size_t n){
        size_t i;
        int iS1, iS2;
        for(i = 0; i < n; i++){
            iS1 = s1[i];
            iS2 = s2[i];
            if(iS1 > iS2){
                return 1;
            } else if(iS1 < iS2){
                return -1;
            } else {
            
               if (i == (n - 1)){         
                    if (str_getLength(s1) > str_getLength(s2))
                        return 1;
                    else if (str_getLength(s1) < str_getLength(s2))
                        return -1;
                    else
                        return 0;}
                }
             }
        }
    
// compare characters returns 1,0,-1
    int chr_compare(const char *s1, const char *s2, size_t srcI, size_t srcJ){
        int iC1, iC2;

            iC1 = s1[srcI];
            iC2 = s2[srcJ];
            if(iC1 > iC2){
                return 1;
            } else if(iC1 < iC2){
                return -1;
            } else 
                 return 0;
                }
// retuns 0,1 depending on wheter it has fouond a match
    int match(void){
        size_t ma = 0;
        for(srcJ = 1; srcJ < str_getLength(st2); srcJ++){
            
            if(chr_compare(st1, st2, (srcI + 1) , srcJ) == 0){
                srcI++;
                ma++;
            }
        }
            if((ma + 1) == str_getLength(st2)){
                return 1;
            }else 
                return 0;
        }
    
//return the first occurance of the string if a macth can be found otherwise reurns null
    char* str_search( char *s1, const char *s2){
        int foundmatch;
        
        for(srcI = 0; srcI < str_getLength(s1); srcI++){
           
            if(chr_compare(s1, s2, srcI, 0) == 0){
           
                firstOcc = &s1[srcI];
                mPos = srcI;
                foundmatch = match();
                if (foundmatch){
                    return firstOcc;
                }else 
                    srcJ = 0;
            }
        }
        return NULL;
    }



    


