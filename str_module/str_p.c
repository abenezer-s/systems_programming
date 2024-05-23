#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <assert.h>

    char *firstOcc, *srcI, *mPos, *srcJ;
         
// returns the lenght of a chararcter array

    size_t str_getLength( char *s){
         char *p1;
        size_t n = 0;   
        for(p1 = s; *p1 != '\0' ; p1++ ){
                n++;
        }
        return n;
    }
    
// copies the elements of s2 to s1 returns a char pointer to s1
 
    char * str_copy(char *s1,  char *s2){
        
        char *p1;
         char *p2;
       
        assert(str_getLength(s1) >= str_getLength(s2));

        size_t remS1 = ((str_getLength(s1) - str_getLength(s2))) ;
       
        char *endofS1 = s1 + (str_getLength(s1) - 1);
         char *endofS2 = s2 + (str_getLength(s2) - 1);
        
        if(str_getLength(s1) > str_getLength(s2)){

            for(p1 = s1,p2 = s2; p1 <= endofS1 ; p1++, p2++){
                
            *p1 = *p2;
            }
            
            for(p1 = endofS1; p1 < (endofS1 + remS1); p1++){
                *p1 = '\0';
            }  
        } else {
            for(p1 = s1,p2 = s2; p1 <= endofS1 ; p1++, p2++){
                
            *p1 = *p2;
            }   
        }
        return s1;

    }
// copies n elements of s2 to s1 returns a char pointer to s1
 
     char * str_ncopy(char *s1,  char *s2, size_t n){
        
        char *p1;
         char *p2;
       
        assert(str_getLength(s1) >= str_getLength(s2));

        size_t remS1 = ((str_getLength(s1) - str_getLength(s2))) ;
       
        char *endofS1 = s1 + (str_getLength(s1) - 1);
         char *endofS2 = s2 + (str_getLength(s2) - 1);
        
        if(str_getLength(s1) > str_getLength(s2)){
            
            for(p1 = s1,p2 = s2; p1 < s1 + n  ; p1++, p2++){
                
                *p1 = *p2;
            }
            endofS1 = p1;
            for(p1 = endofS1; p1 < (endofS1 + remS1); p1++){
                 
                *p1 = '\0';
                
            }  
        } else {
            
            for(p1 = s1,p2 = s2; p1 < s1 + n ; p1++, p2++){
                
            *p1 = *p2;
            }   
        }
        return s1;

    }
   
//compares corresponding elements of s2 to s1 
//returns 1, -1, 0 depening on whether s1 is >,<, or = s2
    int str_compare( char *s1,  char *s2){
         char *p1, *p2;
        int iS1, iS2;
        for(p1 = s1,p2 = s2; p1 <= s1 + (str_getLength(s1) - 1); p1++, p2++){
            iS1 = (int)*p1;
            iS2 = (int)*p2;
            if(iS1 > iS2){
                return 1;
            } else if(iS1 < iS2){
                return -1;
            } else {
            
               if (p1 == s1 + (str_getLength(s1) - 1)){         
                    if (str_getLength(s1) > str_getLength(s2))
                        return 1;
                    else if (str_getLength(s1) < str_getLength(s2))
                        return -1;
                    else
                        return 0;}
                }
             }
        }
//compares n corresponding elements of s2 to s1 
//returns 1, -1, 0 depening on whether s1 is >,<, or = s2
    int str_ncompare( char *s1,  char *s2, size_t n){
         char *p1, *p2;
        int iS1, iS2;
        for(p1 = s1,p2 = s2; p1 < s1 + n; p1++, p2++){
            iS1 = (int)*p1;
            iS2 = (int)*p2;
            if(iS1 > iS2){
                return 1;
            } else if(iS1 < iS2){
                return -1;
            } else {
                if((str_getLength(s1) > str_getLength(s2)) && (p2 == s2 + (n - 1)) ){
                   
                        return 0;      
                                                                 
                }else if((str_getLength(s1) < str_getLength(s2)) && (p1 == s1 + (n- 1)) ){
                      
                        return 0;      
                  
                }
            }
        }
    }


