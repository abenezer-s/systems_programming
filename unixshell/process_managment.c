#define _GNU_SOURCE
#include <signal.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/wait.h>
#include <errno.h>
#include <sys/types.h>
#include "dynarray.h"
#include "process_managment.h"
#include "synt_analysis.h"

#define MAX_PIPE 128

FILE *Fp;   //*Temp;
char StrBuff[MAX_LINE_SIZE];
int NumberOfArgs, strI = 0, iRead = 0, cArg = 0; // the number args in a line

int Sizes[MAX_PIPE]; // holds all the diffrent sizes for a given line that contains pipes
int CurrentSize = 0; // current index into Sizes 

char PathToCmd[(MAX_LINE_SIZE * 2)] = "/bin/";
bool pipeExists = false;
bool promted = false;
bool EOFreached = false;
bool convertedFd = false; // make sure .isrc is only converted once


typedef struct Commands *commands_t;

struct Commands
{
    char *array[MAX_ARGUMENT];
    int nArgs;
    commands_t next;
};

struct Arguments {
    bool firstCmd;
    bool midCmd;
    bool lastCmd; 
    int readSide; 
    int writeSide; 
    commands_t cur_com; 
    int pipeEnds; 
    int *p;
};

commands_t create_struct(int size)
{// create a structure where upto oCommands->array[size] are allocated 
    commands_t oCommands = (commands_t)malloc(sizeof(struct Commands));
    if(oCommands == NULL)
        {
            printf("malloc: coudlnt alloate mem\n");
            exit(1);
        }
    int n = size;
    // allocate for n array members
    int i;
    for(i = 0; i < n; i++)
    {
        oCommands->array[i] = malloc(MAX_LINE_SIZE * sizeof(char));
        if(oCommands->array[i] == NULL){
            printf("malloc: coudlnt alloate mem\n");
            exit(1);
        }
    }
    return oCommands;
}


int get_num_args(tokenList_t tokenList)
{
    token_t cur_token;
    int nArgs = 0;
    for(cur_token = tokenList->tokens;
        cur_token != NULL;
        cur_token = cur_token->next){
            if(cur_token->type == ARG){
                nArgs++;
            }
        }
        return nArgs;
}

