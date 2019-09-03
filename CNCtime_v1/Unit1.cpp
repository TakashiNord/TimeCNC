//---------------------------------------------------------------------------
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
#include "TimeCount.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------

/* This function read the numeric string after key letter   in success it return 1, otherwice 0*/
 int read_word(char *s,char w,char *sw)
{   char *word;    char sym;    int  n;
    if ((word=strchr(s,w))==NULL) return(0);
    n=0;
    do
    {  sym=*(word+(n+1));
       if (isdigit(sym)||sym=='+'||sym=='-'||sym=='.') n++; else break;
    } while (1);
    if (n>0){ strncpy(sw,word+1,n); sw[n]='\0'; return(n); } 
	else { 
        sw[0]='0';
		sw[1]='\0';
		return(1);
	}
}

void __fastcall TForm1::Button1Click(TObject *Sender)
{
  char sp1[128];
  char word[30];
  char *jp;
  long int countAdr,countStr,i;
  int flgoto ; //флаг начала программы -%
  int flsign ; //флаг - комментарий ; ( )
  double feed_rate,feed , prev_feed;
  double pos[3],prev_pos[3];
  double motion_distance;
  double TIME , TIMEacc ;
  double tmp ;
  const int mStr=128;

  char *s;
  char word_p[70], s1[20];
  int n,num;
  char sp2[128];
  char *jpd,*jpdg;
  char *ptr;
  char save[128],*p;
  double gcode ;

  AnsiString sfile;

/*AnsiString   sf5; sf5 = FilterComboBox1->Mask;
  Application->MessageBox(sf5.c_str() ,"Файл......", MB_OK);
  */

  /* Условие показа листинга программы */
  if (MTC200->Checked==True) {
  /* Indramat */
  feed_rate=abs(atof(EditRapid->Text.c_str()));
  if (feed_rate<=1000.0) feed_rate=10000.0 ;
  EditRapid->Text=feed_rate;

  sfile=FileListBox1->FileName;
  //Application->MessageBox(sfile.c_str() ,"Файл......", MB_OK);

  FILE *fin;
  fin=fopen(sfile.c_str(),"r");
  if (fin==NULL) {
    Application->MessageBox("Can't open file \n " ,"Файл......", MB_OK);
    return;
   }

  for(i=0;i<3;i++) {
    pos[i]=0.0;
    prev_pos[i]=0.0;
  }
  feed=feed_rate;
  motion_distance=0.0;
  gcode=0;
  TIME=0.0; TIMEacc=0.0;

  countAdr=0;
  sp1[0]='\0';
  jp=fgets(sp1,mStr,fin);
  countStr=0;
  flgoto=0 ;
  while(jp!=NULL)
  {
    /*Если строка не комментарий  ; ( )*/
    flsign=0;
    if (!((NULL==strchr(jp,';'))&&(NULL==strchr(jp,'('))&&(NULL==strchr(jp,')')))) { flsign=1; }

     /*Поиск начала программы для запуска преобразования*/
     if(flgoto==0) {
        if(NULL!=strchr(jp,'%')) { flgoto=1 ; }
     }

     if (flgoto!=0) {
       countStr++;
       /*Кадры с G[0,1,2,3] XYZ F IJK*/
	if (flsign==0) {

  /*Кадры с G*/
  if (read_word(sp1,'G',word_p))  {
   strcpy(sp2,jp);
   jpd=strchr(sp2,'G');
   if(jpd!=NULL) {
     while(strcmp(jpd,""))
     {
      if (read_word(jpd,'G',word))
      { sscanf(word,"%lf",&gcode);
        //printf(" Gf==%f Gs=%s |" ,gcode,word_p);
        if ((gcode==2)||(gcode==3)) { ; }
        if (gcode==0) { feed=feed_rate; break; }
        if (gcode==1) { break; }
        if (gcode==4) { break; }
      }
     jpdg=strchr(jpd,'G');
     if(jpdg==NULL) strcpy(jpd,"");else strcpy(jpd,jpdg+1);
     }
   }
  }

           // текущая точка
           if (read_word(sp1,'X',word)) { sscanf(word,"%lf",&pos[0]); }
           if (read_word(sp1,'Y',word)) { sscanf(word,"%lf",&pos[1]); }
           if (read_word(sp1,'Z',word)) { sscanf(word,"%lf",&pos[2]); }
           prev_feed=feed;
           if (read_word(sp1,'F',word)) { sscanf(word,"%lf",&feed); }
           for(tmp=0.0,i=0;i<3;i++) {
             tmp+=pow(pos[i]-prev_pos[i],2);
             prev_pos[i]=pos[i];
            }
           motion_distance+=sqrt(tmp) ;
           if (feed<=0.0) feed=prev_feed;
           if (feed<=0.0) feed=feed_rate;
           if (feed>=feed_rate) { TIMEacc+=sqrt(tmp)/feed ; }
           TIME+=sqrt(tmp)/feed ; //(feed*60.0);  feed
 	}

     }/*if (flgoto!=0)*/

    jp= fgets(sp1,mStr,fin);

   }//end of while(fl==0)

   //countAdr;
  fclose(fin);
  //TIME+= 0.1;
  sprintf(sp1,"Файл = %s \n Общее время=%-10.4f мин \n..... из них на ускоренной подаче=%-10.2f мин",sfile.c_str(),TIME,TIMEacc);
  Application->MessageBox(sp1 ,"Файл......", MB_OK);
  } else {

 /* H33 */
  feed_rate=abs(atof(EditRapid->Text.c_str()));
  if (feed_rate<30.0) feed_rate=1200.0 ;
  EditRapid->Text=feed_rate;

  int a=abs(atoi(acc->Text.c_str()));
  if (a<4) a=50 ;
  acc->Text=a;
 
 // Время выполнения
 sfile=FileListBox1->FileName;
 TTimeCounter *tc=new TTimeCounter(sfile.c_str(),a);
 float  TIME1; char mes[400] ;
 TIME1=tc->count();

        // Расчет времени - минут и часов
        float  tmpTime , tm2;
        long int tm1 ;
        tmpTime =TIME1/60.0 ;
        tm1=(int) tmpTime/60.0;
        tm2=(tmpTime/60.0 - tm1) * 60.0 ;
        if (tm1==0) {
           sprintf(mes,"\nВремя выполнения ->\n %.2f секунд = %.2f минут",TIME1,TIME1/60);
         } else {
            sprintf(mes,"\nВремя выполнения ->\n %.2f секунд = %.2f минут = [ %d ч %.1f мин ]",TIME1,tmpTime, tm1 ,tm2);
        }

  tc->TTimeCounter::~TTimeCounter(); // деструктор
  MessageDlg(mes, mtConfirmation, TMsgDlgButtons() << mbOK , 0);
  }


}
//---------------------------------------------------------------------------
void __fastcall TForm1::accChange(TObject *Sender)
{
       int tmp ;
       tmp=abs(atoi(acc->Text.c_str()));
       if (tmp==0) tmp=5 ;
       acc->Text=tmp;
}
//---------------------------------------------------------------------------


