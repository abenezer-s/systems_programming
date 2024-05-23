#ifndef DECOMMENT_H
#define DECOMMENT_H

    enum Statetype {NORMAL, FS, COMMENT, OPAST, QUOTE, SINGLEQUOTE};
    extern long line;
    extern long count;
    extern int c;
    extern int state;

    #include <stdio.h>

    extern long fsPos;
    extern long asPos;
    extern int open_com;
    extern long comm;

    enum Statetype handleComment(int c);
    enum Statetype openComment(int c);
    enum Statetype closeComment(int c);

    enum Statetype handleFs(int c);

    enum Statetype handleNormal(int c);
    extern long quoPos;
    extern long sinQuoPos;

    enum Statetype handleOpast(int c);

    extern int open_quo;
    enum Statetype handleQuote(int c);

    extern int open_sinquo;
    enum Statetype handleSingleQuote(int c);

#endif