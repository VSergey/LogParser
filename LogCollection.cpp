//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "LogCollection.h"

//---------------------------------------------------------------------------
LogInterval::LogInterval(int day, int hour)
{
    intervalHour = hour;
    intervalDay = day;
}
LogInterval::~LogInterval()
{
    clear();
}
void LogInterval::sort()
{
    std::sort(records.begin(), records.end(), SAscendingDateSort());
}
void LogInterval::clear()
{
    for (RecordsArray::iterator it=records.begin(); it != records.end(); it++)
        delete *it;
    records.clear();
}
RecordsArray::iterator LogInterval::begin()
{
    return records.begin();
}
RecordsArray::iterator LogInterval::end()
{
    return records.end();
}
const LogRecord* LogInterval::getRecord(int index) const
{
    return records[index];
}
void LogInterval::add(const LogRecord* rec)
{
    records.push_back(rec);
}
//---------------------------------------------------------------------------
LogCollection::LogCollection()
{
    count=0;
}
LogCollection::~LogCollection()
{
    clear();
}
LogInterval* LogCollection::ensureInterval(int day, int hour)
{
    LogInterval* interval;
    for(int i = list.size()-1; i >= 0; i--) {
        interval = getInterval(i);
        if(interval->day() == day && interval->hour() == hour)
            return interval;
        if(interval->day() < day ||
           interval->day() == day && interval->hour() < hour) {
            // insert new interval
            interval = new LogInterval(day, hour);
            list.insert(list.begin()+i+1, interval);
            return interval;
        }
    }
    interval = new LogInterval(day, hour);
    list.insert(list.begin(), interval);
    return interval;
}
void LogCollection::add(const LogRecord *rec)
{
    int startHour = rec->getStartTime().hour();
    int startDay = rec->getStartTime().day();
    int finishHour = rec->getFinishTime().hour();
    int finishDay = rec->getFinishTime().day();
    if(startDay==finishDay) {
        for(int i = startHour; i <= finishHour; i++) {
            LogInterval* interval = ensureInterval(startDay, i);
            interval->add(rec);
        }
    } else {
        for(int i = startHour; i < 24; i++) {
            LogInterval* interval = ensureInterval(startDay, i);
            interval->add(rec);
        }
        for(int d = startDay+1; d < finishDay; d++) {
            for(int i = 0; i < 24; i++) {
                LogInterval* interval = ensureInterval(d, i);
                interval->add(rec);
            }
        }
        for(int i = 0; i <= finishHour; i++) {
            LogInterval* interval = ensureInterval(finishDay, i);
            interval->add(rec);
        }

    }
    count++;
}
const LogRecord* LogCollection::getRecord(int interval, int index) const
{
    LogInterval* i = getInterval(interval);
    return i->getRecord(index);
}
LogInterval* LogCollection::getInterval(int interval) const
{
    return list[interval];
}
void LogCollection::sort()
{
    for (RecordsCollection::iterator it=list.begin(); it != list.end(); it++) {
        LogInterval* interval = *it;
        interval->sort();
    }
}
void LogCollection::clear()
{
    for (RecordsCollection::iterator it=list.begin(); it != list.end(); it++) {
        LogInterval* interval = *it;
        interval->clear();
    }
    list.clear();
}
//---------------------------------------------------------------------------
#pragma package(smart_init)




