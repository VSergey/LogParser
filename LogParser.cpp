//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
USERES("LogParser.res");
USEFORM("MainForm.cpp", Main);
USEUNIT("LogRecord.cpp");
USEFORM("ParseLogPanel.cpp", ParseLogForm);
USEFORM("AboutPanel.cpp", AboutForm);
USEUNIT("LogCollection.cpp");
USEFORM("IntervalPanel.cpp", IntervalForm);
USEFORM("Diagram.cpp", DiagramForm);
USEUNIT("DiagramCell.cpp");
//---------------------------------------------------------------------------
extern LPSTR cmdLine;
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR line, int)
{
    cmdLine = line;
    try
    {
        Application->Initialize();
        Application->CreateForm(__classid(TMain), &Main);
     Application->CreateForm(__classid(TParseLogForm), &ParseLogForm);
     Application->CreateForm(__classid(TAboutForm), &AboutForm);
     Application->CreateForm(__classid(TIntervalForm), &IntervalForm);
     Application->CreateForm(__classid(TDiagramForm), &DiagramForm);
     Application->Run();
    }
    catch (Exception &exception)
    {
        Application->ShowException(&exception);
    }
    return 0;
}
//---------------------------------------------------------------------------
