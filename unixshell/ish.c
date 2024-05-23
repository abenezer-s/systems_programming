// complete the ishrc requirments

#define _GNU_SOURCE
#include <signal.h>
#include "dynarray.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>
#include "process_managment.h"
#include "lex_analysis.h"
#include "synt_analysis.h"

#define MAX_LINE_SIZE 1024

FILE *temp_file = NULL;

// take input from stdin if provided else from .isrc file and execute commands
char line_buff[MAX_LINE_SIZE];
char c;
bool readable_size;
size_t uiRead = 0;
bool interactive = false;


//sig ahngdeler for ctl-/
int sig_nums = 0;
void sig_quit(int sig)
{
    if(sig == SIGQUIT){
        sig_nums++;
        if(sig_nums == 2){
            exit(0);
        }else{
            printf("\nType Ctrl-\\ again within 5 seconds to exit. \n");
            alarm(5);
        }
    }

}
void sig_hnd(int sig){
    sig_nums = 0;
    printf("exit canceled\n");
    printf("%% ");
    return;
}

FILE *open_temp(char *mode)
{
    FILE *fp;
    char *home = getenv("HOME");    
    if (home != NULL) 
    {
        char filepath[MAX_LINE_SIZE];
        snprintf(filepath, MAX_LINE_SIZE, "%s/ish_temp", home);
        
       if(strcmp(mode, "r") == 0)
       {// open for reading    
            fp = fopen(filepath, "r");
            if(fp == NULL)
            {
                perror("fopen");
            }
        }else if(strcmp(mode, "w") == 0){
            //open for writing
            fp = fopen(filepath, "w");
            if(fp == NULL)
            {
                printf("OPENISHRC: couldnt open file\n");
            }
        } else{
            fprintf(stderr, "openTemp: wrong mode. use \"w\" or \"r\"\n");
            return NULL;
        }
    }
    return fp;
}
// read from stdin
bool read_stdin(FILE **fpp){

    temp_file = open_temp("w");
    setbuf(temp_file, NULL);
    if(temp_file == NULL)
    {
        printf("couldnt open ish_temp\n");
    }
    uiRead = 0;
    for(;;)
    {
        c = fgetc(stdin);// wait for input
        if(c != EOF && c != '\n' && c != '\0')
        {
            fputc(c, temp_file);
            uiRead++;
            if(uiRead > (MAX_LINE_SIZE - 1))
            {
                // continue to read until to EOF or \n
                for(;;)
                {
                    c = fgetc(stdin);
                    if(c == EOF || c == '\n')
                    {
                        printf("Maximum(1023) characters reached. \n");
                        printf("%% ");
                        fflush(stdout);
                        fclose(temp_file);
                        printf("else stdin: tempfiel closed");
                        return false;
                    }
                }
            }
        } else if( c == '\n')
            {
                if(uiRead != 0)
                {// not empty
                    fclose(temp_file);
                    return true;
                 } else
                    {
                        fclose(temp_file);
                        return false;
                    }
            } else if(c == EOF)
                {
                    if(uiRead != 0)
                    {// not empty
                        fclose(temp_file);
                        return true;
                    } else if(interactive)
                        {
                            printf("\n");
                            fclose(temp_file);
                            free(fpp);
                            exit(0);
                        }
                    else{   
                            fclose(temp_file); 
                            return false;
                        }
                }
    }
}

//read from file
bool read_usr_file(char *string){

    int i;
    for(i = 0;;i++)
        {
            c = string[i];
            if(c != '\0')
            {
               // printf("ARGV[0][%] = %c\n",i, c);
                if(uiRead > (MAX_LINE_SIZE - 1))
                {
                    printf("file not readable: file path too long\n");
                    printf("%% ");
                    fflush(stdout);
                    if ( temp_file != NULL) {
                        printf("read_file: tempfiel closed");

                        fclose(temp_file);
                    }
                    return false;
                }
                uiRead++; 
            } else
                {
                    if (temp_file != NULL) {
                        printf("read_file: tempfiel closed");
                        fclose(temp_file);
                    }
                    return true;
                }

        }
}