//copies elements of s2 to s1 starting from the end of s1 returns a char pointer to s1

    char * str_concat(char *s1,  char *s2){
        char *p1, *p2, *endofS1, *endofS2;

        endofS1 = (s1 + (str_getLength(s1) - 1));
        endofS2 = (s2 + (str_getLength(s2) - 1));

        for(p1 = endofS1, p2 = s2 ; p2 < endofS2 ; p1++, p2++ ){
            *p1 = *p2;
            
        }
        *p1 = '\0';
        return s1;
    }
//copies n elements of s2 to s1 starting from the end of s1 returns a char pointer to s1
    char * str_nconcat(char *s1,  char *s2, size_t n ){
        char *p1, *p2, *endofS1, *amount;
       

        endofS1 = (s1 + (str_getLength(s1)));
        amount = s2 + (n - 1);

        for(p1 = endofS1, p2 = s2 ; p2 <= amount ; p1++, p2++ ){
            *p1 = *p2;
            
        }
        *p1 = '\0';
        return s1;
    }
// compare characters returns 1,0,-1
    int chr_compare( char *s1,  char *s2, char *srcI, char *srcJ){
        int iC1, iC2;

            iC1 = *srcI;
            iC2 = *srcJ;
            if(iC1 > iC2){
                return 1;
            } else if(iC1 < iC2){
                return -1;
            } else 
                 return 0;
                }
// retuns 0,1 depending on wheter it has fouond a match
    //int match(void){
    //    size_t ma = 0;
    //    for(srcJ = s2 + 1; srcJ < str_getLength(st2); srcJ++){
    //        
    //        if(chr_compare(st1, st2, (srcI + 1) , srcJ) == 0){
    //            srcI++;
    //            ma++;
    //        }
    //    }
    //        if((ma + 1) == str_getLength(st2)){
    //            return 1;
    //        }else 
    //            return 0;
    //    }
    
//return the first occurance of the string if a macth can be found otherwise reurns null
    char* str_search(  char *s1,  char *s2){
        
        int foundmatch;
        
        for(srcI = s1; srcI < s1 + str_getLength(s1); srcI++){
           
            if(chr_compare(s1, s2, srcI, s2) == 0){
                
                firstOcc = srcI;
                mPos = srcI;

            //check for consecutive matches......
                size_t ma = 0;
                for(srcJ = s2 + 1; srcJ < s2 + str_getLength(s2); srcJ++){

                    if(chr_compare(s1, s2, (srcI + 1) , srcJ) == 0){
                        srcI++;
                        ma++;
                    }
                }
                if((ma + 1) == str_getLength(s2)){
                    foundmatch = 1;
                }else 
                    foundmatch = 0;
            // .....................................
                
                if (foundmatch){
                return firstOcc;
            }else 
                srcJ = s2;

            }    
        }
        return NULL;
    }

    // int main(void){

    // char st1[7] = {'d','o','g','c','a','t','\0'};
    // char st2[4] = {'c','a','t','\0'};
    // char st3[7] = "dogcat";
    // char *c;

    // // st1[0] = 'a';
    // // st1[1] = 'c';
    // // st1[2] = 'r';
    // // st1[3] = '\0';
    // // st2[0] = 'c';
    // // st2[1] = 'a';
    // // st2[2] = 't';
    // // st2[3] = '\0';
    // //c = st1;
    // //d = st2 + 2;
    // //*c = *d;
    // printf("str_copy: %s\n" , str_copy(st1, st2));
    // //printf("end size of str1 %zu\nlenght of s1: %zu\nlenght of s2: %zu\n", sizeof(st1), 
    // //        str_getLength(st1),
    // //        str_getLength(st2));

    // return 0;
//}
