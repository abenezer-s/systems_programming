//process_managment.h
#ifndef FILEINTERPRATER
#define FILEINTERPRATER

#define MAX_LINE_SIZE 1024
#define MAX_ARGUMENT 512
#include <stdio.h>
#include "lex_analysis.h"


void executeLines(int fd, FILE **fp, sigset_t mask);
// takes an fd as input and executes each line in the file and put it to stdout.
void allocateMem(char **array, int size);
void handle_pipe(tokenList_t tokenList, int NumPipes, sigset_t mask);
#endif