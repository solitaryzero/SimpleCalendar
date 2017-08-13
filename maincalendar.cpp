#include "maincalendar.h"

mainCalendar::mainCalendar(QWidget *parent, int year, int month):
    QWidget(parent),currentYear(year),currentMonth(month)
{
    QHBoxLayout *headLayout;
    QVBoxLayout *outerLayout;
    headLayout = new QHBoxLayout();
    prevMonth = new QPushButton("pm");
    displayedTime = new QLabel("test");
    nextMonth = new QPushButton("nm");
    headLayout->addWidget(prevMonth);
    headLayout->addWidget(displayedTime);
    headLayout->addWidget(nextMonth);

    outerLayout = new QVBoxLayout(this);
    cal = new QTableWidget(6,7);
    QStringList dayName;
    dayName << "Mon" << "Tue" << "Wed" << "Thur" << "Fri"
            << "Sat" << "Sun";
    cal->setHorizontalHeaderLabels(dayName);

    outerLayout->addLayout(headLayout);
    outerLayout->addWidget(cal);
}
