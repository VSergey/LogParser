//---------------------------------------------------------------------------
#ifndef FormInfoH
#define FormInfoH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TInfoForm : public TForm
{
__published:	// IDE-managed Components
  TListBox *UsersList;
  TListBox *TxList;
  TButton  *OKButton;
  TLabel   *Label1;
  TLabel   *Label2;
  void __fastcall OKButtonClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
  __fastcall TInfoForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TInfoForm *InfoForm;
//---------------------------------------------------------------------------
#endif
