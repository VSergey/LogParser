//---------------------------------------------------------------------------
#ifndef ParseLogPanelH
#define ParseLogPanelH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <stdio.h>

#include "MainForm.h"

const int s_rowSize = 13;
//---------------------------------------------------------------------------
class TParseLogForm : public TForm
{
__published:	// IDE-managed Components
    TProgressBar *ProgressBar;
    TButton *CancelButton;
    TLabel *Label1;
    TLabel *Label2;
    TLabel *Users;
    TLabel *Records;
    TLabel *ProcessName;
    TLabel *Read;
    TLabel *Label4;
    TLabel *Label3;
    TLabel *Write;
    TLabel *Label5;
    TLabel *Wait;
    void __fastcall CancelButtonClick(TObject *Sender);
private:	// User declarations
    bool processing;
    long filesize(FILE *stream);
    LogRecord * createRecord(char *buffer);
public:		// User declarations
    __fastcall TParseLogForm(TComponent* Owner);
    void load(FILE *, int scale, LogCollection&, UserNames&, UserIndexes&);
};
//---------------------------------------------------------------------------
extern PACKAGE TParseLogForm *ParseLogForm;
//---------------------------------------------------------------------------
#endif
