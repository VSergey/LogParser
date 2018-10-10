//---------------------------------------------------------------------------
#include <vcl.h>
#include <Clipbrd.hpp>
#pragma hdrstop

#include "Diagram.h"
#include "MainForm.h"
#include "FormInfo.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

TDiagramForm *DiagramForm;
long oneHourMillis = 3600000;
int width = 6;
int y = 200;
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
__fastcall TDiagramForm::TDiagramForm(TComponent* Owner)
  : TForm(Owner)
{
  scale = 30000; // 30 sec
  mode = 0; // by users
  lastSelected = NULL;
  timeLabel = NULL;
}
//---------------------------------------------------------------------------
void __fastcall TDiagramForm::FormClose(TObject *Sender,
      TCloseAction &Action)
{
  Hide();
}
//---------------------------------------------------------------------------
void __fastcall TDiagramForm::FormShow(TObject *Sender)
{
  BuildDiagram();
}
//---------------------------------------------------------------------------
void TDiagramForm::initCellsList(int size)
{
  for (DiagramCells::iterator it=cells.begin(); it != cells.end(); it++) {
    DiagramCell *rec = *it;
    if(rec) {
      delete rec;
      *it = NULL;
    }
  }
  cells.clear();
  for(int i = 0; i<size; i++)
    cells.push_back(NULL);
}
//---------------------------------------------------------------------------
bool TDiagramForm::rebuild(LogInterval *interval)
{
  if(!interval) return false;
  initCellsList(oneHourMillis/scale); // by 10 sec in hour
  intervalDay = interval->day();
  intervalHour = interval->hour();
  const LogRecord* firstRecord = *interval->begin();
  long start = 0;
  if(firstRecord) {
    DateTime firstTime = firstRecord->getStartTime();
    start = firstTime.milliseconds(); // start of hour
  }
  for (RecordsArray::iterator it=interval->begin(); it != interval->end(); it++) {
    const LogRecord *rec= *it;

    long startTime = rec->getStartTime().milliseconds() - start;
    long endTime = rec->getFinishTime().milliseconds() - start;
    int startIndex = startTime / scale;
    int endIndex = endTime / scale;
    if(endTime % scale > 0) endIndex++;
    for(int i = startIndex; i <= endIndex; i++) {
      DiagramCell* cell = (cells.size()-i>0)? cells[i]: NULL;
      if(cell == NULL) {
        cell = new DiagramCell(i);
        cells[i] = cell;
      }
      cell->addUser(rec->getUser());
      cell->addTx(rec->getTxNumber());
    }
  }
  return true;
}
//---------------------------------------------------------------------------
void TDiagramForm::BuildDiagram()
{
  if(! rebuild(Main->getSelectedInterval())) return;
  TCursor oldCursor = Screen->Cursor;
  Screen->Cursor = crHourGlass;
  int max = 0;
  int shapeHeight = (mode == 0)? 10 : 4;
  try {
    Panel->Visible = false;
    lastSelected = NULL;
    timeLabel = NULL;
    StatusBar->SimpleText = "";
    Panel->DestroyComponents();
    for (DiagramCells::iterator it=cells.begin(); it != cells.end(); it++) {
      DiagramCell *rec= *it;
      if(rec) {
        TShape *shape = new TShape(Panel);
        shape->Parent = Panel;
        bool hasRealUsers = findRealUser(rec);
        shape->Brush->Color = hasRealUsers? clLime : clWhite;
        int height = (mode == 0)? rec->usersCount() : rec->txCount();
        if(max < height) max = height;
        height *= shapeHeight;
        shape->SetBounds(rec->index() * width, y - height, width, height);
        shape->Hint = AnsiString(rec->usersCount())+" users,  "+AnsiString(rec->txCount()) + " transactions";
        shape->ShowHint = true;
        shape->Tag = rec->index();
        shape->OnMouseDown = ShapeMouseDown;
      }
    }
    Panel->Width = cells.size()*width+5;
    TShape *line = new TShape(Panel);
    line->Parent = Panel;
    line->Brush->Color = clBlack;
    line->SetBounds(0, y+1, Panel->Width, 2);

    for(int i =0; i <= max+2; i++) {
      TShape *l = new TShape(Panel);
      l->Parent = Panel;
      l->Pen->Color = clLtGray;
      l->SetBounds(0, y-(i*shapeHeight), Panel->Width, 1);
      l->SendToBack();
    }
    Panel->Visible = true;
    Panel->Repaint();
  }
  __finally
  {
    Screen->Cursor = oldCursor;
  }
}
//---------------------------------------------------------------------------
bool TDiagramForm::findRealUser(DiagramCell *rec)
{
  for (UserNames::iterator it=rec->usersBegin(); it != rec->usersEnd(); it++) {
      AnsiString user = *it;
      if(user == "system") continue;
      if(user == "-") continue;
      if(user == "ws1") continue;
      if(user == "year.end") continue;
      int i = user.Pos(".ws");
      if(i != 0 && user.Length() > i+1) continue;
      return true;
  }
  return false;
}
//---------------------------------------------------------------------------
void __fastcall TDiagramForm::ScrollBoxMouseWheel(TObject *Sender,
      TShiftState Shift, int WheelDelta, TPoint &MousePos, bool &Handled)
{
  ScrollBox->HorzScrollBar->Position += WheelDelta;
  Handled = true;
  Panel->Repaint();
}
//---------------------------------------------------------------------------
void __fastcall TDiagramForm::Button10sClick(TObject *Sender)
{
  scale = 10000; // 10 sec
  unselectButtons();
  Button10s->Default = true;
  BuildDiagram();
}
//---------------------------------------------------------------------------
void __fastcall TDiagramForm::Button30sClick(TObject *Sender)
{
  scale = 30000; // 30 sec
  unselectButtons();
  Button30s->Default = true;
  BuildDiagram();
}
//---------------------------------------------------------------------------
void __fastcall TDiagramForm::Button1mClick(TObject *Sender)
{
  scale = 60000; // 1 min
  unselectButtons();
  Button1m->Default = true;
  BuildDiagram();
}
//---------------------------------------------------------------------------
void TDiagramForm::unselectButtons()
{
  Button10s->Default = false;
  Button30s->Default = false;
  Button1m->Default = false;
}
//---------------------------------------------------------------------------
void __fastcall TDiagramForm::ShapeMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
  TShape *shape = (TShape *)Sender;
  int index = shape->Tag;
  if(index < 0) return;
  if(lastSelected)  lastSelected->Brush->Color = lastSelectedColor;
  lastSelected = shape;
  lastSelectedColor = shape->Brush->Color;
  shape->Brush->Color = clBlue;
  DiagramCell *rec = cells[index];
  int sc = scale / 1000;
  int min = index * sc / 60;
  int sec = index * sc % 60;
  AnsiString label = "+ "+AnsiString(min)+" min, "+AnsiString(sec)+" sec,  ";
  if(timeLabel == NULL) {
    timeLabel = new TLabel(Panel);
    timeLabel->Parent = Panel;
  }
  timeLabel->Caption = label;
  timeLabel->Left = index * width;
  timeLabel->Top = y + 4;
  AnsiString users = AnsiString(rec->usersCount())+" users: "+rec->userNames()+" ";
  AnsiString tx = AnsiString(rec->txCount()) + " transactions";
  StatusBar->SimpleText = users + tx;
}
//---------------------------------------------------------------------------
void __fastcall TDiagramForm::ButtonMode1Click(TObject *Sender)
{
  if(mode != 0) {
    mode = 0;
    BuildDiagram();
  }
}
//---------------------------------------------------------------------------
void __fastcall TDiagramForm::ButtonMode2Click(TObject *Sender)
{
  if(mode != 1) {
    mode = 1;
    BuildDiagram();
  }
}
//---------------------------------------------------------------------------
void __fastcall TDiagramForm::CopyMenuItemClick(TObject *Sender)
{
  Graphics::TBitmap *bm = new Graphics::TBitmap();
  try {
    bm->Width = ClientWidth;
    bm->Height = ClientHeight;
    BitBlt(bm->Canvas->Handle, 0, 0, Panel->Width, Panel->Height-ToolBar->Height,
           Canvas->Handle, Panel->Left, Panel->Top+ToolBar->Height, SRCCOPY);
    Clipboard()->Assign(bm);
  }
  __finally
  {
    bm->FreeImage();
    delete bm;
  }
}
//---------------------------------------------------------------------------
void __fastcall TDiagramForm::InfoButtonClick(TObject *Sender)
{
  if(lastSelected) {
    int index = lastSelected->Tag;
    if(index < 0) return;
    DiagramCell *rec = cells[index];
    InfoForm->UsersList->Clear();
    for (UserNames::iterator it=rec->usersBegin(); it != rec->usersEnd(); it++) {
      AnsiString user = *it;
      InfoForm->UsersList->Items->Add(user);
    }
    InfoForm->TxList->Clear();
    LogInterval *interval = Main->getSelectedInterval();
    for (TxNames::iterator it=rec->txBegin(); it != rec->txEnd(); it++) {
      AnsiString txId= *it;
      for (RecordsArray::iterator it=interval->begin(); it != interval->end(); it++) {
        const LogRecord *rec= *it;
        if(txId == rec->getTxNumber()) {
          InfoForm->TxList->Items->Add(txId + " - " + rec->getUser() + " -> " + rec->getAction());
          break;
        }
      }
    }
    InfoForm->Show();
  }
}
//---------------------------------------------------------------------------

