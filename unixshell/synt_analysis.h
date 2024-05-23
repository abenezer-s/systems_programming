//synt_analysis.h
#ifndef SYNTANL
#define SYNTANL

#include "lex_analysis.h"


void synt(tokenList_t tokenList, int NumPipe, sigset_t mask);
void execute(char *array[MAX_ARGUMENT], enum Redirect redir, char *fileName, sigset_t mask);

#endif