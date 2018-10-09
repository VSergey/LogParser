//---------------------------------------------------------------------------
#ifndef MainFormH
#define MainFormH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Menus.hpp>
#include <Dialogs.hpp>
#include <vector>
//---------------------------------------------------------------------------
#include "LogCollection.h"
#include "LogCollection.h"
//---------------------------------------------------------------------------
class TMain : public TForm
{
__published:	// IDE-managed Components
    TMainMenu   *MainMenu1;
    TStatusBar  *StatusBar;
    TScrollBox  *ScrollBox;
    TMenuItem   *FileMenuItem;
    TMenuItem   *OpenMenuItem;
    TMenuItem   *AboutMenuItem;
    TMenuItem   *ExitMenuItem;
    TPanel      *Panel1;
    TOpenDialog *OpenDialog;
    TPanel *Panel;
    TLabel *Label1;
    TLabel *TxNumber;
    TLabel *Label2;
    TLabel *User;
    TLabel *Label3;
    TLabel *Wait;
    TLabel *Label5;
    TPanel *Panel2;
    TPanel *Panel3;
    TPanel *Panel4;
    TPanel *Panel5;
    TLabel *Exec;
    TLabel *Label4;
    TLabel *Action;
    TLabel *Label6;
    TLabel *Puts;
    TLabel *Label8;
    TLabel *Gets;
    TLabel *Label7;
    TLabel *Start;
    TLabel *Label9;
    TLabel *End;
    TMenuItem *Zoom;
    TMenuItem *Zoom1000Item;
    TMenuItem *Zoom100Item;
    TMenuItem *Zoom10Item;
    TMenuItem *Zoom10000Item;
    TPanel *UserPanel;
    TMenuItem *SelectIntervalMenuItem;
    TMenuItem *Zoom500Item;
    TMenuItem *NextIntervalMenuItem;
    TMenuItem *PrevIntervalMenuItem;
    TMenuItem *N1;
    TMenuItem *Zoom5000Item;
    TScrollBox *ScrollBoxParent;
    TPanel *LinePanel;
    TMenuItem *TrCountLabel;
    TMenuItem *AppendMenuItem;
    void __fastcall OpenMenuItemClick(TObject *Sender);
    void __fastcall AppendMenuItemClick(TObject *Sender);
    void __fastcall ExitMenuItemClick(TObject *Sender);
    void __fastcall ShapeMouseDown (TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall Zoom1000ItemClick(TObject *Sender);
    void __fastcall Zoom100ItemClick(TObject *Sender);
    void __fastcall Zoom10ItemClick(TObject *Sender);
    void __fastcall Zoom10000ItemClick(TObject *Sender);
    void __fastcall ScrollBoxMouseWheel(TObject *Sender, TShiftState Shift,
          int WheelDelta, TPoint &MousePos, bool &Handled);
    void __fastcall SelectIntervalMenuItemClick(TObject *Sender);
    void __fastcall Zoom500ItemClick(TObject *Sender);
    void __fastcall AboutMenuItemClick(TObject *Sender);
    void __fastcall NextIntervalMenuItemClick(TObject *Sender);
    void __fastcall PrevIntervalMenuItemClick(TObject *Sender);
    void __fastcall Zoom5000ItemClick(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall PanelMouseMove(TObject *Sender, TShiftState Shift, int X, int Y);
    void __fastcall TrCountLabelClick(TObject *Sender);

private:	// User declarations
    LogCollection records;
    int cRecords;
    int scale;
    int selectedInterval;
    UserNames users;
    UserIndexes userIndex;
    AnsiString currentFile;

    void clearRecors();
    void rescaleDiagram();
    AnsiString convertLength(long time);
    void buildChart();
    void updateCaption();

public:		// User declarations
    __fastcall TMain(TComponent* Owner);
    void loadFile(AnsiString fileName, bool append);
    LogInterval* getSelectedInterval() { return records.getInterval(selectedInterval); }
};
//---------------------------------------------------------------------------
extern PACKAGE TMain *Main;
//---------------------------------------------------------------------------
#endif
   