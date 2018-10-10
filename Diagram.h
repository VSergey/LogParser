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
#include "DiagramCell.h"
#include <ToolWin.hpp>
#include <Buttons.hpp>
//---------------------------------------------------------------------------
class TDiagramForm : public TForm
{
__published:	// IDE-managed Components
  TStatusBar  *StatusBar;
  TScrollBox  *ScrollBox;
  TPanel      *Panel;
  TToolBar    *ToolBar;
  TButton     *Button1m;
  TButton     *Button30s;
  TButton     *Button10s;
  TButton     *ButtonMode1;
  TButton     *Button2;
  TButton     *ButtonMode2;
  TPopupMenu  *PopupMenu;
  TMenuItem   *CopyMenuItem;
  TButton     *Button1;
  TBitBtn     *InfoButton;
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
private:	// User declarations
  DiagramCells cells;
  TShape* lastSelected;
  TLabel* timeLabel;
  int intervalDay;
  int intervalHour;
  int scale;
  int mode;
  TColor lastSelectedColor;

  bool rebuild(LogInterval *interval);
  void initCellsList(int size);
  void unselectButtons();
  bool findRealUser(DiagramCell *rec);
public:		// User declarations
  __fastcall TDiagramForm(TComponent* Owner);
  void BuildDiagram();
};
//---------------------------------------------------------------------------
extern PACKAGE TDiagramForm *DiagramForm;
//---------------------------------------------------------------------------
#endif
