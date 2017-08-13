#include "colormanager.h"

colorData::colorData()
{
    colorDate = QDate::currentDate();
    color = Qt::white;
}

colorData::colorData(QDate newDate,QColor newColor)
{
    colorDate = newDate;
    color = newColor;
}

colorManager::colorManager(QObject *parent) : QObject(parent)
{
    colors = QList<colorData>();
}

void colorManager::addColor(QColor newColor, QDate newDate)
{
    colors.append(colorData(newDate,newColor));
}

void colorManager::editColor(QColor newColor, QDate newDate)
{
    int i;
    for (i=0;i<colors.size();i++)
    {
        if (colors[i].colorDate == newDate) break;
    }
    colors[i].color = newColor;
}

void colorManager::deleteColor(QDate newDate)
{
    int i;
    for (i=0;i<colors.size();i++)
    {
        if (colors[i].colorDate == newDate) break;
    }
    colors.removeAt(i);
}

colorManager *mainColorManager;
