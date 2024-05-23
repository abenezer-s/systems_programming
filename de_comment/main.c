#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

#include "DECOMMENT.h"

long  line = 1, count = 0;
int c;

int main(void) {
    enum Statetype state = NORMAL;
    for( ; ; ) {
        c = getchar();
        //printf("state at: %d, count: %ld \n", state, count);
        if( c == EOF && open_com){
            fprintf(stderr, "Error: line%ld: unterminated comment \n", comm);
            //perror("");
            }
       
        if( c == EOF){
            break;
        }
        count++;
        if( c == '/' ){
            //printf("/  \n");
            fsPos = count;
            //printf("fsPos:%ld \n", fsPos); 
            }
        if( c == '*' && state == FS){
            //printf(" * && FS \n");
            asPos = count;
            //printf("asPos:%ld \n", asPos);
            continue;
            }
        if( c == '\n')
            line++;
        if( c == '*' && state == OPAST){
              asPos = count;
              continue;
            }

        switch(state){
            case NORMAL:
                state = handleNormal(c);
                break;
            case FS:
                state = handleFs(c);
                break;
            case COMMENT:
                state = handleComment(c);
                break;
            case OPAST:
                state = handleOpast(c);
                break;
            case QUOTE:
                state = handleQuote(c);
                break;
            case SINGLEQUOTE:
                state = handleSingleQuote(c);
                break;
        }
    }
   return 0;    
}
