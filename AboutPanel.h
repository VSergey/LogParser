//---------------------------------------------------------------------------
#ifndef AboutPanelH
#define AboutPanelH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <jpeg.hpp>
//---------------------------------------------------------------------------
class TAboutForm : public TForm
{
__published:	// IDE-managed Components
    TImage *Image1;
    TButton *Button1;
    TPanel *Panel1;
    TLabel *Label1;
    TLabel *Label2;
    void __fastcall Button1Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TAboutForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TAboutForm *AboutForm;
//---------------------------------------------------------------------------
#endif
