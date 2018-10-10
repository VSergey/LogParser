//---------------------------------------------------------------------------
#ifndef DiagramCellH
#define DiagramCellH

#include "LogCollection.h"
//---------------------------------------------------------------------------
typedef std::set<AnsiString> TxNames;
//---------------------------------------------------------------------------
class DiagramCell
{
private:
  UserNames users;
  TxNames txs;
  int idx;

public:
  DiagramCell(int i);
  ~DiagramCell();

  int index() const { return idx; }
  int usersCount() const { return users.size(); }
  int txCount() const { return txs.size(); }
  AnsiString userNames();

  void addUser(AnsiString& userName);
  void addTx(AnsiString& tx);
  UserNames::iterator usersBegin();
  UserNames::iterator usersEnd();
  TxNames::iterator txBegin();
  TxNames::iterator txEnd();
};
//---------------------------------------------------------------------------
typedef std::vector<DiagramCell*> DiagramCells;
//---------------------------------------------------------------------------
#endif
 