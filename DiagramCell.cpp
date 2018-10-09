//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "DiagramCell.h"
//---------------------------------------------------------------------------
DiagramCell::DiagramCell(int i, AnsiString tx, AnsiString user)
{
  idx = i;
  txs.insert(tx);
  users.insert(user);
}
//---------------------------------------------------------------------------
DiagramCell::~DiagramCell()
{
  users.clear();
  txs.clear();
}
//---------------------------------------------------------------------------
void DiagramCell::addUser(AnsiString userName)
{
  users.insert(userName);
}
//---------------------------------------------------------------------------
void DiagramCell::addTx(AnsiString tx)
{
  txs.insert(tx);
}
//---------------------------------------------------------------------------
AnsiString DiagramCell::userNames()
{
  AnsiString result = "";
  for (UserNames::iterator it=users.begin(); it != users.end(); it++) {
    AnsiString name = *it;
    result += name + ", ";
  }
  return result;
}
//---------------------------------------------------------------------------
#pragma package(smart_init)
 