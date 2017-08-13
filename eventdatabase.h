#ifndef EVENTDATABASE_H
#define EVENTDATABASE_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QDate>
#include <QString>
#include <QDebug>
#include <QPushButton>
#include <QColor>
#include <QPalette>
#include <QList>
#include <iterator>

class eventData
{
public:
    eventData();
    eventData(QString name,QString content,int type,QDate date,int eid, int period=0);
    QString eventName,eventContent;
    int eventType,eventPeriod,id;
    QDate eventStartDate;
};

class eventDatabase : public QObject
{
    Q_OBJECT

public:
    explicit eventDatabase(QObject *parent = 0);
    int singleDataMaxId,periodDataMaxId;
    QList<eventData> getEvents (const QDate &date);
    QList<eventData> singleData,periodData,bannedData;

public slots:
    eventData addSingleEvent(QString name,QString content,QDate date,int period,int newid = -1);
    eventData addPeriodEvent(QString name,QString content,QDate date,int period,int newid = -1);
    eventData addBannedEvent(QString name,QString content,QDate date,int period,int newid = -1);
    void addEvent(const eventData &ev,int id = -1);
    void editSingleEvent(const eventData &ev, const QDate &currentDate);
    void editPeriodEvent(const eventData &ev);
    void deleteEvent(const eventData &delData,const QDate &currentDate);
    void deletePeriodEvent(const eventData &delData,const QDate &currentDate);

private:
};

extern eventDatabase *mainEventDatabase;

#endif // EVENTDATABASE_H
