#include <stdio.h>
#include <ctype.h>
#include "DECOMMENT.h"

long int fsPos = 0, asPos = 0, comm = 0;
int open_com = 0;

enum Statetype handleComment(int c){
   
    enum Statetype state = COMMENT;
    /*printf("INSIDE handleComment open_com: %d \n", open_com);*/
    if(c == '\n'){
        /*printf("new line");*/
        printf("\n");
        state = COMMENT;
        }
    else if( c == '*'){
        asPos = count;
        /*printf("INSIDE handleComment c = * state is:%d \n", state);*/
        state = OPAST;
        /*printf("INSIDE handleComment c is * state is:%d \n", state);}*/
    /*printf("INSIDE handleComment before RETURN state is:%d \n", state);*/
    /*printf("INSIDE openComment asPos:%ld, fsPos:%ld \n BEFORE ret", asPos, fsPos);*/
    }
    return state;

}

enum Statetype openComment(int c) {
    
    enum Statetype state = FS;
    /*printf("INSIDE openComment c = %d state is:%d \n", c, state);*/
    /*printf("INSIDE openComment asPos:%ld \n fsPos:%ld \n", asPos, fsPos);*/
    if( asPos - fsPos == 1){
        /*printf("INSIDE openComment asPos - fsposm = 1 \n");*/
        open_com = 1;
        /*printf("INSIDE openComment open_com = %d \n", open_com);*/
        state = COMMENT;
        comm = line;
        }
    else state = NORMAL;
    /*printf("INSIDE openComment BEFORE RETURB state = %d \n",state);*/

    return state;
}

enum Statetype closeComment(int c) {
    enum Statetype state = OPAST;
    
    if( c == '/') {
        fsPos = count;
        /*printf("INSIDE closeComment fsposm = %ld, asPos = %ld \n", count, asPos);*/
        if( fsPos - asPos == 1) {
            open_com = 0;
            /*printf("INSIDE closeComment fsPos - asPos == 1 \n fsposm = %ld, asPos = %ld \n open_com = %d", count, asPos, open_com );*/
            state = NORMAL;
            }
        else state = COMMENT;
    }
    
    return state;
}

enum Statetype handleFs(int c){
    enum Statetype state = FS;

    //printf("INSIDE handleFs c = %d state is:%d \n", c, state);
    state = openComment(c);
    //printf("INSIDE handleFs state is %d AFTER OPENCOM \n", state);

    if( state != COMMENT){
        //printf("INSIDE handleFs state is NOT COMMENt%d \n", state);
        open_com = 0;
        putchar(c);
    } 
    else {open_com = 1;}

    return state;
}

long quoPos = 0,sinQuoPos = 0;;

enum Statetype handleNormal(int c){
    enum Statetype state;
    ////printf("INSIDE handleNormal open_com is %d\n", open_com);
    if(c == '/' && !open_com ) {
        //printf("INSIDE handleNormal c = / and open_com: %d\n", open_com);
        state = FS;
        //printf("INSIDE handleNormal c = / state is: %d \n", state);
    }
    else if (c == '\"') {
        ////printf("INSIDE handleNormal c = \" ");
        putchar(c);
        quoPos = line;
        state = QUOTE;
        ////printf("INSIDE handleNormal c = \" state is: %d \n", state);
    }
    else if (c == '\'') { 
        ////printf("INSIDE handleNormal c = \' ");
        putchar(c);
        sinQuoPos = line;
        state = SINGLEQUOTE;
        ////printf("INSIDE handleNormal c = \' state is: %d \n", state);
    }
    else {
        putchar(c); 
        state = NORMAL;
        open_com = 0; 
        ////printf("INSIDE handleNormal evythingNORMAL state is: %d \n", state);
        }

    return state;
}

enum Statetype handleOpast(int c){
    enum Statetype state = OPAST;
    
    state = closeComment(c);

    return state;
}

int open_quo = 0;

enum Statetype handleQuote(int c){
    enum Statetype state;
    if (open_quo && (c == '\"')){
        open_quo = 0;
        putchar(c);
        state = NORMAL;
        }
    else {
        open_quo = 1; putchar(c); state = QUOTE;
        }    
    //printf("IN handlequote state is %d before return \n", state);
    return state;

}

int open_sinquo = 0;

enum Statetype handleSingleQuote(int c){
    enum Statetype state;
    if (open_sinquo && (c == '\'')){
        open_sinquo = 0;
        putchar(c);
        state = NORMAL;
        }
    else {
        open_sinquo = 1; putchar(c); state = SINGLEQUOTE;
        
        }    
    return state;

}