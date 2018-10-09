//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "IntervalPanel.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TIntervalForm *IntervalForm;
//---------------------------------------------------------------------------
__fastcall TIntervalForm::TIntervalForm(TComponent* Owner)
  : TForm(Owner)
{
    empty = true;
}
//---------------------------------------------------------------------------
void TIntervalForm::Clear()
{
    empty = true;
    IntervalListBox->Clear();
}
//---------------------------------------------------------------------------
int TIntervalForm::selectedIndex()
{
    return IntervalListBox->ItemIndex;
}
//---------------------------------------------------------------------------
AnsiString TIntervalForm::selectedString()
{
    return IntervalListBox->Items->Strings[IntervalListBox->ItemIndex];
}
//---------------------------------------------------------------------------
void TIntervalForm::init(LogCollection& records, int selectedInterval)
{
    if(empty) {
        // fill list box
        for(int i = 0; i < records.intervals(); i++) {
            LogInterval* interval = records.getInterval(i);
            IntervalListBox->Items->Add(convertDay(interval->day()) +
                " - "+AnsiString(interval->hour())+" h");
        }
        empty = false;
    }
    // select current interval
    IntervalListBox->ItemIndex = selectedInterval;
}
//---------------------------------------------------------------------------
AnsiString TIntervalForm::convertDay(int day)
{
    int d = day % 100;
    int m = day/100%100;
    int y = day/10000;
    return AnsiString(y)+"-"+AnsiString(m)+"-"+AnsiString(d);
}
//---------------------------------------------------------------------------
void __fastcall TIntervalForm::CancelButtonClick(TObject *Sender)
{
    ModalResult = mrCancel;
}
//---------------------------------------------------------------------------
void __fastcall TIntervalForm::OKButtonClick(TObject *Sender)
{
    ModalResult = mrOk;
}
//---------------------------------------------------------------------------
void __fastcall TIntervalForm::IntervalListBoxDblClick(TObject *Sender)
{
    ModalResult = mrOk;
}
//---------------------------------------------------------------------------

