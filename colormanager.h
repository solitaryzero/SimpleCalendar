#ifndef COLORMANAGER_H
#define COLORMANAGER_H

#include <QObject>
#include <QWidget>
#include <QDate>
#include <QColor>
#include <QList>

class colorData
{
public:
    colorData();
    colorData(QDate newDate,QColor newColor);
    QDate colorDate;
    QColor color;
};

class colorManager : public QObject
{
    Q_OBJECT
public:
    explicit colorManager(QObject *parent = 0);
    QList<colorData> colors;

public slots:
    void addColor(QColor newColor, QDate newDate);
    void editColor(QColor newColor, QDate newDate);
    void deleteColor(QDate newDate);

};

extern colorManager *mainColorManager;

#endif // COLORMANAGER_H
