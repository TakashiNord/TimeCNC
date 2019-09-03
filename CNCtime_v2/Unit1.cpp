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
  FILE *fin;
  char sp1[255],sp2[128];
  char *jpd,*jpdg;
  double gcode ;
  char word[30],word_p[70];
  char *jp;
  long int countAdr,countStr,i;
  int flgoto ; //флаг начала программы -%
  int flsign ; //флаг - комментарий ; ( )
  double feed , prev_feed;
  double pos[3],prev_pos[3];
  double motion_distance;
  double TIME , TIMEacc ;
  double tmp ;
  const int mStr=128;

  AnsiString sfile;
  double feed_rate;
  long int acc ;

  /* Условие показа листинга программы */
  if (RadioGroupMETHODS->ItemIndex==0) {
   /* Indramat-MTC200*/
   feed_rate=fabs(atof(ComboBoxRAPID->Text.c_str()));
   if (tmp<=0) {
     feed_rate=12000.0 ;
     ComboBoxRAPID->Text=feed_rate;
     MessageDlg("Уточните значение ускоренного перемещения...", mtWarning, TMsgDlgButtons() << mbOK , 0);
     return ;
   }

   acc=abs(atoi(ComboBoxACC->Text.c_str()));
   if (acc<=0) {
     acc=100 ;
     ComboBoxACC->Text=acc;
     MessageDlg("Уточните значение ускорения...", mtWarning, TMsgDlgButtons() << mbOK , 0);
     return ;
   }

  sfile=FileListBox1->FileName;
  //Application->MessageBox(sfile.c_str() ,"Файл......", MB_OK);

  fin=fopen(sfile.c_str(),"r");
  if (fin==NULL) {
    sp1[0]='\0';
    sprintf(sp1,"Невозможно открыть файл\n %s \n уточните... ",sfile.c_str());
    MessageDlg(sp1, mtError, TMsgDlgButtons() << mbOK , 0);
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
  sp1[0]='\0';
  sprintf(sp1,"Файл = %s \n Общее время=%-10.4f мин \n..... из них на ускоренной подаче=%-10.2f мин",sfile.c_str(),TIME,TIMEacc);
  Application->MessageBox(sp1 ,"Файл......", MB_OK);
  }

 if (RadioGroupMETHODS->ItemIndex==1) {
    /* H33 */
    feed_rate=fabs(atof(ComboBoxRAPID->Text.c_str()));
    if (tmp<=0) {
      feed_rate=12000.0 ;
      ComboBoxRAPID->Text=feed_rate;
      MessageDlg("Уточните значение ускоренного перемещения...", mtWarning, TMsgDlgButtons() << mbOK , 0);
      return ;
    }

    acc=abs(atoi(ComboBoxACC->Text.c_str()));
    if (acc<=0) {
      acc=100 ;
      ComboBoxACC->Text=acc;
      MessageDlg("Уточните значение ускорения...", mtWarning, TMsgDlgButtons() << mbOK , 0);
      return ;
    }

    sfile=FileListBox1->FileName;
    //Application->MessageBox(sfile.c_str() ,"Файл......", MB_OK);


    fin=fopen(sfile.c_str(),"r");
    if (fin==NULL) {
      sp1[0]='\0';
      sprintf(sp1,"Невозможно открыть файл\n %s \n уточните... ",sfile.c_str());
      MessageDlg(sp1, mtError, TMsgDlgButtons() << mbOK , 0);
      return;
    }
    fclose(fin);

    // Время выполнения
    TTimeCounter *tc=new TTimeCounter(sfile.c_str(),acc);
    TIME=(double) tc->count();
    tc->TTimeCounter::~TTimeCounter(); // деструктор

    // Расчет времени - минут и часов
    float  tmpTime , tm2;
    long int tm1 ;
    tmpTime =TIME/60.0 ;
    tm1=(int) tmpTime/60.0;
    tm2=(tmpTime/60.0 - tm1) * 60.0 ;
    sp1[0]='\0';
    if (tm1==0) {
       sprintf(sp1,"\nВремя выполнения ->\n %.2f секунд = %.2f минут",TIME,TIME/60);
     } else {
        sprintf(sp1,"\nВремя выполнения ->\n %.2f секунд = %.2f минут = [ %d ч %.1f мин ]",TIME,tmpTime, tm1 ,tm2);
    }

    MessageDlg(sp1, mtConfirmation, TMsgDlgButtons() << mbOK , 0);

  }


}
//---------------------------------------------------------------------------


