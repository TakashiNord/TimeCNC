#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <math.h>
#include <sys/stat.h>

#include <vcl.h>
#pragma hdrstop

#include "TimeCount.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
// �����������
__fastcall TTimeCounter::TTimeCounter(char *file,int A)
{
        f=0;
        TIME=0;
        F4=0;
        a=A;
        sprintf(UP_file,"%s",file);
}

float __fastcall TTimeCounter::count()
{
int ii; char *N000="N000F0624";
inp=fopen(UP_file,"r");
prev=fgets(s_prev,199,inp); i=0; push(prev);
   if(F4==1){curr=N000;F4=0;} else curr=fgets(s_curr,199,inp);
   i++; push(curr);
   if(F4==1){next=N000;F4=0;} else next=fgets(s_next,199,inp);
   i++; push(next);
   if(prev!=NULL)
   do
   {
   TIME=TIME+analiz();
   /* ������ ������� � ������� � ������ ���� ��������
   ����� ��������� ������� ������� analiz */
       for(i=0;i<=1;i++)
       {
       P[i].feed=P[i+1].feed;
       P[i].span=P[i+1].span;
       }
     //if(next==NULL) break;
     if(F4==1){next=N000;F4=0;} else next=fgets(s_next,199,inp);
     if(next==NULL) break;
     push(next);
   }while(1);
 fclose(inp);

/*�������� ���������*/
// ������ ������� - ����� � �����
/*  float  tmpTime , tm2 ;
  long int tm1 ;
  tmpTime =TIME/60.0 ;
  tm1=(int) tmpTime/60.0;
  tm2=(tmpTime/60.0 - tm1) * 60.0 ;

 char mes[100];
 if (tm1==0) {
     sprintf(mes,"����� ���������� -> \n%.2f ������ = %.2f �����",TIME,TIME/60);
  } else {
     sprintf(mes,"����� ���������� -> \n%f ������ = %.2f �����  = [ %d � %.2f ��� ] ",TIME,tmpTime, tm1 ,tm2);
   }
  ShowMessage(mes);
*/
  return (TIME);
}


/* This function read the numeric string after key letter
   in success it return 1, otherwice 0
*/
int __fastcall TTimeCounter::read_word(char *s,char w,char *sw)
{   char *word,sym;
    int  n;

    if ((word=strchr(s,w))==NULL) return(0);
    n=0;
    do
    {  sym=*(word+(n+1));
       if (isdigit(sym)||sym==' '||sym=='+'||sym=='-'||sym=='.') n++;
       else break;
    } while (1);
    if (n>0){ strncpy(sw,word+1,n); sw[n]='\0'; return(n); }
    else                                        return(0);
}

/* ��� ������� ����������� ������ ������� up-�����
   � ����� ����� f */
int __fastcall TTimeCounter::feed_convert1(int ff)
{
int x,y,f;

if(ff>4000) {ff=ff-4000;F4=1;}
x=ff/100; /* x - ��� ����� ���� � ������ ���� 3 */
if(x==4){
	f=(ff-400)/10;
	if(f==0)f=1;
	if(f>9)f=f/10;
	}
else
	{
y=ff-x*100; /* y - ��� ��� ��������� ����� ���� ������ */
x=x-5;
f=y*pow(10,x);
	}

return f;
}



/* ������ ���� �������� ������ prev, curr � next.
   ���������� ����� ��������� ����� curr */

float __fastcall TTimeCounter::analiz(void)
{
float t=0, /* ����� ��������� ����� curr */
      t1,t2,t3,s1,s2,s3;
float Vm; /* ������� ���������� �������� ������ ���
	     ������� ����� */
float V1,V2,V3;
V1=P[i-2].feed/60.0; V2=P[i-1].feed/60.0; V3=P[i].feed/60.0;
/* ���� � ����� ��� �����������, �� ������� 0 */
if(P[i-1].span==0.0) return 0;
if(V2==0.0) return 0;

/* ���� � ����� ������ � ���������� */
if(V2>V1 && V2>V3)
   {
   Vm=sqrt((V1*V1+V3*V3)/2+a*P[i-1].span);
   if(Vm<=V2)
     {
     t1=(Vm-V1)/a; t2=(Vm-V3)/a;
     t=t1+t2; return t;
     }
   t1=(V2-V1)/a; s1=(V2*V2-V1*V1)/2/a;
   t3=(V2-V3)/a; s3=(V2*V2-V3*V3)/2/a;
   s2=P[i-1].span-s1-s3;
   t2=s2/V2;
   t=t1+t2+t3; return t;
   } /* ������ � ���������� */

/* ���� � ����� ������ ������ */
if(V2>V1 && V2<=V3)
   {
   Vm=sqrt(V1*V1+2*a*P[i-1].span);
   if(Vm<=V3)
     {t=(Vm-V1)/a; P[i-1].feed=Vm*60; return t;}
   t1=(V2-V1)/a; s1=(V2*V2-V1*V1)/2/a;
   s2=P[i-1].span-s1;
   t2=s2/V2;
   t=t1+t2; return t;
   } /* ������ ������ */

/* ���� � ����� ������ ���������� */
if(V2<=V1 && V2>V3)
   {
   if((V2*V2-2*a*P[i-1].span)<=0) Vm=0;
   else Vm=sqrt(V2*V2-2*a*P[i-1].span);
   if(Vm>=V3) /* �.�. �� �������� ������������� c V2 �� V3*/
     {t=(V2-Vm)/a; P[i-1].feed=Vm*60; return t;}
   t2=(V2-V3)/a; s2=(V2*V2-V3*V3)/2/a;
   s3=V3*V3/2/a; /* ����������, ����������� ��� ���������� ����������
		    �� 0, ������� ����� �������� �� ����������
		    �������� V3>0 */
   if(V3==0) t3=0;
   else      t3=s3/V3;
   s1=P[i-1].span-s2-s3;
   if(s1>0) t1=s1/V2;
   else t1=0;
   t=t1+t2+t3; return t;
   } /* ������ ���������� */

/* ���� � ����� ��� �� ����������, �� ������� */
return P[i-1].span/V2;
}

/* ��� ������� ��������� � ������� � ����������� � ������
   ����� kadr */
void __fastcall TTimeCounter::push(char *kadr)
{
float x,y,z;
int ff;
int move=0,feed_set=0;
char word_p[50];
     if (read_word(kadr,'X',word_p)) { sscanf(word_p,"%f",&x);
				       move=1; }
     else x=0.0;
     if (read_word(kadr,'Y',word_p)) { sscanf(word_p,"%f",&y);
				       move=1; }
     else y=0.0;
     if (read_word(kadr,'Z',word_p)) { sscanf(word_p,"%f",&z);
				       move=1; }
     else z=0.0;
     if (read_word(kadr,'F',word_p)) { sscanf(word_p,"%d",&ff);
				       f=feed_convert1(ff);
				       feed_set=1; }


/* ��������� � ������� � �� i-�� ������� */
if (feed_set==1) P[i].feed=f;
else if (i==0) P[i].feed=0;
     else if(move==1) P[i].feed=f;
if (move==1) P[i].span=sqrt(x*x+y*y+z*z)/100;
else P[i].span=0;

}

TTimeCounter::~TTimeCounter()
{
        //TODO: Add your source code here
        f=0;
        TIME=0;
        F4=0;
        a=0;
}
