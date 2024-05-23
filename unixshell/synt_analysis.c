//synt_analysis.c
#include <signal.h>
#include <stddef.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include "process_managment.h"
#include "lex_analysis.h"
#include "synt_analysis.h"
//#include <asm-generic/signal.h>

int count_cmd(tokenList_t token_list)
{
    token_t cur_token;
    int nArgs = 0;
    for(cur_token = token_list->tokens;
        cur_token != NULL;
        cur_token = cur_token->next){
            if(cur_token->type == ARG || cur_token->type == PIPE){
                nArgs++;
            }
        }
        return nArgs;
}

void synt(tokenList_t token_list, int Num_Pipe, sigset_t mask)
{// takes in a token list then returns an array which contaians arguments for execvp. 
// sets redirect to true if there is '<' or '>'
    int i,j;
    token_t cur_token, next_token;
    enum Redirect enRedirect = NONE;
    char *file_name;
    int nArguments = count_cmd(token_list);
    char *args[MAX_ARGUMENT];
	for(i = 0; i< (nArguments + 1); i++){
        args[i] = malloc(MAX_LINE_SIZE * sizeof(char));
        if(args[i] == NULL){
            perror("malloc");
            exit(EXIT_FAILURE);
        }
    }
    i = 0;
    for(cur_token = token_list->tokens;
        cur_token != NULL;
        cur_token = cur_token->next)
    {
        if(cur_token->type == ARG){
            strcpy(args[i++], cur_token->val);
        }else if(cur_token->type == REDIRECTION){
            enRedirect = cur_token->redir;
            
        }else if(cur_token->type == isFILE){
            file_name = cur_token->val;
        }else{
            strcpy(args[i++], cur_token->val);
        }
    }
    args[i] = NULL;
    int NumArg = token_list->num_tokens;
    if(token_list->pipe_exists) {
        handle_pipe(token_list, Num_Pipe, mask);
    }else{
	    execute(args, enRedirect, file_name, mask);
    }
    //free args
    for(i = 0; i < (nArguments + 1); i++){
        free(args[i]);
    }
    //free tokenList
    for(cur_token = token_list->tokens;
        cur_token != NULL;
        cur_token = next_token)
        {
            next_token = cur_token->next;
            free(cur_token->val);
            free(cur_token);
        }
        free(token_list);
        return;
}

void execute(char *array[MAX_ARGUMENT], enum Redirect redir, char *file_name, sigset_t mask)
{//takes in an array and execute command and set redirect to false
    int fd;
    //char *wPath = "/home/abenezer/Documents/2023/System_Programming/C/unixshell"; // working directory path
    mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
	if(strcmp(array[0], "setenv") != 0 &&
            strcmp(array[0], "cd") != 0 && 
            strcmp(array[0], "unsetenv") != 0 && 
            strcmp(array[0], "exit") != 0)
        { 
            if(redir != NONE)
            {
                if(redir == STDOUT){
                    // cd to working 
                    //chdir(wPath);
                    pid_t pid;
                    if((pid = fork()) == 0)   //in child
                    {
                        //unblock SIGINT
                        sigprocmask(SIG_UNBLOCK, &mask, NULL);
                        if((fd = open(file_name,O_WRONLY | O_CREAT | O_TRUNC, mode)) == -1){
                            perror("open");
                        }
                        close(1);
                        dup(fd);
                        close(fd);
                        if(execvp(array[0], array) < 0)
                        {
                            printf("%s: ish: Command not found.\n", array[0]);
                        }
                        exit(0);
                    }
                    wait(NULL);
                }else if(redir == STDIN){
                    //chdir(wPath);
                    pid_t pid;
                    if((pid = fork()) == 0)   //in child
                    {
                        sigprocmask(SIG_UNBLOCK, &mask, NULL);
                        if((fd = open(file_name,O_WRONLY, mode)) == -1){
                            perror("open");
                        }
                        close(0);
                        dup(fd);
                        close(fd);
                        if(execvp(array[0], array) < 0)
                        {
                            printf("%s: ish: Command not found.\n", array[0]);
                            exit(0);
                        }
                    }
                    wait(NULL);
                } 
            }else if(redir == NONE){
                pid_t pid;
                if((pid = fork()) == 0)   //in child
                {
                    sigprocmask(SIG_UNBLOCK, &mask, NULL);
                    if(execvp(array[0], array) < 0)
                    {
                        printf("%s: ish: Command not found.\n", array[0]);
                        exit(0);
                    }
                }
                wait(NULL);
            }

        }else if(strcmp(array[0], "setenv") == 0)
            {
                if(array[1] != NULL){
                    setenv((array[1]), array[2] , 1);
                }
                else{
                    fprintf(stderr, "Error: setenv: No Argument.\n");
                }                        
        }else if(strcmp(array[0], "cd") == 0)
            {
                if(array[1] == NULL)
                {
                    if(chdir("/home") == -1){
                       perror("chdir");
                    }
                } else{
                    if(chdir(array[1]) == -1){
                    perror("chdir");
                    }
                }
        }else if(strcmp(array[0], "unsetenv") == 0)
            {
                if(array[1] != NULL){
                    unsetenv(array[1]);
                    }
                else{
                    fprintf(stderr, "Error: unsetenv: No Argument.\n");
                }
            }
        else if(strcmp(array[0], "exit") == 0)
                {
                    if(array[1] != NULL){
                        fprintf(stderr, "Error: exit: can not have an argument.\n");
                        }
                    else{
                        exit(0);
                    }
                }

}