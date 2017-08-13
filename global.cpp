#include "global.h"
#include <cstring>
#include <fstream>
#include <QFile>
#include <QTextStream>
#include <QString>
#include <QDir>

global::global()
{

}

void global::init()
{
    mainEventDatabase = new eventDatabase();
    eventAddDialog = new eventAdder(QDate::currentDate());
    mainFileViewer = new fileViewer();
    mainColorManager = new colorManager();

    std::fstream fin;
    int type,y,m,d,period,id,r,g,b;
    std::string name,content,color;

    fin.open("events.txt");
    while (fin >> type >> y >> m >> d >> period >> id)
    {
        getline(fin,name);
        getline(fin,name);
        getline(fin,content);
        mainEventDatabase->addEvent(eventData(QString::fromStdString(name),QString::fromStdString(content),type,QDate(y,m,d),id,period));
    }
    fin.close();

    fin.open("filenames.txt");
    while (fin >> y >> m >> d)
    {
        getline(fin,name);
        getline(fin,name);
        mainFileViewer->addFile(QString::fromStdString(name),QDate(y,m,d));
    }
    fin.close();

    fin.open("colors.txt");
    while (fin >> y >> m >> d >> r >> g >> b)
    {
        mainColorManager->addColor(QColor(r,g,b),QDate(y,m,d));
    }
    fin.close();
}

void global::record()
{
    int i;

    QFile f1("events.txt");
    f1.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream txtOutput1(&f1);
    eventData temp;
    for (i=0;i<mainEventDatabase->singleData.size();i++)
    {
        temp = mainEventDatabase->singleData[i];
        txtOutput1 << temp.eventType << " " << temp.eventStartDate.year() << " " << temp.eventStartDate.month()
                   << " " << temp.eventStartDate.day() << " " << temp.eventPeriod << " " << temp.id << endl;
        txtOutput1 << temp.eventName << endl;
        txtOutput1 << temp.eventContent <<endl;
    }
    for (i=0;i<mainEventDatabase->periodData.size();i++)
    {
        temp = mainEventDatabase->periodData[i];
        txtOutput1 << temp.eventType << " " << temp.eventStartDate.year() << " " << temp.eventStartDate.month()
                   << " " << temp.eventStartDate.day() << " " << temp.eventPeriod << " " << temp.id << endl;
        txtOutput1 << temp.eventName << endl;
        txtOutput1 << temp.eventContent <<endl;
    }
    for (i=0;i<mainEventDatabase->bannedData.size();i++)
    {
        temp = mainEventDatabase->bannedData[i];
        txtOutput1 << temp.eventType << " " << temp.eventStartDate.year() << " " << temp.eventStartDate.month()
                   << " " << temp.eventStartDate.day() << " " << temp.eventPeriod << " " << temp.id << endl;
        txtOutput1 << temp.eventName << endl;
        txtOutput1 << temp.eventContent <<endl;
    }
    f1.close();

    QFile f2("filenames.txt");
    f2.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream txtOutput2(&f2);
    fileData temp1;
    for (i=0;i<mainFileViewer->fileNames.size();i++)
    {
        temp1 = mainFileViewer->fileNames[i];
        txtOutput2 << temp1.time.year() << " " << temp1.time.month() << " " << temp1.time.day() << endl;
        txtOutput2 << temp1.fileName << endl;
    }
    f2.close();

    QFile f3("colors.txt");
    f3.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream txtOutput3(&f3);
    colorData temp2;
    for (i=0;i<mainColorManager->colors.size();i++)
    {
        temp2 = mainColorManager->colors[i];
        txtOutput3 << temp2.colorDate.year()<< " " << temp2.colorDate.month() << " " << temp2.colorDate.day() << " ";
        txtOutput3 << temp2.color.red() << " " << temp2.color.green() << " " << temp2.color.blue() << endl;
    }
    f3.close();
}

global gObject;
