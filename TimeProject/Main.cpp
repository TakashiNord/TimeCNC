//---------------------------------------------------------------------------
#define NO_WIN32_LEAN_AND_MEAN
#include <stdio.h>
#include <string.h>

#include <vcl.h>
#pragma hdrstop

#include "Main.h"
#include "about.h"
#include "TimeCount.h"
#include "algobaset.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormMain *FormMain;
//---------------------------------------------------------------------------
__fastcall TFormMain::TFormMain(TComponent* Owner)
        : TForm(Owner)
{
   try
   {
     ComboBoxType->Clear();
     ComboBoxType->Items->Append("Н33(Атака)");
     ComboBoxType->Items->Append("MTC200");
     ComboBoxType->Items->Append("Альфа");
     ComboBoxType->Items->Append("Siemens");
     ComboBoxType->Items->Append("HEID (stand)");
     ComboBoxType->Items->Append("HEID (iso)");
     ComboBoxType->Items->Append("Fanuc");
     ComboBoxType->Items->Append("Mazatrol series 600");
     ComboBoxType->Items->Append("NC110-NC210");
     ComboBoxType->Items->Append("NS720");
     ComboBoxType->Items->Append("2C42");
     ComboBoxType->Items->Append("2P32");
     ComboBoxType->Items->Append("Н55");
     ComboBoxType->Items->Append("others <другие>");
    }
    catch (Exception &exception) { ; }

   ComboBoxMode->ItemIndex=1;  ComboBoxModeChange(NULL);
   //ComboBoxMode->Enabled=False; // False;
   ComboBoxRAPID->Text="1200";
   ComboBoxType->ItemIndex=0;  ComboBoxTypeChange(NULL);
   ToolButton6Click(NULL);
   AnsiString S=DateTimeToStr(Now())+ " TimeCount v3.5 build 56, Copyright (c) 2005, RAE" ;
   //StatusBarStr->SimpleText=S
   StatusBarStr->Panels->Items[2]->Text =S;
   FileListBox1->Mask = FilterComboBox1->Mask;
   FileListBox1->Update();
   //SendMessage(FileListBox1->Handle, LB_SETSEL, TRUE, 0 );

   ProgressBar = new TProgressBar ( StatusBarStr );
   ProgressBar->Parent = StatusBarStr;
   ProgressBar->Position = 0;
   ProgressBar->Visible = false;

    RECT Rect;
    StatusBarStr->Perform ( SB_GETRECT,  1,  (LPARAM)&Rect );

    ProgressBar->Top = Rect.top;
    ProgressBar->Left = Rect.left;
    ProgressBar->Width = StatusBarStr->Panels->Items [ 1 ]->Width;
    ProgressBar->Height = Rect.bottom - Rect.top;
}
//---------------------------------------------------------------------------