void __fastcall TForm1::RadioGroupMETHODSClick(TObject *Sender)
{

     if (RadioGroupMETHODS->ItemIndex==0) {
     // MTC200
     //add a string to the list
       try
       {
         ComboBoxRAPID->Clear();
         ComboBoxRAPID->Items->Append("30000");
         ComboBoxRAPID->Items->Append("12000");
         ComboBoxRAPID->Items->Append("10000");
         ComboBoxRAPID->Items->Append("8000");
         ComboBoxRAPID->Items->Append("6000");
         ComboBoxRAPID->Items->Append("5000");
         ComboBoxRAPID->Items->Append("3000");
         ComboBoxRAPID->Items->Append("2000");
         ComboBoxRAPID->Items->Append("1000");
         ComboBoxRAPID->Items->Append("800");
         ComboBoxRAPID->ItemIndex=1;
         ComboBoxACC->Clear();
         ComboBoxACC->Items->Append("100");
         ComboBoxACC->Items->Append("50");
         ComboBoxACC->Items->Append("35");
         ComboBoxACC->Items->Append("15");
         ComboBoxACC->Items->Append("10");
         ComboBoxACC->ItemIndex=2;
         LabelAcc->Caption="Ускорение (% от 100%) =" ;
        }
        catch (Exception &exception)
        {
         }

     }
     if (RadioGroupMETHODS->ItemIndex==1) {
     // H33
     //add a string to the list
       try
       {
         ComboBoxRAPID->Clear();
         ComboBoxRAPID->Items->Append("2400");
         ComboBoxRAPID->Items->Append("1200");
         ComboBoxRAPID->Items->Append("1000");
         ComboBoxRAPID->Items->Append("900");
         ComboBoxRAPID->Items->Append("800");
         ComboBoxRAPID->Items->Append("700");
         ComboBoxRAPID->Items->Append("600");
         ComboBoxRAPID->Items->Append("300");
         ComboBoxRAPID->ItemIndex=1;
         ComboBoxACC->Clear();
         ComboBoxACC->Items->Append("100");
         ComboBoxACC->Items->Append("50");
         ComboBoxACC->Items->Append("40");
         ComboBoxACC->Items->Append("30");
         ComboBoxACC->Items->Append("25");
         ComboBoxACC->Items->Append("20");
         ComboBoxACC->Items->Append("10");
         ComboBoxACC->Items->Append("5");
         ComboBoxACC->ItemIndex=2;
         LabelAcc->Caption="Ускорение (мм/(сек*сек)) =" ;
        }
        catch (Exception &exception)
        {
         }

     }

}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormCreate(TObject *Sender)
{
     // MTC200
     RadioGroupMETHODS->ItemIndex=0;
     RadioGroupMETHODSClick(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button2Click(TObject *Sender)
{
 Application->Terminate();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ComboBoxRAPIDChange(TObject *Sender)
{
   long int tmp ;
   tmp=abs(atoi(ComboBoxRAPID->Text.c_str()));
   if (tmp<=0) tmp=12000 ;
   ComboBoxRAPID->Text=tmp;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ComboBoxACCChange(TObject *Sender)
{
   long int tmp ;
   tmp=abs(atoi(ComboBoxACC->Text.c_str()));
   if (tmp<=0) tmp=100 ;
   ComboBoxACC->Text=tmp;
}
//---------------------------------------------------------------------------

