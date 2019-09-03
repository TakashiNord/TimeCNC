//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "about.h"
#include <windows.h>
#include <stdio.h>

#define BUFFER 256
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TAboutForm *AboutForm;
//---------------------------------------------------------------------------
__fastcall TAboutForm::TAboutForm(TComponent* Owner)
        : TForm(Owner)
{
     //RichEdit1->Lines->LoadFromFile("About.rtf");
}
//---------------------------------------------------------------------------
void __fastcall TAboutForm::FormShow(TObject *Sender)
{
  LPTSTR lpszInfo; // pointer to information string
  DWORD cchBuff = BUFFER;
  TCHAR tchBuffer[BUFFER];  // buffer for concatenated string
  char  info[BUFFER+20];
  
  lpszInfo=tchBuffer;

  // Get and display the user name.
  GetUserName(lpszInfo, &cchBuff);
  info[0]='\0';sprintf(info,"Ник: %s",lpszInfo);
  LabelUser->Caption=(AnsiString)info;
  // Get and display the name of the computer.
  cchBuff = BUFFER;
  GetComputerName(lpszInfo, &cchBuff);
  info[0]='\0';sprintf(info,"Компьютер: %s",lpszInfo);
  LabelComputer->Caption=(AnsiString)info;

  TMemoryStatus MS;
  MS.dwLength = sizeof(MS);
  GlobalMemoryStatus(&MS);
  PhysMem->Caption = FormatFloat((AnsiString)"#,###' KB'", MS.dwTotalPhys / 1024);
  LPSTR lpMemLoad = new char[5];
  sprintf(lpMemLoad,"%d %%",MS.dwMemoryLoad);
  FreeRes->Caption = (AnsiString)lpMemLoad;
  delete [] lpMemLoad;
}
//---------------------------------------------------------------------------

void __fastcall TAboutForm::LabelCloseClick(TObject *Sender)
{
   Close();
}
//---------------------------------------------------------------------------



