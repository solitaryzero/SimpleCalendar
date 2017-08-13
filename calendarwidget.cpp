#include "calendarwidget.h"

calendarWidget::calendarWidget(const QDate &currentDate, QWidget *parent):
    QWidget(parent)
{
    storedDate = QDate(currentDate.year(),currentDate.month(),1);
    isDragAble = true;

    outerLayout = new QVBoxLayout();
    outerLayout->setContentsMargins(0,0,0,0);

//header
    header = new QWidget(this);
    headerLayout = new QHBoxLayout();
    headerLayout->addWidget(prevButton = new QPushButton(this));
    prevButton->setIcon(QPixmap(":/icons/previous.png"));
    prevButton->setFlat(true);
    headerLayout->addWidget(timeHeader = new QLabel(QString::number(storedDate.year())+" - "
                                                    +QString::number(storedDate.month())));
    timeHeader->setFont(QFont("Times",18));
    headerLayout->addWidget(nextButton = new QPushButton(this));
    nextButton->setIcon(QPixmap(":/icons/next.png"));
    nextButton->setFlat(true);
    headerLayout->addSpacerItem(new QSpacerItem(20,20,QSizePolicy::Expanding,QSizePolicy::Minimum));
    headerLayout->addWidget(pinButton = new QPushButton(this));
    pinButton->setIcon(QPixmap(":/icons/pin.png"));
    pinButton->setFlat(true);
    headerLayout->addWidget(dragButton = new QPushButton(this));
    dragButton->setIcon(QPixmap(":/icons/no_drag.png"));
    dragButton->setFlat(true);
    headerLayout->addWidget(importButton = new QPushButton(this));
    importButton->setIcon(QPixmap(":/icons/import.png"));
    importButton->setFlat(true);
    headerLayout->addWidget(exportButton = new QPushButton(this));
    exportButton->setIcon(QPixmap(":/icons/export.png"));
    exportButton->setFlat(true);
    header->setLayout(headerLayout);
    outerLayout->addWidget(header);

    header->setAutoFillBackground(true);
    headerColor = QColor("#66ccff");
    QPalette palette = header->palette();
    palette.setColor(QPalette::Window,headerColor);
    header->setPalette(palette);
//headerend

//calendar
    int i,j;
    calendarLayout = new QGridLayout();
    calendarLayout->setContentsMargins(0,0,0,0);
    calendarLayout->setSpacing(1);

    calendarLayout->addWidget(new QLabel(""),0,0,Qt::AlignCenter);

    //weekdayname
    for (i=1;i<=7;i++)
    {
        calendarLayout->addWidget(weekDay[i] = new QLabel(weekDayName[i]),0,i);
        weekDay[i]->setAlignment(Qt::AlignCenter);
        weekDay[i]->setFont(QFont("Times",18));
        weekDay[i]->setAutoFillBackground(true);
        weekDayColor = QColor("#00ff7f");
        QPalette palette = weekDay[i]->palette();
        palette.setColor(QPalette::Window,weekDayColor);
        weekDay[i]->setPalette(palette);
    }
    //weekdaynameend

    //days
    int firstDay = storedDate.dayOfWeek();
    for (i=1;i<=6;i++)
        for (j=1;j<=7;j++)
        {
            calendarLayout->addWidget(calendarCells[i][j] =
            new calendarCell(storedDate.addDays((i-1)*7+(j-firstDay)),storedDate,i,j,this),i,j);
        }
    //daysend

    //weeknum
    for (i=1;i<=6;i++)
    {
        calendarLayout->addWidget(weekNum[i] =
                      new QLabel(QString::number(calendarCells[i][1]->getCellDate().weekNumber())),i,0);
        weekNum[i]->setAlignment(Qt::AlignCenter);
        weekNum[i]->setFont(QFont("Times",18));

        weekNum[i]->setAutoFillBackground(true);
        weekNumColor = QColor("#ffff7f");
        QPalette palette = weekNum[i]->palette();
        palette.setColor(QPalette::Window,weekNumColor);
        weekNum[i]->setPalette(palette);
    }
    //weeknumend

//calendarend

//outer
    outerLayout->addLayout(calendarLayout,1);
    outerLayout->setSpacing(0);
    setLayout(outerLayout);
//outerend

//connections
    connect(prevButton,SIGNAL(clicked()),this,SLOT(prevMonth()));
    connect(nextButton,SIGNAL(clicked()),this,SLOT(nextMonth()));
    connect(dragButton,SIGNAL(clicked(bool)),this,SLOT(changeDragState()));
    connect(importButton,SIGNAL(clicked(bool)),this,SLOT(cwimport()));
    connect(exportButton,SIGNAL(clicked(bool)),this,SLOT(cwexport()));
    for (i=1;i<=6;i++)
        for (j=1;j<=7;j++)
        {
            connect(calendarCells[i][j],SIGNAL(callWidgetUpdate()),this,SLOT(calendarWidgetUpdate()));
            connect(calendarCells[i][j],SIGNAL(clicked(int,int)),this,SLOT(judgeMonth(int,int)));
        }
//connectionsend
}

void calendarWidget::prevMonth()
{
    storedDate = storedDate.addMonths(-1);
    calendarWidgetUpdate();
}

void calendarWidget::nextMonth()
{
    storedDate = storedDate.addMonths(1);
    calendarWidgetUpdate();
}

void calendarWidget::judgeMonth(int i, int j)
{
    QDate date = calendarCells[i][j]->getCellDate();

    if (date.addMonths(1).month()==storedDate.month())
    {
        prevMonth();
        return;
    }
    if (date.addMonths(-1).month()==storedDate.month())
    {
        nextMonth();
        return;
    }

    calendarCells[i][j]->addEvent();
}

void calendarWidget::changeDragState()
{
    int i,j;
    if (isDragAble)
    {
    dragButton->setIcon(QPixmap(":/icons/drag.png"));
    isDragAble = false;
    for (i=1;i<=6;i++)
        for (j=1;j<=7;j++)
        {
            calendarCells[i][j]->setAcceptDrops(false);
        }
    return;
    }

    if (!isDragAble)
    {
    dragButton->setIcon(QPixmap(":/icons/no_drag.png"));
    isDragAble = true;
    for (i=1;i<=6;i++)
        for (j=1;j<=7;j++)
        {
            calendarCells[i][j]->setAcceptDrops(true);
        }
    return;
    }
}

void calendarWidget::cwimport()
{
    gObject.init();
    calendarWidgetUpdate();
    QMessageBox *lan = new QMessageBox(QMessageBox::Information,tr("Import"),tr("Import Complete!"),QMessageBox::Ok);
    lan->exec();
}

void calendarWidget::cwexport()
{
    gObject.record();
    QMessageBox *lan = new QMessageBox(QMessageBox::Information,tr("Export"),tr("Export Complete!"),QMessageBox::Ok);
    lan->exec();
}

void calendarWidget::calendarWidgetUpdate()
{
    int i,j;
    int firstDay = storedDate.dayOfWeek();

    timeHeader->setText(QString::number(storedDate.year())+" - "+QString::number(storedDate.month()));

    for (i=1;i<=6;i++)
        for (j=1;j<=7;j++)
        {
           calendarCells[i][j]->cellUpdate(storedDate.addDays((i-1)*7+(j-firstDay)),storedDate);
        }

    for (i=1;i<=6;i++)
    {
       weekNum[i]->setText(QString::number(calendarCells[i][1]->getCellDate().weekNumber()));
    }
    for (i=1;i<=7;i++)
    {
        weekDay[i]->setText(weekDayName[i]);
    }
}


