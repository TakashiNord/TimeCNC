//---------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>

#define mStr 256

#define nTools 100

struct mTime {
 int    cnt_tool ; // - ����� ���������� ������������ (=(0||1)-���������� ����)
 int    cnt[nTools] ; // [�����] >0 ����� �����������
 double time_rapid[nTools]; //������������ ����� �������� �� rapid_feed (���)
 double time_base[nTools];  //������������ ����� - ����� -����� ���������� �� (���)
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

!!!!!!!! 2-�� ������� - ����������� !!!!!!!!

������ ������� �����������:
 x - ����� ����������� (��������� ������ x=00)
 y - ����� ����������  (��������� ������ y=00)
 Tx M6
 Txy M6
 Tx.y M6
(������ ���� ����� �������)

��������� ����� ���������� ��

����������� ������� ������� time_cnc 

*/

double time_cnc_ex(int ,char *, double , double  , struct mTime *);

/*
��������� ����� ���������� ��
��������� :
    ������ ���������� ���������� �2 ��� (��� ����� �������-����������)
    ���� � �� ����� ����� �������� �� ����������� - ������ ����������
��������� :
    typeCNC - ��� CNC (�������� ���� � ����� 90,91 � 4(�����))
              => 0 - standart (�����������=*dim )
                 1 - MTC200 (G4F  �����������=*1 )
                 2 - ����� ( G4 (TMR, n ) �����������=*1 )
                           � G70 - ���������������� � ����������� (���);
                           � G71 - ���������������� � ����������� (��).
                 3 - Siemens (G4F �����������=*1 )
                 40 - HEIDENHAIN �����������=*1
                      G04 F
                 41 - HEIDENHAIN �����������=*1
                      | CYCL DEF 9.0 DWELL TIME
                      | CYCL DEF 9.1 DWELL 1.5
                      | IX-X IY-Y IZ-Z
                 5 - Fanuc, Mazatrol (G4X �����������=*1 )
                 6 - NC110-NC210 (G4 TMR= �����������=*1 )
                 7 - NS720 (G4Q (q/100) �����������=*1 )
                 8 - 2C42 (G4E(e/10) �����������=*0.001 )
                 9 - 2P32 (G4X(x/1000) �����������=*1 )
    namefile - ��� ����� ,
    rapid_feed - �������� ���������� ������
    dim     - �����������    ( ����������*=�����������)
    *pTIME1 - ������������ ����� - ����� -����� ���������� �� (���)
    *pTIME2 - ������������ ����� �������� �� rapid_feed (���)
������������ �������� :
    t  -  ����� ���������� �� (�������)
    -1 -  ���������� ������� ���� '������'
*/
double time_cnc(int ,char *, double , double , double *,double *);
