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
        TEdit *EditRapid;
        TLabel *Label1;
        TEdit *EditFile;
        TFilterComboBox *FilterComboBox1;
        TRadioGroup *RadioGroup1;
        TRadioButton *MTC200;
        TRadioButton *H33;
        TLabel *Label2;
        TEdit *acc;
        TMemo *Memo1;
        TCheckBox *CheckBox1;
        void __fastcall Button1Click(TObject *Sender);
        void __fastcall accChange(TObject *Sender);
        void __fastcall EditRapidChange(TObject *Sender);
        void __fastcall MTC200Click(TObject *Sender);
        void __fastcall H33Click(TObject *Sender);
        void __fastcall FileListBox1Change(TObject *Sender);
        void __fastcall FilterComboBox1Change(TObject *Sender);
        void __fastcall CheckBox1Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
