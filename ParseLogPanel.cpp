//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "LogRecord.h"
#include "ParseLogPanel.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

TParseLogForm *ParseLogForm;
//---------------------------------------------------------------------------
__fastcall TParseLogForm::TParseLogForm(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TParseLogForm::CancelButtonClick(TObject *Sender)
{
    processing = false;
}
//---------------------------------------------------------------------------
LogRecord * TParseLogForm::createRecord(char *buffer)
{
  try {
    LogRecord *rec = LogRecord::parse(buffer);
    return rec;
  } catch(Exception& ignore) {
    // TODO
  }
  return NULL;
}
//---------------------------------------------------------------------------
void TParseLogForm::load(FILE *fp, int scale, LogCollection& records,
                         UserNames& users, UserIndexes& userIndex)
{
    long fileSize = filesize(fp);
    bool overflow = false;
    if(fileSize == 0) {
      fileSize = 2000000000L;
      overflow = true;
    }
    long currentSize = 0;
    ProgressBar->Position = 0;
    long step = fileSize/10000;
    if(step < 1) {
      step = 1;
      ProgressBar->Max = fileSize;
    } else {
      ProgressBar->Max = 10001;
    }
    ProcessName->Caption = "Parse ...";
    Show();
    processing = true;
    char buffer[401];
    int read = 0;
    int write = 0;
    int wait = 0;
    Users->Caption = "";
    Records->Caption = "";
    Read->Caption = "";
    Write->Caption = "";
    Wait->Caption = "";
    AnsiString autoSynchUser = AnsiString("auto.synch");
    while(fgets(buffer, 400, fp) && processing) {
        int last = currentSize/step;
        currentSize += strlen(buffer);
        if(overflow) {
          ProgressBar->Position = currentSize/10/step;
        } else {
          ProgressBar->Position = currentSize/step;
        }
        LogRecord *rec = createRecord(buffer);
        if(rec != NULL) {
            if(rec->getUser() == autoSynchUser) {
              // skip autoSynchUser
              delete rec;
            } else {
              records.add(rec);
              if(rec->isRead()) read++;
              else write++;
              if(rec->getWait() > 0) wait++;
              users.insert(rec->getUser());

              if(last != ProgressBar->Position) {
                Users->Caption = users.size();
                Records->Caption = records.size();
                Read->Caption = read;
                Write->Caption = write;
                Wait->Caption = wait;
              }
            }
        }
        Application->ProcessMessages();
    }
    // sort
    records.sort();

    Users->Caption = users.size();
    Records->Caption = records.size();
    Read->Caption = read;
    Write->Caption = write;
    Wait->Caption = wait;

    int i = 0;
    for (UserNames::iterator it=users.begin(); it != users.end(); it++, i++) {
        AnsiString user= *it;
        TLabel *label = new TLabel(Main->UserPanel);
        label->Parent = Main->UserPanel;
        label->Tag = i;
        label->Caption = user + " ";
        label->Alignment = taRightJustify;
        label->Left = 0;
        label->Top = i*s_rowSize;
        label->Align = alTop;
        label->ParentFont = false;
        label->Font->Height = s_rowSize;
        userIndex[user] = label;
   }
    Main->LinePanel->Height = i*s_rowSize+20;
    Hide();
}
//---------------------------------------------------------------------------
long TParseLogForm::filesize(FILE *stream)
{
   long curpos, length;

   curpos = ftell(stream);
   fseek(stream, 0L, SEEK_END);
   length = ftell(stream);
   fseek(stream, curpos, SEEK_SET);
   return length;
}
//---------------------------------------------------------------------------
