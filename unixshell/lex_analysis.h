#ifndef ANALYZER
#define ANALYZER
#include <stdbool.h>
#include "token.h"

//enum Redirect {NONE, STDOUT, STDIN};

tokenList_t lex(char *string, int *NumPipes);

#endif