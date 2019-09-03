//---------------------------------------------------------------------------

#ifndef MainH
#define MainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <ToolWin.hpp>
#include <FileCtrl.hpp>
#include <ImgList.hpp>
#include <Dialogs.hpp>
#include <Menus.hpp>
#include <Graphics.hpp>
//---------------------------------------------------------------------------
class TFormMain : public TForm
{
__published:	// IDE-managed Components
        TPanel *Panel1;
        TToolBar *ToolBar1;
        TPanel *Panel4;
        TToolButton *ToolButton1;
        TToolButton *ToolButton2;
        TToolButton *ToolButton3;
        TToolButton *ToolButton4;
        TMemo *MemoStr;
        TImageList *ImageListMainTool1;
        TStatusBar *StatusBarStr;
        TToolBar *ToolBar2;
        TToolButton *ToolButton5;
        TToolButton *ToolButton6;
        TPanel *Panel2;
        TGroupBox *GroupBox1;
        TComboBox *ComboBoxMode;
        TStaticText *StaticTextMode;
        TGroupBox *GroupBoxType;
        TComboBox *ComboBoxType;
        TLabel *LabelRAPID;
        TComboBox *ComboBoxRAPID;
        TLabel *LabelAcc;
        TComboBox *ComboBoxACC;
        TLabel *LabelRapiddop;
        TImageList *ImageListStr;
        TPanel *Panel6;
        TDriveComboBox *DriveComboBox1;
        TPanel *Panel7;
        TPanel *Panel8;
        TDirectoryListBox *DirectoryListBox1;
        TFileListBox *FileListBox1;
        TFilterComboBox *FilterComboBox1;
        TCheckBox *CheckBoxFileFilter;
        TTimer *Timer1;
        TToolButton *ToolButton7;
        TPopupMenu *PopupMenu1;
        TMenuItem *N1;
        TImageList *ImageListMainTool2;
        TMenuItem *UltraEdit321;
        TMenuItem *WordPad1;
        TToolButton *ToolButton8;
        TSaveDialog *SaveDialog1;
        TCheckBox *CheckBoxTools;
        TBevel *Bevel1;
        void __fastcall ToolButton3Click(TObject *Sender);
        void __fastcall ToolButton4Click(TObject *Sender);
        void __fastcall ToolButton2Click(TObject *Sender);
        void __fastcall ToolButton1Click(TObject *Sender);
        void __fastcall ToolButton6Click(TObject *Sender);
        void __fastcall ToolButton5Click(TObject *Sender);
        void __fastcall ComboBoxTypeChange(TObject *Sender);
        void __fastcall ComboBoxModeChange(TObject *Sender);
        void __fastcall Timer1Timer(TObject *Sender);
        void __fastcall ToolButton7Click(TObject *Sender);
        void __fastcall N1Click(TObject *Sender);
        void __fastcall FormResize(TObject *Sender);
        void __fastcall ToolButton8Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TFormMain(TComponent* Owner);
        void MessageOutput(AnsiString S);
        TProgressBar *ProgressBar;
};
//---------------------------------------------------------------------------
extern PACKAGE TFormMain *FormMain;
//---------------------------------------------------------------------------
#endif
