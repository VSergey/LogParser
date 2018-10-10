//---------------------------------------------------------------------------
#ifndef LogRecordH
#define LogRecordH

//---------------------------------------------------------------------------
class DateTime
{
private:
    long o_days;          // count of full days
    long o_milliseconds;  // count of milliseconds in non full day

public:
    DateTime() { o_days = 0; o_milliseconds = 0; }
    // parse string like this
    // 2006-03-06 05:10:47,255
    DateTime(char *);
    DateTime(long d, long ms);
    DateTime(const DateTime& date);

    long day() const { return o_days; }
    long milliseconds() const { return o_milliseconds; }
    int hours() const;

    bool operator ==(const DateTime& date) const;
    bool operator !=(const DateTime& date) const;
    bool operator < (const DateTime& date) const;
    bool operator <=(const DateTime& date) const;
    bool operator > (const DateTime& date) const;
    bool operator >=(const DateTime& date) const;

    DateTime operator +(long ms) const;
    DateTime operator -(long ms) const;
    DateTime operator -(const DateTime& date) const;
};
//---------------------------------------------------------------------------
class LogRecord
{
private:
    AnsiString txNumber;
    long wait, exec;
    long gets, puts;
    AnsiString user;
    AnsiString action;
    DateTime startTime;
    DateTime finishTime;
    bool readAction;

    AnsiString timeString(long time) const;
    static char * readTxNumber(char *s, char *buffer);

public:
    static LogRecord * parse(char *str);
    LogRecord(AnsiString txNumber, long wait, long exec, long gets, long puts, AnsiString user,
        AnsiString action, DateTime startTime, DateTime finishTime, bool readAction);
    ~LogRecord() {}
    bool operator == (const LogRecord& rec) const;
    bool operator < (const LogRecord& rec) const;
    bool operator <= (const LogRecord& rec) const;
    bool operator > (const LogRecord& rec) const;
    bool operator >= (const LogRecord& rec) const;

    long getWait() const           { return wait; }
    long getExec() const           { return exec; }
    long getGets() const           { return gets; }
    long getPuts() const           { return puts; }
    AnsiString getTxNumber() const { return txNumber; }
    bool isRead() const            { return readAction; }
    AnsiString getUser() const     { return user; }
    AnsiString getAction() const   { return action; }
    DateTime getStartTime() const  { return startTime; }
    DateTime getFinishTime() const { return finishTime; }
    long getLength() const         { return (finishTime - startTime).milliseconds(); }
    AnsiString convertStartTime()  const { return timeString(startTime.milliseconds()); }
    AnsiString convertFinishTime() const { return timeString(finishTime.milliseconds()); }
};

// Ascending date sorting function
struct SAscendingDateSort
{
     bool operator()(const LogRecord* rec1, const LogRecord* rec2) {
          return *rec1 < *rec2;
     }
};

//---------------------------------------------------------------------------
#endif
