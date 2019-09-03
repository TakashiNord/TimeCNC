//---------------------------------------------------------------------------

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <math.h>

#ifndef TimeCountH
#define TimeCountH
//---------------------------------------------------------------------------
class TTimeCounter
{
private:
        char UP_file[128];
        int a;            /* Ускорение (мм в сек*сек) */
        struct post{int feed;    /* подача */
                    float span;  /* перемещение */
	        }P[3];
        int i;   /* Индекс для массива структур P */
        int f;  /* Подача текущего кадра */
        char s_prev[200],*prev,s_curr[200],*curr,s_next[200],*next;
        FILE *inp;
        float TIME;
        int F4; /* Признак режима торможения */
        //int  feed_convert1x(int ff);
        int  feed_convert1(int ff);
        void push(char *kadr);
        float analiz(void);
        int read_word(char *s,char w,char *sw);

public:		// User declarations
        TTimeCounter();
        float TIME1; // общее время
        float TIME2; // время на макс подаче
        float count();
        int maxflag;
        int maxf;
        void  start(char *file,int A, int maxf);
        ~TTimeCounter();
};

#endif



 