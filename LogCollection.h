//---------------------------------------------------------------------------
#ifndef LogCollectionH
#define LogCollectionH

#include <vector>
#include <set>
#include <map>
#include "LogRecord.h"

typedef std::vector<const LogRecord*> RecordsArray;
typedef std::set<AnsiString> UserNames;
typedef std::map<AnsiString, TLabel*> UserIndexes;

class LogInterval
{
    int intervalDay;
    int intervalHour;
    RecordsArray records;
public:
    LogInterval(int day, int hour);
    ~LogInterval();
    int hour() const { return intervalHour; }
    int day() const { return intervalDay; }
    RecordsArray::iterator begin();
    RecordsArray::iterator end();
    void add(const LogRecord* rec);
    const LogRecord* getRecord(int index) const;
    void sort();
    void clear();
};

typedef std::vector<LogInterval*> RecordsCollection;

class LogCollection
{
private:
    long count;
    RecordsCollection list;

    LogInterval* ensureInterval(int day, int hour);

public:
    LogCollection();
    ~LogCollection();
    void add(const LogRecord* rec);
    const LogRecord* getRecord(int interval, int index) const;
    LogInterval* getInterval(int interval) const;
    void sort();
    long size() const { return count; }
    int intervals() const { return list.size(); }
    void clear();
};
//---------------------------------------------------------------------------
#endif
