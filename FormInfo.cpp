//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "FormInfo.h"
#include "Diagram.h"
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
void TInfoForm::update(DiagramCell *cell, LogInterval *interval, bool readOnly, bool hideWS)
{
  UsersList->Clear();
  for (Users::iterator it=cell->usersBegin(); it != cell->usersEnd(); it++) {
    AnsiString user = *it;
    UsersList->Items->Add(user);
  }
  TxList->Clear();
  for (TxNames::iterator it=cell->txBegin(); it != cell->txEnd(); it++) {
    AnsiString txId = *it;
    for (RecordsArray::iterator it=interval->begin(); it != interval->end(); it++) {
      const LogRecord *rec= *it;
      if(txId == rec->getTxNumber()) {
        if(readOnly && !rec->isRead()) break;
        if(hideWS && !DiagramForm->isRealUser(rec->getUser())) break;
        TxList->Items->Add(txId + " -" + (rec->isRead()? "R":"W") + "- " +
          rec->getUser() + " -> " + rec->convertStartTime() + ": " + rec->getAction());
        break;
      }
    }
  }
  Show();
}
//---------------------------------------------------------------------------
