//---------------------------------------------------------------------------
#include <vcl.h>
#include <algorithm>
#include <functional>
#pragma hdrstop

#include "MainForm.h"
#include "AboutPanel.h"
#include "ParseLogPanel.h"
#include "IntervalPanel.h"
#include "Diagram.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TMain *Main;
LPSTR cmdLine;
//---------------------------------------------------------------------------
__fastcall TMain::TMain(TComponent* Owner)
    : TForm(Owner)
{
    scale = 1000;
    selectedInterval = -1;
}
//---------------------------------------------------------------------------
void __fastcall TMain::OpenMenuItemClick(TObject *Sender)
{
    if(OpenDialog->Execute()) {
      loadFile(OpenDialog->FileName, false);
    }
}
//---------------------------------------------------------------------------
void __fastcall TMain::AppendMenuItemClick(TObject *Sender)
{
    if(OpenDialog->Execute()) {
      loadFile(OpenDialog->FileName, true);
    }
}
//---------------------------------------------------------------------------
void TMain::loadFile(AnsiString fileName, bool append)
{
    FILE *fp = fopen(fileName.c_str(), "r");
    if(fp==NULL) {
        AnsiString error = "Can't open file '"+fileName+"'";
        Application->MessageBox(error.c_str(), "File open error", MB_OK);
    } else {
        TCursor oldCursor = Screen->Cursor;
        Screen->Cursor = crHourGlass;
        try {
            if(!append) {
              clearRecors();
              users.clear();
            }
            Panel->Visible = false;
            IntervalForm->Clear();
            Panel->DestroyComponents();
            userIndex.clear();
            UserPanel->DestroyComponents();
            ParseLogForm->load(fp, scale, records, users, userIndex);
            currentFile = fileName;
            if(records.intervals() > 0) {
                selectedInterval = 0;
                SelectIntervalMenuItem->Enabled = true;
                buildChart();
                updateCaption();
            } else {
                selectedInterval = -1;
                SelectIntervalMenuItem->Enabled = false;
                Caption = currentFile;
            }
            Panel->Visible = true;
            Panel->Repaint();
            PrevIntervalMenuItem->Enabled = false;
            NextIntervalMenuItem->Enabled = records.intervals() > 1;
            fclose(fp);
        }
        __finally
        {
            Screen->Cursor = oldCursor;
        }
    }
}
//---------------------------------------------------------------------------
void TMain::updateCaption()
{
    LogInterval* interval = records.getInterval(selectedInterval);
    Caption = currentFile + "    interval: " +
        TIntervalForm::convertDay(interval->day()) + " - " + AnsiString(interval->hour()) + " h";
    TrCountLabel->Caption = AnsiString("Transactions: ") + AnsiString(cRecords);

}
//---------------------------------------------------------------------------
void __fastcall TMain::ExitMenuItemClick(TObject *Sender)
{
    Close();
}
//---------------------------------------------------------------------------
void TMain::clearRecors()
{
    records.clear();
    selectedInterval = -1;
}
//---------------------------------------------------------------------------
void __fastcall TMain::ShapeMouseDown(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
    int index = ((TComponent *)Sender)->Tag;
    if(index < 0) return;
    const LogRecord *rec = records.getRecord(selectedInterval, index);
    TxNumber->Caption = rec->getTxNumber();
    User->Caption = AnsiString(rec->getUser());
    Wait->Caption = convertLength(rec->getWait());
    Exec->Caption = convertLength(rec->getExec());
    Action->Caption = AnsiString(rec->getAction());
    Puts->Caption = AnsiString(rec->getPuts());
    Gets->Caption = AnsiString(rec->getGets());
    Start->Caption = rec->convertStartTime();
    End->Caption   = rec->convertFinishTime();
}
//---------------------------------------------------------------------------
AnsiString TMain::convertLength(long time)
{
    if(time < 1000) {
        return AnsiString(time);
    }
    return AnsiString(time/1000)+"s "+AnsiString(time%1000)+ "ms";
}
//---------------------------------------------------------------------------
void __fastcall TMain::Zoom10000ItemClick(TObject *Sender)
{
    scale = 10000;
    Zoom10000Item->Checked = true;
    rescaleDiagram();
}
//---------------------------------------------------------------------------
void __fastcall TMain::Zoom5000ItemClick(TObject *Sender)
{
    scale = 5000;
    Zoom5000Item->Checked = true;
    rescaleDiagram();
}
//---------------------------------------------------------------------------
void __fastcall TMain::Zoom1000ItemClick(TObject *Sender)
{
    scale = 1000;
    Zoom1000Item->Checked = true;
    rescaleDiagram();
}
//---------------------------------------------------------------------------
void __fastcall TMain::Zoom500ItemClick(TObject *Sender)
{
    scale = 500;
    Zoom500Item->Checked = true;
    rescaleDiagram();
}
//---------------------------------------------------------------------------
void __fastcall TMain::Zoom100ItemClick(TObject *Sender)
{
    scale = 100;
    Zoom100Item->Checked = true;
    rescaleDiagram();
}
//---------------------------------------------------------------------------
void __fastcall TMain::Zoom10ItemClick(TObject *Sender)
{
    scale = 10;
    Zoom10Item->Checked = true;
    rescaleDiagram();
}
//---------------------------------------------------------------------------
void __fastcall TMain::AboutMenuItemClick(TObject *Sender)
{
    AboutForm->ShowModal();
}
//---------------------------------------------------------------------------
void TMain::rescaleDiagram()
{
    TCursor oldCursor = Screen->Cursor;
    Screen->Cursor = crHourGlass;
    try {
      Panel->Visible = false;
      Panel->DestroyComponents();
      buildChart();
      if(DiagramForm->Visible) DiagramForm->BuildDiagram();
      Panel->Visible = true;
    }
    __finally
    {
      Screen->Cursor = oldCursor;
    }
}
//---------------------------------------------------------------------------
void __fastcall TMain::ScrollBoxMouseWheel(TObject *Sender,
      TShiftState Shift, int WheelDelta, TPoint &MousePos, bool &Handled)
{
    ScrollBox->HorzScrollBar->Position += WheelDelta;
    Handled = true;
    Panel->Repaint();
}
//---------------------------------------------------------------------------
void __fastcall TMain::SelectIntervalMenuItemClick(TObject *Sender)
{
    // select interval
    IntervalForm->init(records, selectedInterval);
    if(IntervalForm->ShowModal() == mrOk) {
        selectedInterval = IntervalForm->selectedIndex();
        NextIntervalMenuItem->Enabled = records.intervals()-1 > selectedInterval;
        PrevIntervalMenuItem->Enabled = selectedInterval > 0;
        rescaleDiagram();
        updateCaption();
    }
}
//---------------------------------------------------------------------------
void __fastcall TMain::NextIntervalMenuItemClick(TObject *Sender)
{
    if(records.intervals()-1 > selectedInterval) {
        selectedInterval++;
        PrevIntervalMenuItem->Enabled = true;
        NextIntervalMenuItem->Enabled = records.intervals()-1 > selectedInterval;
        rescaleDiagram();
        updateCaption();
    }
}
//---------------------------------------------------------------------------
void __fastcall TMain::PrevIntervalMenuItemClick(TObject *Sender)
{
    if(selectedInterval > 0) {
        selectedInterval--;
        NextIntervalMenuItem->Enabled = true;
        PrevIntervalMenuItem->Enabled = selectedInterval > 0;
        rescaleDiagram();
        updateCaption();
    }
}
//---------------------------------------------------------------------------
void TMain::buildChart()
{
    if(selectedInterval < 0) return;
    long lastX = 0;
    long skipX = 0;

    LogInterval* interval = records.getInterval(selectedInterval);
    int i = 0;
    long start = 0;
    const LogRecord* firstRecord = *interval->begin();
    if(firstRecord) {
        DateTime firstTime = firstRecord->getStartTime();
        start = firstTime.milliseconds();
    }
    // disable user labels
    for (UserNames::iterator iter=users.begin(); iter != users.end(); iter++) {
      AnsiString user= *iter;
      userIndex[user]->Enabled = false;
    }

    cRecords = 0;
    for (RecordsArray::iterator it=interval->begin(); it != interval->end(); it++, i++) {
        const LogRecord *rec= *it;
        cRecords++;
        int width = rec->getLength()/scale;
        if(width < 2) width = 2;
        long x = ((rec->getStartTime().milliseconds() - start) / scale) - skipX;
        if(lastX < x) {
            skipX += x - lastX;
            x = lastX;
        }
        if(lastX < x+width) lastX = x+width;

        TShape *shape = new TShape(Panel);
        shape->Parent = Panel;
        if(rec->isRead()) {
            if(rec->getWait() > 0)
                shape->Brush->Color = clYellow;
        } else {
            if(rec->getWait() > 0)
                shape->Brush->Color = clFuchsia;
            else
                shape->Brush->Color = clLime;
        }
        AnsiString userName = rec->getUser();
        TLabel *userLabel = userIndex[userName];
        userLabel->Enabled = true;
        int index = userLabel->Tag;
        int y = index*s_rowSize + 5;
        shape->SetBounds(x, y, width, 10);
        shape->Hint = rec->getAction()+" exec:"+AnsiString(rec->getLength());
        shape->ShowHint = true;
        shape->Tag = i;
        shape->OnMouseDown = ShapeMouseDown;
    }
    Panel->Width = lastX - skipX - start/scale;
}
//---------------------------------------------------------------------------
void __fastcall TMain::FormShow(TObject *Sender)
{
  if(cmdLine != NULL) {
    if(strlen(cmdLine) > 0) {
      loadFile(AnsiString(cmdLine), false);
    }
    cmdLine = NULL;
  }
}
//---------------------------------------------------------------------------
void __fastcall TMain::PanelMouseMove(TObject *Sender, TShiftState Shift, int X, int Y)
{
  StatusBar->SimpleText = "X: "+IntToStr(X) + " Y: "+IntToStr(Y);
}
//---------------------------------------------------------------------------
void __fastcall TMain::TrCountLabelClick(TObject *Sender)
{
  if(selectedInterval < 0) return;
  DiagramForm->Show();
}
//---------------------------------------------------------------------------

