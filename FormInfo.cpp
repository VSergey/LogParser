//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "FormInfo.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TInfoForm *InfoForm;
//---------------------------------------------------------------------------
__fastcall TInfoForm::TInfoForm(TComponent* Owner)
  : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TInfoForm::OKButtonClick(TObject *Sender)
{
  Hide();  
}
//---------------------------------------------------------------------------