void TFormMain::MessageOutput(AnsiString S)
{
  MemoStr->Lines->Append(S);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ToolButton1Click(TObject *Sender)
{
  MessageBeep(0);
  Application->ProcessMessages();
  Application->Terminate();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ToolButton2Click(TObject *Sender)
{
 AboutForm->ShowModal();
 /*try
  {
    Application->CreateForm(__classid(TAboutForm), &AboutForm);
    AboutForm->ShowModal();
    delete AboutForm;
  }
  catch (Exception &exception) { Application->ShowException(&exception); }
  */
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ToolButton3Click(TObject *Sender)
{
   AnsiString S = "";
   if (SelectDirectory("Выберите Папку с УП", "", S)) DirectoryListBox1->Directory=S;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ToolButton4Click(TObject *Sender)
{
  AnsiString sfile,S ;
  double feed_rate=0,acc=0;
  double TIME1, TIME2 ;
  double sTIME1, sTIME2 ;
  double  tmpTime , tm2 ; long int tm1 ;
  char info[256],infostr[50],type_cnc[20];
  int mode,type;
  bool chFileFilter;
  long int result;
  int i, num , num_files ;

  int j, iT , sT ; double sT1,sT2 ;
  struct mTime pT;
  bool tmode;

  /*****************************************/

  TIME1=0.0; TIME2=0.0;
  sTIME1=0.0; sTIME2=0.0;

  mode=ComboBoxMode->ItemIndex;
  type=ComboBoxType->ItemIndex;
  tmode=CheckBoxTools->Checked;
  chFileFilter=CheckBoxFileFilter->Checked;
  num_files=FileListBox1->Items->Count;

  if (num_files<=0) { return ; }
  num=0; for (i = 0; i < num_files; i++) {  if (FileListBox1->Selected[i]) num++; }
  if ((num<1 && !chFileFilter)) { return ; }

  // если ручное
  if (mode!=0) {

    feed_rate=atof(ComboBoxRAPID->Text.c_str());
    if (feed_rate<=0.) {
      S="Расчет прерван..\nУточните значение ускоренного перемещения...(<=0)";
      MessageOutput(S);
      MessageDlg(S, mtWarning, TMsgDlgButtons() << mbOK , 0);
      return ;
    }
    feed_rate=fabs(feed_rate);

    acc=atof(ComboBoxACC->Text.c_str());
    if (acc<0.) {
     if (type==0) { /*Н33(Атака)*/
      S="Расчет прерван..\nУточните значение ускорения...(<=0)";
      MessageOutput(S);
      MessageDlg(S, mtWarning, TMsgDlgButtons() << mbOK , 0);
      return ;
     }
    }
    acc=fabs(acc);

  }

  ProgressBar->Min = 0;
  ProgressBar->Max = num_files;
  ProgressBar->Show();

  TTimeCounter *tc=new TTimeCounter();
  MessageOutput("");
  MessageOutput("======Start > =" + DateTimeToStr(Now()));
  MessageOutput("--------------------------------------------------------------------------------------------------------");
  MessageOutput("#N  |      File Name       |                     Time                             |     Type CNC");
  MessageOutput("--------------------------------------------------------------------------------------------------------");

  MessageOutput("");
  for (i = 0; i < num_files; i++)
  {
    TIME1=0.0; TIME2=0.0;

    Application->ProcessMessages();

    if (chFileFilter==False) {
      if (!(FileListBox1->Selected[i])) continue;
    }

    sfile=FileListBox1->Items->Strings[i];

    ProgressBar->Position = i;

    if (!FileExists(sfile))
    {
      MessageBeep(0);
      MessageOutput(AnsiString("Файл: ") + sfile + AnsiString(" не найден..."));
      continue;
    }

    // auto-определение
    if (mode==0) {
      //result= program_format_H33(sfile.c_str(), 0);
      //if (result==0) { type=0 ; }
    }

    type_cnc[0]='\0';
    switch (type) {
     case 0:  // Н33 (Атака)
      sprintf (type_cnc,"H33,Атака");
      //if (mode==0) {
        if (mode==0) { feed_rate=program_maxfeed_H33_H55(sfile.c_str(),0); }
        program_time_H33_H55(33,sfile.c_str(),feed_rate, &TIME1, &TIME2,0 );
      //} else {
      //   tc->start(sfile.c_str(),(int)acc,(int)feed_rate);
      //   TIME1=(double) tc->count()/60.;
      //   TIME2=(double) tc->TIME2/60. ;
      //}
     break;
     case 1:  // MTC200
      sprintf (type_cnc,"MTC200");
      if (mode==0) { feed_rate=program_maxfeed(sfile.c_str()); }
      if (!tmode) time_cnc(1 ,sfile.c_str(), feed_rate , 1. , &TIME1, &TIME2);
      else time_cnc_ex(1 ,sfile.c_str(), feed_rate , 1. , &pT);
     break;
     case 2:  // Альфа
      sprintf (type_cnc,"Альфа");
      if (mode==0) { feed_rate=program_maxfeed(sfile.c_str()); }
      if (!tmode) time_cnc(2 ,sfile.c_str(), feed_rate , 1. , &TIME1, &TIME2);
      else time_cnc_ex(2 ,sfile.c_str(), feed_rate , 1. , &pT);
     break;
     case 3:  // Siemens
      sprintf (type_cnc,"Siemens");
      if (mode==0) { feed_rate=program_maxfeed(sfile.c_str()); }
      if (!tmode) time_cnc(3 ,sfile.c_str(), feed_rate , 1. , &TIME1, &TIME2);
      else time_cnc_ex(3 ,sfile.c_str(), feed_rate , 1. , &pT);
     break;
     case 4:  // HEIDENHAIN
      sprintf (type_cnc,"HEIDENHAIN");
      if (mode==0) { feed_rate=program_maxfeed(sfile.c_str()); }
      if (!tmode) time_cnc(41 ,sfile.c_str(), feed_rate , 1. , &TIME1, &TIME2);
      else time_cnc_ex(41 ,sfile.c_str(), feed_rate , 1. , &pT);
     break;
     case 5:  // HEIDENHAIN
      sprintf (type_cnc,"HEIDENHAIN (iso)");
      if (mode==0) { feed_rate=program_maxfeed(sfile.c_str()); }
      if (!tmode) time_cnc(40 ,sfile.c_str(), feed_rate , 1. , &TIME1, &TIME2);
      else time_cnc_ex(40 ,sfile.c_str(), feed_rate , 1. , &pT);
     break;
     case 6:  // Fanuc
      sprintf (type_cnc,"Fanuc");
      if (mode==0) { feed_rate=program_maxfeed(sfile.c_str()); }
      if (!tmode) time_cnc(5 ,sfile.c_str(), feed_rate , 1. , &TIME1, &TIME2);
      else time_cnc_ex(5 ,sfile.c_str(), feed_rate , 1. , &pT);
     break;
     case 7:  // Mazatrol series 600
      sprintf (type_cnc,"Mazatrol");
      if (mode==0) { feed_rate=program_maxfeed(sfile.c_str()); }
      if (!tmode) time_cnc(5 ,sfile.c_str(), feed_rate , 1. , &TIME1, &TIME2);
      else time_cnc_ex(5 ,sfile.c_str(), feed_rate , 1. , &pT);
     break;
     case 8:  // NC110-NC210
      sprintf (type_cnc,"NC110-NC210");
      if (mode==0) { feed_rate=program_maxfeed(sfile.c_str()); }
      if (!tmode) time_cnc(6 ,sfile.c_str(), feed_rate , 1. , &TIME1, &TIME2);
      else time_cnc_ex(6 ,sfile.c_str(), feed_rate , 1. , &pT);
     break;
     case 9:  // NS720
      sprintf (type_cnc,"NS720");
      if (mode==0) { feed_rate=program_maxfeed(sfile.c_str()); }
      if (!tmode) time_cnc(7 ,sfile.c_str(), feed_rate , 1. , &TIME1, &TIME2);
      else time_cnc_ex(7 ,sfile.c_str(), feed_rate , 1. , &pT);
     break;
     case 10:  // 2C42
      sprintf (type_cnc,"2C42");
      if (mode==0) { feed_rate=program_maxfeed(sfile.c_str()); }
      if (!tmode) time_cnc(8 ,sfile.c_str(), feed_rate , 1. , &TIME1, &TIME2);
      else time_cnc_ex(8 ,sfile.c_str(), feed_rate , 1. , &pT);
     break;
     case 11:  // 2P32
      sprintf (type_cnc,"2P32");
      if (mode==0) { feed_rate=program_maxfeed(sfile.c_str()); }
      if (!tmode) time_cnc(9 ,sfile.c_str(), feed_rate , 1. , &TIME1, &TIME2);
      else time_cnc_ex(9 ,sfile.c_str(), feed_rate , 1. , &pT);
     break;
     case 12:  // Н55
      sprintf (type_cnc,"Н55");
      if (mode==0) { feed_rate=program_maxfeed_H33_H55(sfile.c_str(),1); }
      program_time_H33_H55(55,sfile.c_str(),feed_rate, &TIME1, &TIME2,1 );
     break;
     default :
        sprintf (type_cnc,"(unknown)");
        if (mode==0) { feed_rate=program_maxfeed(sfile.c_str()); }
        if (!tmode) time_cnc(0 ,sfile.c_str(), feed_rate , 1. , &TIME1, &TIME2);
        else time_cnc_ex(0 ,sfile.c_str(), feed_rate , 1. , &pT);

    }// end switch (type)


    switch (type) {
     case 0:  case 12:
     // Расчет времени - минут и часов
     tmpTime =TIME1;//60.0 ;
     tm1=(long int) tmpTime/60.0;
     tm2=(tmpTime/60.0 - tm1) * 60.0 ;
     tm2=ceil(tm2);
     sTIME1+=TIME1;   TIME2=ceil(TIME2);   sTIME2+=TIME2;
     // вывод
     info[0]='\0';
     sprintf (info, "% 4ld  %-20.20s   Tm=%- 4.2lf ' =[ %ld ч %.0f мин ]  (Tf=%-.0lf мин, Fmax>=%-.1lf)       %s",\
                   i+1,\
                   sfile.c_str(),\
                   TIME1,tm1 ,tm2 ,\
                   TIME2 , feed_rate , type_cnc);
     MessageOutput(info);
     break;
     default :

      if (!tmode) {

        tmpTime =TIME1;//60.0 ;
        tm1=(long int) tmpTime/60.0;
        tm2=(tmpTime/60.0 - tm1) * 60.0 ;
        tm2=ceil(tm2);
        sTIME1+=TIME1;   TIME2=ceil(TIME2);   sTIME2+=TIME2;
        // вывод
        info[0]='\0';
        sprintf (info, "% 4ld  %-20.20s   Tm=%- 4.2lf ' =[ %ld ч %.0f мин ]  (Tf=%-.0lf мин, Fmax>=%-.1lf)       %s",\
                      i+1,\
                      sfile.c_str(),\
                      TIME1,tm1 ,tm2 ,\
                      TIME2 , feed_rate , type_cnc);
        MessageOutput(info);

      } else {

       info[0]='\0';
       sprintf (info, "% 4ld  %-20.20s                                                          %s",\
                    i+1, sfile.c_str(),type_cnc);
       MessageOutput(info);
       // подчитываем количество произведенных смен инструмента (без T0)
       for(j=1,sT=0;j<nTools;j++) { if (pT.cnt[j]>0) sT++; }
       if (sT<=1) {
         TIME1=pT.time_base[0];  TIME2=pT.time_rapid[0];   iT=pT.cnt[0];
         // Расчет времени - минут и часов
         tmpTime =TIME1;  tm1=(long int) tmpTime/60.0;  tm2=(tmpTime/60.0 - tm1) * 60.0 ;  tm2=ceil(fabs(tm2));
         info[0]='\0';
         sprintf (info, "                         (T%d)    Tm=%- 4.2lf ' =[ %ld ч %.0f мин ]  (Tf=%-.0lf мин, Fmax>=%-.1lf)",\
                       iT,\
                       TIME1,tm1 ,tm2 ,\
                       TIME2 , feed_rate );
         MessageOutput(info);
         sTIME1+=TIME1;   TIME2=ceil(TIME2);   sTIME2+=TIME2;
       } else {
        MessageOutput("--------------------------------------------------------------------------------------------------------");
        sT1=0.;sT2=0.;
        for(j=1,sT=0;j<nTools;j++) {
         if (pT.cnt[j]>0) {
            sT++;
            TIME1=pT.time_base[j];  TIME2=pT.time_rapid[j];  iT=pT.cnt[j];
            // Расчет времени - минут и часов
            tmpTime =TIME1;  tm1=(long int) tmpTime/60.0;  tm2=(tmpTime/60.0 - tm1) * 60.0 ;  tm2=ceil(fabs(tm2));
            info[0]='\0';
            sprintf (info, "                         (T%d)    Tm=%- 4.2lf ' =[ %ld ч %.0f мин ]  (Tf=%-.0lf мин, Fmax>=%-.1lf)",\
                          iT,\
                          TIME1,tm1 ,tm2 ,\
                          TIME2 , feed_rate );
            MessageOutput(info);
            sT1+=TIME1;   TIME2=ceil(TIME2);   sT2+=TIME2;
         }
        }
        // суммарное время,вывод
        tmpTime =sT1; tm1=(long int) tmpTime/60.0;  tm2=(tmpTime/60.0 - tm1) * 60.0 ;  tm2=ceil(fabs(tm2));
        info[0]='\0';
        sprintf (info, "            Сумма:%-20.20s   Ts=%- 4.2lf ' =[ %ld ч %.0f мин ]  (Tfs=%-.0lf мин)",\
                        " ",\
                        sT1,tm1 ,tm2 ,\
                        sT2);
        MessageOutput(info);
        MessageOutput("--------------------------------------------------------------------------------------------------------");
        sTIME1+=sT1;  sTIME2+=sT2;
       }

      } // end else

     } // end switch (type)

   }
   tc->TTimeCounter::~TTimeCounter(); // деструктор
   delete tc;

   MessageOutput("");
   MessageOutput("--------------------------------------------------------------------------------------------------------");
   // суммарное время
   // вывод
   tmpTime =sTIME1;//60.0 ;
   tm1=(long int) tmpTime/60.0;
   tm2=(tmpTime/60.0 - tm1) * 60.0 ;
   tm2=ceil(tm2);
   info[0]='\0';
   sprintf (info, "Сумма:%-20.20s   Ts=%- 4.2lf ' =[ %ld ч %.0f мин ]  (Tfs=%-.0lf мин)",\
                   " ",\
                   sTIME1,tm1 ,tm2 ,\
                   sTIME2);
   MessageOutput(info);

   MessageOutput("");
   MessageOutput("======End > " + TimeToStr(Time()) );

   ProgressBar->Visible=False;
   ProgressBar->Position = 0;

}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ToolButton6Click(TObject *Sender)
{
   MemoStr->Clear();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ToolButton5Click(TObject *Sender)
{
   //TSaveDialog *SaveDialog1;
   //SaveDialog1 = new TSaveDialog(this);
   if (SaveDialog1->Execute()) MemoStr->Lines->SaveToFile(SaveDialog1->FileName);
   //delete SaveDialog1;
   DirectoryListBox1->Update();
   FileListBox1->Update();
   Application->ProcessMessages();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ToolButton7Click(TObject *Sender)
{
 try { ShellExecute( NULL, "open" ,"notepad",NULL,NULL,SW_SHOW ); }
 catch (Exception &exception){ ;
    /*Application->ShowException(&exception);*/
    Application->MessageBox("ShellExecute failed; notepad.exe  ","> ошибка ...", MB_OK);
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ToolButton8Click(TObject *Sender)
{
 try { ShellExecute( NULL, "explore" ,NULL,NULL,NULL,SW_SHOW ); }
 catch (Exception &exception){ Application->ShowException(&exception); }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ComboBoxTypeChange(TObject *Sender)
{
   AnsiString S ;
   int i=ComboBoxType->ItemIndex;

   ComboBoxACC->Enabled=True;
   LabelAcc->Enabled=True;

   S=ComboBoxRAPID->Text;

   switch (i) {
    case 0:
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
        catch (Exception &exception) { ; }
    break;

    case 1:
     // MTC200
     //add a string to the list
       try
       {
         ComboBoxRAPID->Clear();
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
        catch (Exception &exception) { ; }
    break;

    default : ;

    //add a string to the list
       try
       {
         ComboBoxRAPID->Clear();
         ComboBoxRAPID->Items->Append("100000");
         ComboBoxRAPID->Items->Append("80000");
         ComboBoxRAPID->Items->Append("60000");
         ComboBoxRAPID->Items->Append("50000");
         ComboBoxRAPID->Items->Append("40000");
         ComboBoxRAPID->Items->Append("30000");
         ComboBoxRAPID->Items->Append("20000");
         ComboBoxRAPID->Items->Append("15000");
         ComboBoxRAPID->Items->Append("12000");
         ComboBoxRAPID->Items->Append("10000");
         ComboBoxRAPID->Items->Append("8000");
         ComboBoxRAPID->Items->Append("6000");
         ComboBoxRAPID->Items->Append("5000");
         ComboBoxRAPID->Items->Append("3000");
         ComboBoxRAPID->Items->Append("2000");
         ComboBoxRAPID->Items->Append("1000");
         ComboBoxRAPID->Items->Append("800");
         ComboBoxRAPID->Items->Append("600");
         ComboBoxRAPID->Items->Append("500");
         ComboBoxRAPID->Items->Append("300");
         //ComboBoxRAPID->ItemIndex=14;

         ComboBoxACC->Clear();
         ComboBoxACC->Text="NONE";
         ComboBoxACC->Enabled=False;
         LabelAcc->Caption="Ускорение (мм/(сек*сек)) =" ;
         LabelAcc->Enabled=False;
        }
        catch (Exception &exception) { ; }

        ComboBoxRAPID->Text=S;

    }

}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ComboBoxModeChange(TObject *Sender)
{
   int i=ComboBoxMode->ItemIndex;
   switch (i) {
    case 0: /*Auto*/
    //StaticTextMode->Caption="Распознавание типа УП, расчет ускорения (если необх) и макс подачи происходит автоматически";
    StaticTextMode->Caption="Расчет ускорения (если необх) и макс подачи происходит автоматически. (Возможна большая ошибка!)";
    LabelRAPID->Enabled=False;
    LabelRapiddop->Enabled=False;
    LabelAcc->Enabled=False;
    break;
    default : ;
    StaticTextMode->Caption="Пользователь должен Сам установить: тип УП, ускорение (если необх), максимальное значение холостого хода";
    LabelRAPID->Enabled=True;
    LabelRapiddop->Enabled=True;
    LabelAcc->Enabled=True;
   }

}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Timer1Timer(TObject *Sender)
{
    int i;
    i=FileListBox1->ItemIndex;
    DirectoryListBox1->Update();
    FileListBox1->Update();
    FileListBox1->ItemIndex=i;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::N1Click(TObject *Sender)
{
// notepad
  int i , num, j , num_files;
  char cmdApp[128];
  AnsiString sfile ;

  num_files=FileListBox1->Items->Count;

  num=0;
  for (i = 0; i < num_files; i++) { if (FileListBox1->Selected[i]) num++; }

  if (num>1) {
   i=MessageDlg("Выбрано открытие более 1 файла.\n Открыть в редакторе  ?",mtConfirmation, TMsgDlgButtons() << mbNo << mbYes, 0);
   if (i==mrNo) return;
  }

  cmdApp[0]='\0';
  j=PopupMenu1->Items->IndexOf((TMenuItem *)Sender);
  switch (j) {
       case 1: strcpy(cmdApp,"uedit32"); break ;
       case 2: strcpy(cmdApp,"write"); break ;
       default : strcpy(cmdApp,"notepad"); break;
  }

  for (i = 0; i < num_files; i++)
  {
    Application->ProcessMessages();
    if (!(FileListBox1->Selected[i])) continue;
    sfile=FileListBox1->Items->Strings[i];
    if (!FileExists(sfile))
    {
      MessageBeep(0);
      MessageOutput(AnsiString("Файл ") + sfile + AnsiString(" не найден..."));
      continue;
    }

    // вывод
    try
    {
      ShellExecute( NULL, "open" ,cmdApp,sfile.c_str(),0,SW_SHOWNORMAL );
    }
    catch (Exception &exception) { Application->ShowException(&exception); return ; }

  }

}
//---------------------------------------------------------------------------

void __fastcall TFormMain::FormResize(TObject *Sender)
{
    //int Size0=StatusBarStr->Panels->Items[1]->Width;
    /*int Size = StatusBarStr->Width;
    for ( int i = 0; i < StatusBarStr->Panels->Count; i++ )
      if (i!=1)
         Size -= StatusBarStr->Panels->Items[i]->Width;
    */
    //resize the first panel based on the form width
    //StatusBarStr->Panels->Items[ 1 ]->Width = Size;
    RECT Rect;
    StatusBarStr->Perform ( SB_GETRECT,  1,  (LPARAM)&Rect );

    ProgressBar->Top = Rect.top;
    ProgressBar->Left = Rect.left;
    ProgressBar->Width = StatusBarStr->Panels->Items [ 1 ]->Width;
    ProgressBar->Height = Rect.bottom - Rect.top;

}
//---------------------------------------------------------------------------



