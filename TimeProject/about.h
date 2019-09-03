//---------------------------------------------------------------------------

#ifndef aboutH
#define aboutH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include <StdCtrls.hpp>
#include <Buttons.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TAboutForm : public TForm
{
__published:	// IDE-managed Components
        TLabel *Label2;
        TLabel *Label3;
        TLabel *Label4;
        TLabel *Label5;
        TLabel *PhysMem;
        TLabel *FreeRes;
        TBevel *Bevel1;
        TLabel *LabelUser;
        TLabel *LabelComputer;
        TImage *Image2;
        TLabel *Label7;
        TLabel *Label8;
        TLabel *Label9;
        TLabel *Label1;
        TRichEdit *RichEdit1;
        TLabel *LabelClose;
        void __fastcall FormShow(TObject *Sender);
        void __fastcall LabelCloseClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TAboutForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TAboutForm *AboutForm;
//---------------------------------------------------------------------------
#endif
