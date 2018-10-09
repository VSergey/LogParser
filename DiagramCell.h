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
  DiagramCell(int i, AnsiString tx, AnsiString user);
  ~DiagramCell();

  int index() const { return idx; }
  int usersCount() const { return users.size(); }
  int txCount() const { return txs.size(); }
  void addUser(AnsiString userName);
  void addTx(AnsiString tx);
  AnsiString userNames();
};
//---------------------------------------------------------------------------
typedef std::vector<DiagramCell*> DiagramCells;
//---------------------------------------------------------------------------
#endif
 