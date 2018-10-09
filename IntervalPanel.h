//---------------------------------------------------------------------------
#ifndef IntervalPanelH
#define IntervalPanelH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>

#include "LogCollection.h"

//---------------------------------------------------------------------------
class TIntervalForm : public TForm
{
__published:	// IDE-managed Components
    TListBox *IntervalListBox;
    TLabel *Label1;
    TButton *OKButton;
    TButton *CancelButton;
    void __fastcall CancelButtonClick(TObject *Sender);
    void __fastcall OKButtonClick(TObject *Sender);
    void __fastcall IntervalListBoxDblClick(TObject *Sender);
private:	// User declarations
    bool empty;
    
public:		// User declarations
    __fastcall TIntervalForm(TComponent* Owner);

    void Clear();
    int selectedIndex();
    AnsiString selectedString();
    void init(LogCollection& records, int selectedInterval);
    static AnsiString convertDay(int day);
};
//---------------------------------------------------------------------------
extern PACKAGE TIntervalForm *IntervalForm;
//---------------------------------------------------------------------------
#endif