bool get_input(char *string,  FILE **fpp)
{
    //return true if string is < 1024 characters. false otherwise. 
    // if strng NULL then read from stdin
        int i;
        //uiRead
        if(string != NULL) //read from file given by user
        {
            return read_usr_file(string);
              
        } else // read from stdin
            {   
                return read_stdin(fpp);
            }
}

int open_ishrc(void)
{ //reutrn file descripter for .ishrc config file
    int fd;
    char *home = getenv("HOME");    
    if (home != NULL) 
    {
        char filepath[MAX_LINE_SIZE];
        snprintf(filepath, MAX_LINE_SIZE, "%s/.ishrc", home);
        fd = open(filepath, O_RDONLY);
        
        if(fd == -1)
        {
            return -1;
        }
    }
    return fd;
}

bool initialize(int arg_nums, char *argv, FILE **fpp, sigset_t mask){
    bool readable;
    
    int fd, argc = arg_nums;
    
    if(argc > 1)
        {
            // get filename from cmdline
            // size needs to be < 1024
    
            bool readableSize = get_input(argv, fpp);
            // check if file exists
            if((fd = open(argv, O_RDONLY)) > 0)
            {
                if(readableSize)
                {
                    readable = true;
                    close(fd);
                } else
                    readable = false;
            }
            // if file is readable open and intreprate lines
            if(readable)
            {   
                fd = open(argv, O_RDONLY);
                //interprate fd
                executeLines(fd, fpp, mask);
                if((close(fd)) == -1){
                   perror("close");
                }   
            }
            return true;       
        } else
            {
                //open .ishrc
                fd = open_ishrc();
                    if(fd != -1)
                    {
                        readable = 1;
                    } else
                        readable = 0;
    
                    if(readable)
                    {
                        //interprate fd
                        executeLines(fd, fpp, mask);
                        if((close(fd)) == -1){
                           perror("close");
                        }
                        FILE *fp = *fpp;
                        if((fclose(fp)) == EOF){
                           perror("fclose");
                        }        
                    }    
                return true;
            }
}

void main(int argc, char *argv[])
{
    FILE **fpp = malloc(sizeof(FILE **));
    size_t uiRead = 0;
    sigset_t mask, sSet;

    int initialized = false;
    
    setvbuf(stdout, NULL, _IONBF, 0);

    //unblock SIGALRM
    sigemptyset(&sSet);
    sigaddset(&sSet, SIGALRM);
    sigprocmask(SIG_UNBLOCK, &sSet, NULL);

    //block SIGINT
    sigemptyset(&mask);
    sigaddset(&mask, SIGINT);
    sigprocmask(SIG_BLOCK, &mask, NULL);

    //install handlers
    signal(SIGALRM, sig_hnd);
    signal(SIGQUIT, sig_quit);

    //intialization
    while(!initialized)
    {
        // initialize
        initialized = initialize(argc, argv[1], fpp, mask);
        
    } 

    printf("%% ");
    fflush(stdout);
    //interactive phase, get input fom stdin and lexically analyze
    interactive = true;
    tokenList_t tokenList;
    int NumPipe;
    
    for(;;)
    {

        readable_size = get_input(NULL, fpp); // get input and also check the size of a commandline input

        temp_file = open_temp("r");// open to read from ishLineTemp
        if( temp_file == NULL)
        {
            printf("main: couldnt open file\n");
        }

        if(readable_size)
        {
            
            //copy the contents of lineTemp to lineBuff
            if(fgets(line_buff, MAX_LINE_SIZE, temp_file) != NULL)
            {
                //printf("lineBuf: %s\n", lineBuff);
                tokenList = lex(line_buff, &NumPipe);
                //syntatically analyze tokenList n xecute
                synt(tokenList, NumPipe, mask);
            }
            fclose(temp_file);
            
            printf("%% ");
            fflush(stdout);
        }else  
            {
                
                printf("%% ");
                fflush(stdout);
                fclose(temp_file);
            }
        
    }
}
