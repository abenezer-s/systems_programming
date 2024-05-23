#include <stdio.h>
#include <stdlib.h> 

#ifndef STR_MODULE_H
#define STR_MODULE_H

size_t str_getLength(const char *s);
char * str_copy(char s1[], const char s2[]);
char * str_ncopy(char s1[], const char s2[], size_t n);
char * str_concat(char *s1, const char *s2);
char * str_nconcat(char *s1, const char *s2, size_t n);


#endif