#ifndef CALENDARWIDGET_H
#define CALENDARWIDGET_H

#include "calendarcell.h"
#include "eventadder.h"
#include "global.h"
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QPalette>

class calendarWidget : public QWidget
{
    Q_OBJECT

public:
    explicit calendarWidget(const QDate &currentDate, QWidget *parent = 0);
    QWidget *header;
    QPushButton *prevButton,*nextButton,*pinButton,*dragButton,*importButton,*exportButton;
    QLabel *timeHeader;
    calendarCell *calendarCells[7][8];
    QLabel *weekDay[8],*weekNum[7];
    QVBoxLayout *outerLayout;
    QHBoxLayout *headerLayout;
    QGridLayout *calendarLayout;

    QColor headerColor,weekDayColor,weekNumColor,emptyColor;

public slots:
    void prevMonth();
    void nextMonth();
    void judgeMonth(int i, int j);
    void calendarWidgetUpdate();
    void changeDragState();
    void cwimport();
    void cwexport();

private:
    const QString weekDayName[9]={"",tr("Mon"),tr("Tue"),tr("Wed"),tr("Thu"),tr("Fri"),tr("Sat"),tr("Sun")};
    QDate storedDate;
    bool isDragAble;
};

#endif // CALENDARWIDGET_H