int initialize_struct(commands_t cur_com, commands_t commands_list, tokenList_t tokenList){
    //initialize structure 
    cur_com = commands_list;
    int num_args_new = 0;
    token_t cur_token, next_token;
    int i = 0;
    for(cur_token = tokenList->tokens;
        cur_token != NULL;
        cur_token = next_token)
    {
        if(cur_token->type != PIPE){
            strcpy(cur_com->array[i++], cur_token->val);
            num_args_new++;
            next_token = cur_token->next;
 
         }else{
            
            cur_com->array[i] = NULL;
            cur_com->nArgs = num_args_new;
            num_args_new = 0; i = 0;
            cur_com = cur_com->next;
            next_token = cur_token->next;
        }
    }
    cur_com->array[i] = NULL;
    cur_com->nArgs = num_args_new;
    
    return 1;
}
void handle_child_proc(struct Arguments arguments){
    bool firstCmd = arguments.firstCmd; 
    bool midCmd = arguments.midCmd ;
    bool lastCmd = arguments.lastCmd ;
    int readSide = arguments.readSide ;
    int writeSide = arguments.writeSide; 
    commands_t cur_com = arguments.cur_com ;
    int *p = arguments.p ;
    int pipeEnds = arguments.pipeEnds ;
    //arrange pipes
    if(firstCmd)
    {
        if(dup2(p[writeSide], 1) < 0){
            perror("dup2");
            exit(EXIT_FAILURE);
        }
        //close all
        for(int i = 0; i < pipeEnds; i++){
            close(p[i]);
        }
        //execute
        if(execvp(cur_com->array[0], cur_com->array) < 0){
            perror(cur_com->array[0]);
            exit(EXIT_FAILURE);
        }                
    } else if(midCmd)
        {
            dup2(p[readSide], 0);
            dup2(p[writeSide], 1);
            //close all
            for(int i = 0; i < pipeEnds; i++){
                close(p[i]);
            }
            //execute
            if(execvp(cur_com->array[0], cur_com->array) < 0){
                perror(cur_com->array[0]);
                exit(EXIT_FAILURE);
            }                    
    } else if(lastCmd)
        {
            dup2(p[readSide], 0);
            //close all
            for(int i = 0; i < pipeEnds; i++){
                close(p[i]);
            }
            //execute
            if(execvp(cur_com->array[0], cur_com->array) < 0){
                perror(cur_com->array[0]);
                exit(EXIT_FAILURE);
            }
        }

}
void handle_pipe(tokenList_t tokenList, int NumPipes, sigset_t mask)
{
    //creat an empty linked list of commands 
    int NumPipe = NumPipes;
    int num_commands = NumPipe + 1;
    int i, j;
    int NumArg = get_num_args(tokenList);

    commands_t cur_com, prev_com, commands_list = NULL;
    int size = (NumArg + 1);
    for(i = 0; i < num_commands; i++)
    {   
        cur_com = create_struct(size);
        cur_com->next = NULL;

        if(commands_list == NULL){
            commands_list = cur_com;
            prev_com = cur_com;
        }else
            {
                prev_com->next = cur_com;
                prev_com = cur_com;
            }
    }
    //initialize structure 
    initialize_struct(cur_com, commands_list, tokenList);
    //execute each command
    int pipeEnds = (NumPipe * 2);
    int p[pipeEnds];
    //create pipes
    for(i = 0; i < NumPipe; i++ )
    {
        if(pipe(p + (i * 2)) < 0){
            perror("pipe");
            exit(EXIT_FAILURE);
        }
    }
    int readSide = 0, writeSide = 1; // pipe end's numbers
    bool firstCmd = true, midCmd = false, lastCmd = false;
    int executed = 0;
    pid_t pid;
    for(cur_com = commands_list; 
        cur_com != NULL;
        cur_com = cur_com->next)
    {
        if((pid = fork()) == 0)
        {// child process
            //unblock SIGINT
            sigprocmask(SIG_UNBLOCK, &mask, NULL);
            //set up arguments
            struct Arguments arguments;
            arguments.firstCmd = firstCmd;
            arguments.midCmd = midCmd;
            arguments.lastCmd = lastCmd;
            arguments.readSide = readSide;
            arguments.writeSide = writeSide;
            arguments.cur_com = cur_com;
            arguments.p = p;
            arguments.pipeEnds = pipeEnds;
            
            handle_child_proc(arguments);
                
        } else if(pid < 0){
                perror("error");
                exit(EXIT_FAILURE);
            }
            //parent,handle parent process
            executed++;
            if(firstCmd){
                firstCmd = false;
            }
            
            if(NumPipe != 1)
            {
                if(NumPipe == executed){
                    midCmd = false;
                    lastCmd = true;  
                } else{
                    midCmd = true;
                }
            } else{
                lastCmd = true;
            }
            // update pipe ends
            if(midCmd){
                writeSide += 2;
            }
            if(lastCmd && (NumPipe > 1)){

                readSide += 2;
            }
    }
    //close all fd from parent
    for(i = 0; i < pipeEnds; i++){
        close(p[i]);
    }
    //wait
   for(i = 0; i < (NumPipe + 1); i++)
   {
        wait(NULL);
   }
    //free commands_list and arrays
    commands_t nextCom;
    for(cur_com = commands_list; 
        cur_com != NULL;
        cur_com = nextCom)
        {
            for(i = 0; i < size; i++){
                free(cur_com->array[i]);
            }
            nextCom = cur_com->next;
            free(cur_com);
        }
    NumPipe = 0;
 return;
}

char *cleanString(char *oString)
{
    char *cleanString = oString; 
    // Remove any non-printable characters from the cleanString
    int i, j;
    for (i = 0, j = 0; cleanString[i] != '\0'; i++) {
        if (isprint(cleanString[i])) {
            cleanString[j++] = cleanString[i];
        }
    }
    cleanString[j] = '\0';
    //can use the cleaned cleanString for further processing
    return cleanString;
}

void executeLines(int fd, FILE **fpp, sigset_t mask) // recusive function execute every command(Line) in a file
{
    char c;
    char *cleanedString;
    int NumPipe;

    iRead = 0, cArg = 0;
    //execute comanads from file
    if(fd != 0)
    {
        //convert fd to fp
        Fp = fdopen(fd, "r");
        *fpp = Fp; 
    }
    tokenList_t tokenList;
    for(;;) //
    {
        c = fgetc(Fp);
        if(c !='\n' && c != EOF)
        {
            //only write to Temp prinatbale chararcters
            if(isprint(c))
            {         
                StrBuff[strI++] = c;   
            }
        } else if(c == '\n')
            {   
                //printf("\n");
                if(strI != 0){
                    StrBuff[strI] = '\0';
                    strI = 0;
                    //print comand
                    printf("%% %s\n", StrBuff);
                    tokenList = lex(StrBuff, &NumPipe);
                    synt(tokenList, NumPipe, mask);   
                } else{
                    continue;
                }
            } else if(c == EOF){
                break;
            }
    }
    return;  
}