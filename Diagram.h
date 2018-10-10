//---------------------------------------------------------------------------
#ifndef DiagramH
#define DiagramH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Menus.hpp>
#include <Dialogs.hpp>
#include <ToolWin.hpp>
#include <Buttons.hpp>
#include "DiagramCell.h"
#include "LogCollection.h"
//---------------------------------------------------------------------------
class TDiagramForm : public TForm
{
__published:	// IDE-managed Components
  TStatusBar  *StatusBar;
  TScrollBox  *ScrollBox;
  TPanel      *Panel;
  TToolBar    *ToolBar;
  TButton     *ButtonMode1;
  TButton     *ButtonMode2;
  TPopupMenu  *PopupMenu;
  TMenuItem   *CopyMenuItem;
  TBitBtn     *InfoButton;
  TCheckBox   *ReadOnlyCheckBox;
  TSpeedButton *SpeedButton1;
  TSpeedButton *SpeedButton2;
  TSpeedButton *SpeedButton3;
  TSpeedButton *SpeedButton1m;
  TSpeedButton *SpeedButton30s;
  TSpeedButton *SpeedButton10s;
  TCheckBox *WsTxCheckBox;
  void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
  void __fastcall FormShow(TObject *Sender);
  void __fastcall ScrollBoxMouseWheel(TObject *Sender, TShiftState Shift,
          int WheelDelta, TPoint &MousePos, bool &Handled);
  void __fastcall Button10sClick(TObject *Sender);
  void __fastcall Button30sClick(TObject *Sender);
  void __fastcall Button1mClick(TObject *Sender);
  void __fastcall ShapeMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
  void __fastcall ButtonMode1Click(TObject *Sender);
  void __fastcall ButtonMode2Click(TObject *Sender);
  void __fastcall CopyMenuItemClick(TObject *Sender);
  void __fastcall InfoButtonClick(TObject *Sender);
  void __fastcall ReadOnlyCheckBoxClick(TObject *Sender);
  void __fastcall WsTxCheckBoxClick(TObject *Sender);
private:	// User declarations
  DiagramCells cells;
  TShape* lastSelected;
  TLabel* timeLabel;
  int intervalDay;
  int intervalHour;
  int scale;
  int mode;
  TColor lastSelectedColor;

  void initCellsList(int size);
  bool findRealUser(DiagramCell *rec);
  bool rebuild(LogInterval *interval);
public:		// User declarations
  __fastcall TDiagramForm(TComponent* Owner);
  void BuildDiagram();
  bool isRealUser(AnsiString &user);
};
//---------------------------------------------------------------------------
extern PACKAGE TDiagramForm *DiagramForm;
//---------------------------------------------------------------------------
#endif
