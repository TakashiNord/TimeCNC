//---------------------------------------------------------------------------

#ifndef TimeCountH
#define TimeCountH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TTimeCounter
{
private:
        char UP_file[100];
        int a;            /* ��������� (�� � ���*���) */
        struct post{int feed;    /* ������ */
                    float span;  /* ����������� */
	        }P[3];
        int i;   /* ������ ��� ������� �������� P */
        int f;  /* ������ �������� ����� */
        char s_prev[200],*prev,s_curr[200],*curr,s_next[200],*next;
        FILE *inp;
        float TIME;
        int F4; /* ������� ������ ���������� */
        int  __fastcall feed_convert1(int ff);
        void __fastcall push(char *kadr);
        float __fastcall analiz(void);
        int __fastcall read_word(char *s,char w,char *sw);

public:		// User declarations
        __fastcall TTimeCounter(char *file,int A);
        float __fastcall TTimeCounter::count();
        ~TTimeCounter();
};
//---------------------------------------------------------------------------
extern PACKAGE TTimeCounter *TimeCounter;
//---------------------------------------------------------------------------
#endif



 