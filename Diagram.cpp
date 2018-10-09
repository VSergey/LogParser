//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Diagram.h"
#include "MainForm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

TDiagramForm *DiagramForm;
long oneHhourMillis = 3600000;
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
  clearCells();
  cells.resize(size);
  for(int i = 0; i<size; i++)
    cells[i] = NULL;
}
//---------------------------------------------------------------------------
bool TDiagramForm::rebuild(LogInterval *interval)
{
  if(!interval) return false;
  initCellsList(oneHhourMillis/scale+1); // by 10 sec in hour
  intervalDay = interval->day();
  intervalHour = interval->hour();
  const LogRecord* firstRecord = *interval->begin();
  long start = 0;
  if(firstRecord) {
    DateTime firstTime = firstRecord->getStartTime();
    start = firstTime.milliseconds()/oneHhourMillis*oneHhourMillis; // start of hour
  }
  for (RecordsArray::iterator it=interval->begin(); it != interval->end(); it++) {
    const LogRecord *rec= *it;

    AnsiString userName = rec->getUser();
    AnsiString tx = rec->getTxNumber();
    long startTime = rec->getStartTime().milliseconds() - start;
    long endTime = rec->getFinishTime().milliseconds() - start;
    int startIndex = startTime / scale;
    int endIndex = endTime / scale;
    if(endTime % scale > 0) endIndex++;
    for(int i = startIndex; i <= endIndex; i++) {
      DiagramCell* cell = cells[i];
      if(cell == NULL) {
        cells[i] = new DiagramCell(i, tx, userName);
      } else {
        cell->addUser(userName);
        cell->addTx(tx);
      }
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
      const DiagramCell *rec= *it;
      if(rec) {
        TShape *shape = new TShape(Panel);
        shape->Parent = Panel;
        shape->Brush->Color = clLime;
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
void TDiagramForm::clearCells()
{
  for (DiagramCells::iterator it=cells.begin(); it != cells.end(); it++) {
    DiagramCell *rec= *it;
    if(rec) delete rec;
  }
  cells.clear();
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
  if(lastSelected)  lastSelected->Brush->Color = clLime;
  lastSelected = shape;
  shape->Brush->Color = clBlue;
  DiagramCell *rec = cells[index];
  int sc = scale / 1000;
  int min = index * sc / 60;
  int sec = index * sc % 60;
  AnsiString label = AnsiString(min)+" min, "+AnsiString(sec)+" sec,  ";
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
void __fastcall TDiagramForm::Button3Click(TObject *Sender)
{
  if(mode != 1) {
    mode = 1;
    BuildDiagram();
  }
}
//---------------------------------------------------------------------------

