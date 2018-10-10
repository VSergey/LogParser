//---------------------------------------------------------------------------
#ifndef DiagramCellH
#define DiagramCellH

#include <string>
#include <set>
#include <vector>
#include <sysdefs.h>

//---------------------------------------------------------------------------
typedef std::set<AnsiString> TxNames;
typedef std::set<AnsiString> Users;
//typedef std::set<std::string> Users;
//---------------------------------------------------------------------------
class DiagramCell
{
private:
  Users users;
  TxNames txs;
  int idx;

public:
  DiagramCell(int i);
  ~DiagramCell();

  int index() const { return idx; }
  int usersCount() const { return users.size(); }
  int txCount() const { return txs.size(); }
  AnsiString userNames();

  void addUser(AnsiString userName);
  void addTx(AnsiString tx);
  Users::iterator usersBegin();
  Users::iterator usersEnd();
  TxNames::iterator txBegin();
  TxNames::iterator txEnd();
};
//---------------------------------------------------------------------------
typedef std::vector<DiagramCell*> DiagramCells;
//---------------------------------------------------------------------------
#endif
 