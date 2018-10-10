//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "DiagramCell.h"
//---------------------------------------------------------------------------
DiagramCell::DiagramCell(int i)
{
  idx = i;
}
//---------------------------------------------------------------------------
DiagramCell::~DiagramCell()
{
  txs.clear();
  users.clear();
}
//---------------------------------------------------------------------------
void DiagramCell::addUser(AnsiString& userName)
{
  users.insert(userName);
}
//---------------------------------------------------------------------------
void DiagramCell::addTx(AnsiString& tx)
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
UserNames::iterator DiagramCell::usersBegin()
{
  return users.begin();
}
//---------------------------------------------------------------------------
UserNames::iterator DiagramCell::usersEnd()
{
  return users.end();
}
//---------------------------------------------------------------------------
TxNames::iterator DiagramCell::txBegin()
{
  return txs.begin();
}
//---------------------------------------------------------------------------
TxNames::iterator DiagramCell::txEnd()
{
  return txs.end();
}
//---------------------------------------------------------------------------
#pragma package(smart_init)
 