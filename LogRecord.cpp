//---------------------------------------------------------------------------
#include <vcl.h>
#include <string.h>
#include <string.h>
#include <ctype.h>

#pragma hdrstop

#include "LogRecord.h"

const long s_hour = 36L*100000L;
const long s_maxMS = 24L*s_hour;

//---------------------------------------------------------------------------
// parse string like this
// 2006-03-06 05:10:47,255
DateTime::DateTime(char *str)
{
    char dayBuffer[9];      // from example
    dayBuffer[0] = str[0];  // 2
    dayBuffer[1] = str[1];  // 0
    dayBuffer[2] = str[2];  // 0
    dayBuffer[3] = str[3];  // 6
    dayBuffer[4] = str[5];  // 0
    dayBuffer[5] = str[6];  // 3
    dayBuffer[6] = str[8];  // 0
    dayBuffer[7] = str[9];  // 6
    dayBuffer[8] = 0x0;
    o_days = atol(dayBuffer);

    char timeBuffer[4];
    timeBuffer[0] = str[11]; // 0
    timeBuffer[1] = str[12]; // 5
    timeBuffer[2] = 0x0;
    int hours = atoi(timeBuffer);
    timeBuffer[0] = str[14]; // 1
    timeBuffer[1] = str[15]; // 0
    int minutes = atoi(timeBuffer) + hours*60;
    timeBuffer[0] = str[17]; // 4
    timeBuffer[1] = str[18]; // 7
    int seconds = atoi(timeBuffer) + minutes*60;
    timeBuffer[0] = str[20]; // 2
    timeBuffer[1] = str[21]; // 5
    timeBuffer[2] = str[22]; // 5
    timeBuffer[3] = 0x0;
    o_milliseconds = atoi(timeBuffer) + seconds *1000;
}
DateTime::DateTime(long d, long ms)
{
    o_days = d;
    o_milliseconds = ms;
}
DateTime::DateTime(const DateTime& date)
{
    o_days = date.o_days;
    o_milliseconds = date.o_milliseconds;
}
int DateTime::hours() const
{
    return o_milliseconds/s_hour;
}
bool DateTime::operator ==(const DateTime& date) const
{
    return o_days == date.o_days &&
        o_milliseconds == date.o_milliseconds;
}
bool DateTime::operator !=(const DateTime& date) const
{
    return o_days != date.o_days ||
        o_milliseconds != date.o_milliseconds;
}
bool DateTime::operator < (const DateTime& date) const
{
    if(o_days < date.o_days) return true;
    return o_milliseconds < date.o_milliseconds;
}
bool DateTime::operator <=(const DateTime& date) const
{
    if(o_days <= date.o_days) return true;
    return o_milliseconds <= date.o_milliseconds;
}
bool DateTime::operator > (const DateTime& date) const
{
    if(o_days > date.o_days) return true;
    return o_milliseconds > date.o_milliseconds;
}
bool DateTime::operator >=(const DateTime& date) const
{
    if(o_days >= date.o_days) return true;
    return o_milliseconds >= date.o_milliseconds;
}
DateTime DateTime::operator +(long ms) const
{
    long nms = o_milliseconds + ms;
    if(nms < s_maxMS)
        return DateTime(o_days, nms);
    int d = nms / s_maxMS;
    nms -= nms % s_maxMS;
    return DateTime(o_days+d, nms);
}
DateTime DateTime::operator -(long ms) const
{
    long nms = o_milliseconds - ms;
    if(nms > 0)
        return DateTime(o_days, nms);
    int d = nms / s_maxMS;
    nms += nms % s_maxMS;
    return DateTime(o_days-d, nms);
}
DateTime DateTime::operator -(const DateTime& date) const
{
    long d = o_days - date.o_days;
    long ms = o_milliseconds - date.o_milliseconds;
    if(ms <= 0) {
        if(d <= 0)
            return DateTime(d, ms);
        return DateTime(d-1, ms+s_maxMS);
    } else {
        if(d >= 0)
            return DateTime(d, ms);
        return DateTime(d+1, ms-s_maxMS);
    }
}
//---------------------------------------------------------------------------
char * LogRecord::readTxNumber(char *s, char *buffer)
{
      int i;
      // read tx number
      for(i=0; isdigit(*s); s++) {
        buffer[i++] = *s;
      }
      if(*s == ':')  {
        buffer[i++] = *(s++);
        for(; isdigit(*s); s++) {
          buffer[i++] = *s;
        }
      }
      buffer[i] = 0x0;
      return buffer;
}
//---------------------------------------------------------------------------
LogRecord * LogRecord::parse(char *str)
{
    int i;
    // try create record from aspect 16.2 log
    char *s = strstr(str, "[TransactionQueue]");
    if(s != NULL) {
      // move pointer after substring '[TransactionQueue] '
      s+= 19;

      char *pdone = strstr(s, "done");
      if(!pdone) return NULL;
      pdone += 6;
      boolean readAction = ('R' == *pdone);

      DateTime finishTime = DateTime(str);

      // read tx number
      char buffer[100];
      AnsiString txNumber = readTxNumber(s, buffer);

      char *puser = strstr(pdone, "user");
      if(!puser) throw Exception("invalid row: can't found 'user'");
      puser += 6;

      char *pwait = strstr(puser, "wait");
      if(!pwait) throw Exception("invalid row: can't found 'wait'");
      strncpy(buffer, puser, pwait - puser - 1);
      buffer[pwait - puser - 1] = 0x0;
      AnsiString user = AnsiString(buffer);
      pwait += 6;
      for(i=0; isdigit(*pwait); i++,pwait++) {
        buffer[i] = *pwait;
      }
      buffer[i] = 0x0;
      long wait = atol(buffer);

      char *pexec = strstr(pwait, "exec");
      if(!pexec) throw Exception("invalid row: can't found 'exec'");
      pexec += 6;
      for(i=0; isdigit(*pexec); i++,pexec++) {
        buffer[i] = *pexec;
      }
      buffer[i] = 0x0;
      long exec = atol(buffer);

      char *paction = pexec;
      char *pgets = strstr(paction, "gets");
      long gets = 0;
      if(pgets) {
        pgets += 6;
        for(i=0; isdigit(*pgets); i++,pgets++) {
            buffer[i] = *pgets;
        }
        buffer[i] = 0x0;
        gets = atol(buffer);
        paction = pgets;
      }

      char *pputs = strstr(paction, "puts");
      long puts = 0;
      if(pputs) {
        pputs += 6;
        for(i=0; isdigit(*pputs); i++,pputs++) {
            buffer[i] = *pputs;
        }
        buffer[i] = 0x0;
        puts = atol(buffer);
        paction = pputs;
      }
      int l = strlen(paction);
      if(l > 0 && paction[l-1]=='\n') paction[l-1] = 0x0;
      AnsiString action = AnsiString(paction);
      DateTime startTime = finishTime-exec;

      return new LogRecord(txNumber, wait, exec, gets, puts, user, action, startTime, finishTime, readAction);
    } else {
      // try create record from aspect 10.2.4 log
      s = strstr(str, "[TxMonitor]");
      if(s != NULL) {
        // move pointer after substring '[TxMonitor] '
        s+=12;
        char *ts = strstr(str, "[java]");
        if(ts == NULL)
          ts = str;
        else
          ts+=7; // skip '[java] '
        DateTime finishTime = DateTime(ts);

        // read tx number
        char buffer[100];
        AnsiString txNumber = readTxNumber(s, buffer);

        char *pdone = strstr(s, "done");
        if(!pdone) throw Exception("invalid row: can't found 'done'");
        pdone += 6;
        boolean readAction = ('R' == *pdone);

        char *puser = strstr(pdone, "user");
        if(!puser) throw Exception("invalid row: can't found 'user'");
        puser += 6;

        char *pwait = strstr(puser, "wait");
        if(!pwait) throw Exception("invalid row: can't found 'wait'");
        strncpy(buffer, puser, pwait - puser - 1);
        buffer[pwait - puser - 1] = 0x0;
        AnsiString user = AnsiString(buffer);
        pwait += 6;
        for(i=0; isdigit(*pwait); i++,pwait++) {
          buffer[i] = *pwait;
        }
        buffer[i] = 0x0;
        long wait = atol(buffer);

        char *pexec = strstr(pwait, "exec");
        if(!pexec) throw Exception("invalid row: can't found 'exec'");
        pexec += 6;
        for(i=0; isdigit(*pexec); i++,pexec++) {
          buffer[i] = *pexec;
        }
        buffer[i] = 0x0;
        long exec = atol(buffer);

        char *paction = pexec;
        char *pgets = strstr(paction, "gets");
        long gets = 0;
        if(pgets) {
          pgets += 6;
          for(i=0; isdigit(*pgets); i++,pgets++) {
            buffer[i] = *pgets;
          }
          buffer[i] = 0x0;
          gets = atol(buffer);
          paction = pgets;
        }

        char *pputs = strstr(paction, "puts");
        long puts = 0;
        if(pputs) {
          pputs += 6;
          for(i=0; isdigit(*pputs); i++,pputs++) {
            buffer[i] = *pputs;
          }
          buffer[i] = 0x0;
          puts = atol(buffer);
          paction = pputs;
        }

        AnsiString action = AnsiString(paction);
        DateTime startTime = finishTime-exec;

        return new LogRecord(txNumber, wait, exec, gets, puts, user, action, startTime, finishTime, readAction);
      }
    }
    return NULL;
}
//---------------------------------------------------------------------------
LogRecord::LogRecord(AnsiString txNumber, long wait, long exec, long gets, long puts, AnsiString user,
                     AnsiString action, DateTime startTime, DateTime finishTime, bool readAction)
{
  this->txNumber = txNumber;
  this->wait = wait;
  this->exec = exec;
  this->gets = gets;
  this->puts = puts;
  this->user = user;
  this->action = action;
  this->startTime = startTime;
  this->finishTime = finishTime;
  this->readAction = readAction;
}
bool LogRecord::operator == (const LogRecord& rec) const
{
    if(this == &rec) return true;
    return startTime == rec.startTime;
}
bool LogRecord::operator < (const LogRecord& rec) const
{
    return startTime < rec.startTime;
}
bool LogRecord::operator <= (const LogRecord& rec) const
{
    return startTime <= rec.startTime;
}
bool LogRecord::operator > (const LogRecord& rec) const
{
    return startTime > rec.startTime;
}
bool LogRecord::operator >= (const LogRecord& rec) const
{
    return startTime >= rec.startTime;
}
//---------------------------------------------------------------------------
AnsiString LogRecord::timeString(long time) const
{
    int ms = time%1000;
    time-= ms;
    time /= 1000;
    int sec = time%60;
    time-= sec;
    time /= 60;
    int min = time%60;
    time-= min;
    time /= 60;
    int hour = time;
    return AnsiString(hour)+":"+AnsiString(min)+":"+AnsiString(sec)+","+AnsiString(ms);
}
//---------------------------------------------------------------------------
#pragma package(smart_init)
