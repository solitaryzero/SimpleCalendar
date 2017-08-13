#include "eventdatabase.h"

eventData::eventData()
{
    eventName = "null";
    eventContent = "null";
    eventType = 0;
    eventPeriod = 0;
    eventStartDate = QDate(1,1,1);
    id = -1;
}

eventData::eventData(QString name,QString content,int type,QDate date, int eid, int period)
{
    eventName = name;
    eventContent = content;
    eventType = type;
    eventPeriod = period;
    eventStartDate = date;
    id = eid;
}

eventDatabase::eventDatabase(QObject *parent):
    QObject(parent)
{
    singleData = QList<eventData>();
    periodData = QList<eventData>();
    bannedData = QList<eventData>();
    singleDataMaxId = 0;
    periodDataMaxId = 100000;
}

eventData eventDatabase::addSingleEvent(QString name,QString content,QDate date,int period, int newid)
{
    if (newid == -1) newid = singleDataMaxId+1;
    eventData newEvent(name,content,1,date,newid,0);
    singleData.append(newEvent);
    singleDataMaxId++;
    return newEvent;
}

eventData eventDatabase::addPeriodEvent(QString name,QString content,QDate date,int period,int newid)
{
    if (newid == -1) newid = periodDataMaxId+1;
    eventData newEvent(name,content,2,date,newid,period);
    periodData.append(newEvent);
    periodDataMaxId++;
    return newEvent;
}

eventData eventDatabase::addBannedEvent(QString name,QString content,QDate date,int period,int newid)
{
    if (newid == -1) newid = periodDataMaxId+1;
    eventData newEvent(name,content,3,date,newid,0);
    bannedData.append(newEvent);
    return newEvent;
}

void eventDatabase::addEvent(const eventData &ev,int id)
{
    if (ev.eventType == 1)
    {
        addSingleEvent(ev.eventName,ev.eventContent,ev.eventStartDate,ev.eventPeriod,id);
        return;
    }

    if (ev.eventType == 2)
    {
        addPeriodEvent(ev.eventName,ev.eventContent,ev.eventStartDate,ev.eventPeriod,id);
        return;
    }

    if (ev.eventType == 3)
    {
        addBannedEvent(ev.eventName,ev.eventContent,ev.eventStartDate,ev.eventPeriod,id);
        return;
    }
}

void eventDatabase::editSingleEvent(const eventData &ev, const QDate &currentDate)
{
    int i;
    if (ev.eventType == 1)
    {
        for (i=0;i<singleData.size();i++)
        {
            if (ev.id == singleData[i].id)
            {
                singleData[i] = ev;
                break;
            }
        }
        return;
    }

    if (ev.eventType == 2)
    {
        eventData newEvent(ev.eventName,ev.eventContent,3,currentDate,ev.id,0);
        bannedData.append(newEvent);
        eventData newEvent1(ev.eventName,ev.eventContent,1,currentDate,ev.id,0);
        singleData.append(newEvent1);
        return;
    }
}

void eventDatabase::editPeriodEvent(const eventData &ev)
{
    int i;
    for (i=0;i<periodData.size();i++)
    {
        if (ev.id == periodData[i].id)
        {
            periodData[i] = ev;
            break;
        }
    }
}

void eventDatabase::deleteEvent(const eventData &delData,const QDate &currentDate)
{
    int i,type;
    type = delData.eventType;

    if (type==1)
    {
        for (i=0;i<singleData.size();i++)
        {
            if (singleData[i].id == delData.id) break;
        }

        if (i>=singleData.size())
        {
            qDebug() << "bug";
            return;
        }

        singleData.removeAt(i);
        return;
    }

    if (type==2)
    {
        eventData newEvent(delData.eventName,delData.eventContent,1,currentDate,delData.id,0);
        bannedData.append(newEvent);
        return;
    }
}

void eventDatabase::deletePeriodEvent(const eventData &delData,const QDate &currentDate)
{
    int i;

    for (i=0;i<periodData.size();i++)
    {
        if (periodData[i].id == delData.id) break;
    }

    if (i>=periodData.size())
    {
        qDebug() << "bug";
        return;
    }

    periodData.removeAt(i);

    for (i=0;i<bannedData.size();i++)
    {
        if (bannedData[i].id == delData.id)
        {
            bannedData.removeAt(i);
        }
    }

    for (i=0;i<singleData.size();i++)
    {
        if (singleData[i].id == delData.id)
        {
            singleData.removeAt(i);
        }
    }

}

QList<eventData> eventDatabase::getEvents(const QDate &date)
{
    QList<eventData> result;
    int i,j;
    for (i=0;i<singleData.size();i++)
    {
        if (singleData[i].eventStartDate == date)
        {
            result.append(singleData[i]);
        }
    }

    for (i=0;i<periodData.size();i++)
    {
        if ((periodData[i].eventPeriod == -1) &&
            (periodData[i].eventStartDate.day() == date.day()))
        {
            bool flag = true;
            for (j=0;j<bannedData.size();j++)
            {
                if ( (bannedData[j].eventStartDate == date) && (bannedData[j].id == periodData[i].id) )
                {
                    flag = false;
                    break;
                }
            }
            if (flag) result.append(periodData[i]);
            continue;
        }

        if (periodData[i].eventPeriod == -1) continue;

        if ((periodData[i].eventStartDate.daysTo(date)>=0) &&
            (periodData[i].eventStartDate.daysTo(date)%periodData[i].eventPeriod == 0))
        {
            bool flag = true;
            for (j=0;j<bannedData.size();j++)
            {
                if ((bannedData[j].eventStartDate == date) && (bannedData[j].id == periodData[i].id) )
                {
                    flag = false;
                    break;
                }
            }
            if (flag) result.append(periodData[i]);
            continue;
        }
    }

    return result;
}

eventDatabase *mainEventDatabase;
