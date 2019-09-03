//---------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>

#define mStr 256

#define nTools 100

struct mTime {
 int    cnt_tool ; // - общее количество инструментов (=(0||1)-инструмент один)
 int    cnt[nTools] ; // [номер] >0 номер инструмента
 double time_rapid[nTools]; //возвращаемое время движения на rapid_feed (мин)
 double time_base[nTools];  //возвращаемое время - общее -время выполнения УП (мин)
};

int read_word(char *,char ,char *) ;
int  read_str(char *s,char *w,char *sw);
void replace_word(char *,char ,char *) ;
int delete_word(char *,char ) ;
int strqwe(char *s,char *w);

double decodeFeed (double ) ;
long int program_format_H33(char *, int );
long int program_format_H55(char *) ;
double program_time_H33_H55(int ,char *,double, double *, double *, int ) ;
double program_maxfeed_H33_H55(char *, int ) ;

double program_maxfeed(char *namefile) ;


/*

!!!!!!!! 2-ой вариант - расширенный !!!!!!!!

Формат команды инструмента:
 x - номер инструмента (принимаем формат x=00)
 y - номер корректора  (принимаем формат y=00)
 Tx M6
 Txy M6
 Tx.y M6
(первые нули можно убирать)

Вычисляет время выполнения УП

расширенный вариант функции time_cnc 

*/

double time_cnc_ex(int ,char *, double , double  , struct mTime *);

/*
Вычисляет время выполнения УП
Замечание :
    Ошибка вычисления составляет ±2 мин (нет учета разгона-торможения)
    если в УП очень много движений по окружностям - ошибка возрастает
Параметры :
    typeCNC - тип CNC (различие лишь в функц 90,91 и 4(пауза))
              => 0 - standart (размерность=*dim )
                 1 - MTC200 (G4F  размерность=*1 )
                 2 - Альфа ( G4 (TMR, n ) размерность=*1 )
                           ¦ G70 - программирование в микрометрах (мкм);
                           ¦ G71 - программирование в миллиметрах (мм).
                 3 - Siemens (G4F размерность=*1 )
                 40 - HEIDENHAIN размерность=*1
                      G04 F
                 41 - HEIDENHAIN размерность=*1
                      | CYCL DEF 9.0 DWELL TIME
                      | CYCL DEF 9.1 DWELL 1.5
                      | IX-X IY-Y IZ-Z
                 5 - Fanuc, Mazatrol (G4X размерность=*1 )
                 6 - NC110-NC210 (G4 TMR= размерность=*1 )
                 7 - NS720 (G4Q (q/100) размерность=*1 )
                 8 - 2C42 (G4E(e/10) размерность=*0.001 )
                 9 - 2P32 (G4X(x/1000) размерность=*1 )
    namefile - имя файла ,
    rapid_feed - значение ускоренной подачи
    dim     - размерность    ( координата*=размерность)
    *pTIME1 - возвращаемое время - общее -время выполнения УП (мин)
    *pTIME2 - возвращаемое время движения на rapid_feed (мин)
Возвращаемое значение :
    t  -  время выполнения УП (секунды)
    -1 -  Невозможно открыть файл 'чтение'
*/
double time_cnc(int ,char *, double , double , double *,double *);
