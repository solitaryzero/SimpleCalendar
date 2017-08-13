#ifndef MAINCALENDAR_H
#define MAINCALENDAR_H

#include <QWidget>
#include <QTableWidget>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QString>

class mainCalendar : public QWidget
{
    Q_OBJECT

public:
    mainCalendar(QWidget *parent, int year, int month);

public slots:
    //void calendarUpdate(int year, int month);

private:
    int currentYear,currentMonth;
    QLabel *displayedTime;
    QPushButton *prevMonth,*nextMonth;
    QTableWidget *cal;
};

#endif // MAINCALENDAR_H