void __fastcall TForm1::EditRapidChange(TObject *Sender)
{
       long int tmp ;
       tmp=abs(atoi(EditRapid->Text.c_str()));
       if (tmp==0) tmp=600 ;
       EditRapid->Text=tmp;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::MTC200Click(TObject *Sender)
{
       EditRapid->Text="10000.0" ;
       acc->Text="35";
}
//---------------------------------------------------------------------------

void __fastcall TForm1::H33Click(TObject *Sender)
{
       EditRapid->Text="1200.0" ;
       acc->Text="50";
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FileListBox1Change(TObject *Sender)
{
  char sp1[128], *jp ;
  const int mStr =128 ;
  long int i ,j;

 if (CheckBox1->Checked==False) return ;

 for (i = 0; i < FileListBox1->Items->Count; i++)

{
  if (FileListBox1->Selected[i])
  {
    if (!FileExists(FileListBox1->Items->Strings[i]))
    {
      MessageBeep(0);
      if (Application->MessageBox("Can't open file... ",NULL, MB_OKCANCEL | MB_DEFBUTTON1) == IDCANCEL)
        break;
      else
        continue;
    }
 AnsiString   sf5; sf5 = FilterComboBox1->Mask;
 //Application->MessageBox(sf5.c_str() ,"Файл......", MB_OK);
 if (sf5=="*.*") {
  } else {
  Memo1->Clear();
  FILE *F = fopen(FileListBox1->Items->Strings[i].c_str(),"r");
  sp1[0]='\0';
  jp=fgets(sp1,mStr,F);
  j=0;
  while(jp!=NULL)
  {
    Memo1->Lines->Strings[j]=sp1;
    j++;
    jp= fgets(sp1,mStr,F);
  }//end of while(fl==0)
  fclose(F);
  Memo1->Update();
   }
  }
 }

}
//---------------------------------------------------------------------------

void __fastcall TForm1::FilterComboBox1Change(TObject *Sender)
{
 AnsiString   sf5; sf5 = FilterComboBox1->Mask;
 //Application->MessageBox(sf5.c_str() ,"Файл......", MB_OK);
/* if (sf5=="*.*") { Memo1->Visible=False; } else { Memo1->Visible=True; } */
 
}
//---------------------------------------------------------------------------


void __fastcall TForm1::CheckBox1Click(TObject *Sender)
{
  if (CheckBox1->Checked==False) { Memo1->Clear(); }
  FileListBox1Change(Sender);
}
//---------------------------------------------------------------------------

