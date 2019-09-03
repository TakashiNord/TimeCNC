//---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <FileCtrl.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
        TDirectoryListBox *DirectoryListBox1;
        TDriveComboBox *DriveComboBox1;
        TFileListBox *FileListBox1;
        TButton *Button1;
        TLabel *Label1;
        TEdit *EditFile;
        TFilterComboBox *FilterComboBox1;
        TRadioGroup *RadioGroupMETHODS;
        TLabel *LabelAcc;
        TComboBox *ComboBoxACC;
        TComboBox *ComboBoxRAPID;
        TTimer *Timer1;
        TBevel *Bevel1;
        TButton *Button2;
        TLabel *Label2;
        TLabel *Label6;
        TStaticText *StaticText1;
        void __fastcall Button1Click(TObject *Sender);
        void __fastcall RadioGroupMETHODSClick(TObject *Sender);
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall Button2Click(TObject *Sender);
        void __fastcall ComboBoxRAPIDChange(TObject *Sender);
        void __fastcall ComboBoxACCChange(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
