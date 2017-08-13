#include "calendarcell.h"

calendarCell::calendarCell(const QDate &requiredDate, const QDate &firstDate, int row, int column, QWidget *parent) :
    QWidget(parent)
{
    setAcceptDrops(true);
    setMouseTracking(true);
    this->setContentsMargins(0,0,0,0);

    eventList = QList<eventCell*>();
    outerLayout = new QVBoxLayout(this);
    outerLayout->setContentsMargins(0,0,0,0);
    cellCore = new QWidget();
    atRow = row;
    atColumn = column;

    cellUpdate(requiredDate,firstDate);
}

void calendarCell::generateEventCells(QList<eventData> evs)
{
    QList<eventCell*> ecs;
    int i;
    for (i=0;i<evs.size();i++)
    {
        ecs.append(new eventCell(evs[i],i));
    }
    eventList = ecs;
}

void calendarCell::getBackgroundColor()
{
    int i;
    for (i=0;i<mainColorManager->colors.size();i++)
    {
        if (cellDate == mainColorManager->colors[i].colorDate) break;
    }
    if (i >= mainColorManager->colors.size()) return;
    setBackgroundColor(mainColorManager->colors[i].color);
}

void calendarCell::setBackgroundColor(int r, int g, int b)
{
    setAutoFillBackground(true);
    backgroundColor = QColor(r,g,b);
    QPalette palette = this->palette();
    palette.setColor(QPalette::Window,backgroundColor);
    setPalette(palette);
}

void calendarCell::setBackgroundColor(QColor color)
{
    setAutoFillBackground(true);
    backgroundColor = QColor(color);
    QPalette palette = this->palette();
    palette.setColor(QPalette::Window,backgroundColor);
    setPalette(palette);
}

void calendarCell::cellUpdate(const QDate &newDate, const QDate &newFirstDate)
{
    QString dstText;

    isIncurrentMonth = true;
    if (newDate.month()!=newFirstDate.month()) isIncurrentMonth = false;

    if (cellCore != nullptr)
    {
        outerLayout->removeWidget(cellCore);
        delete(cellCore);
    }
    cellCore = new QWidget();
    cellCore->setContentsMargins(2,2,2,2);
    header = new QWidget();
    cellLayout = new QVBoxLayout();
    cellLayout->setContentsMargins(0,0,0,0);
    cellLayout->setSpacing(1);
    iconLayout = new QHBoxLayout();
    iconLayout->setContentsMargins(0,0,0,0);
    iconLayout->setSpacing(5);

    dayz = new QLabel("");
    dayz->setFont(QFont("Times",10));
    showFileButton = new QPushButton(cellCore);
    showFileButton->setIcon(QPixmap(":/icons/see_file.png"));
    showFileButton->setFlat(true);
    addEventButton = new QPushButton(cellCore);
    addEventButton->setIcon(QPixmap(":/icons/add_event.png"));
    addEventButton->setFlat(true);
    editColorButton = new QPushButton(cellCore);
    editColorButton->setIcon(QPixmap(":/icons/edit_color.png"));
    editColorButton->setFlat(true);
    iconLayout->addWidget(dayz);
    iconLayout->addSpacerItem(new QSpacerItem(0,0,QSizePolicy::Expanding,QSizePolicy::Minimum));
    iconLayout->addWidget(showFileButton);
    iconLayout->addWidget(addEventButton);
    iconLayout->addWidget(editColorButton);
    header->setLayout(iconLayout);

    showFileButton->hide();
    addEventButton->hide();
    editColorButton->hide();

    header->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    cellLayout->addWidget(header);

    connect(showFileButton,SIGNAL(clicked(bool)),this,SLOT(showFile()));
    connect(addEventButton,SIGNAL(clicked(bool)),this,SLOT(addEvent()));
    connect(editColorButton,SIGNAL(clicked(bool)),this,SLOT(editColor()));
    this->setCellDate(newDate);
    int i;
    generateEventCells(mainEventDatabase->getEvents(cellDate));

    setAutoFillBackground(true);
    backgroundColor = Qt::white;
    QPalette palette = this->palette();
    palette.setColor(QPalette::Window,backgroundColor);
    setPalette(palette);

    if (isIncurrentMonth) getBackgroundColor();

    if (!isIncurrentMonth)
    {
        dstText = "<h4> <font color=grey>"
                +QString::number(this->getCellDate().day())
                +"</font></h4>";
        dayz->setText(dstText);
        for (i=0;i<=2;i++)
        {
            QSpacerItem *tempSpacer = new QSpacerItem(0,0,QSizePolicy::Expanding,QSizePolicy::Expanding);
            cellLayout->addSpacerItem(tempSpacer);
        }
        cellCore->setLayout(cellLayout);
        outerLayout->addWidget(cellCore);
        return;
    }

    if (newDate.dayOfWeek()>=6)
    {
        dstText = "<h4> <font color=red>"
                +QString::number(this->getCellDate().day())
                +"</font></h4>";
    }
    else
    {
       dstText = QString::number(this->getCellDate().day());
    }

    dayz->setText(dstText);
    for (i=0;i<eventList.size();i++)
    {
        eventList[i]->setFixedHeight(this->height()/4-3);
        eventList[i]->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
        cellLayout->addWidget(eventList[i]);
        eventList[i]->setAlignment(Qt::AlignCenter);
        connect(eventList[i],SIGNAL(clicked(int)),this,SLOT(editEvent(int)));
    }
    for (i=eventList.size();i<=2;i++)
    {
        QSpacerItem *tempSpacer = new QSpacerItem(0,0,QSizePolicy::Expanding,QSizePolicy::Expanding);
        cellLayout->addSpacerItem(tempSpacer);
    }
    cellCore->setLayout(cellLayout);
    outerLayout->addWidget(cellCore);

    return;
}

void calendarCell::addEvent()
{
    eventAddDialog->resetDialog(cellDate);
    if (eventAddDialog->exec() == QDialog::Accepted)
    {
        eventData ev = eventAddDialog->intendedEvent;
        mainEventDatabase->addEvent(ev);
        callWidgetUpdate();
    }
}

void calendarCell::editEvent(int id)
{
    eventData ev = eventList[id]->cellEvent;
    if (ev.eventType == 2)
    {
        QMessageBox::StandardButton rb =
        QMessageBox::question(this, tr("Choice"), tr("This is a serie event. Do you want to edit the whole serie?"),
                              QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        if(rb == QMessageBox::Yes)
        {
            eventAddDialog->isEditingPeriod = true;
            eventAddDialog->resetDialog(eventList[id]->cellEvent,ev.eventStartDate);
            if (eventAddDialog->exec() == QDialog::Accepted)
            {
                eventData tempev = eventAddDialog->intendedEvent;
                tempev.id = eventList[id]->cellEvent.id;
                mainEventDatabase->editPeriodEvent(tempev);
            }
            emit callWidgetUpdate();
            return;
        }
    }

    eventAddDialog->isEditingPeriod = false;
    eventAddDialog->resetDialog(eventList[id]->cellEvent,cellDate);

    if (eventAddDialog->exec() == QDialog::Accepted)
    {
        eventData tempev = eventAddDialog->intendedEvent;
        tempev.id = eventList[id]->cellEvent.id;
        mainEventDatabase->editSingleEvent(tempev,cellDate);
    }
    cellUpdate(cellDate,QDate(cellDate.year(),cellDate.month(),1));
}

void calendarCell::editColor()
{
    QColor newColor = QColorDialog::getColor(Qt::white,this,tr("Color Set"));
    if (!newColor.isValid()) return;
    if (backgroundColor == Qt::white)
    {
        if (newColor == Qt::white) return;
        mainColorManager->addColor(newColor,cellDate);
        setBackgroundColor(newColor);
        return;
    }
    if (backgroundColor != Qt::white)
    {
        if (newColor == Qt::white)
        {
            mainColorManager->deleteColor(cellDate);
        }
        else
        {
            mainColorManager->editColor(newColor,cellDate);
        }
        setBackgroundColor(newColor);
        return;
    }
}

void calendarCell::showFile()
{
    mainFileViewer->showFile(cellDate);
}

void calendarCell::mousePressEvent(QMouseEvent *ev)
{
    emit clicked(atRow,atColumn);
}

void calendarCell::resizeEvent(QResizeEvent *event)
{
    header->setFixedHeight(this->height()/4);
    for (int i=0;i<eventList.size();i++)
        eventList[i]->setFixedHeight(this->height()/4-3);
}

void calendarCell::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat("text/uri-list"))
        event->acceptProposedAction();
}

void calendarCell::dropEvent(QDropEvent *event)
{
    QList<QUrl> urls = event->mimeData()->urls();
    if(urls.isEmpty())
        return;

    foreach(QUrl url, urls)
    {
        fileLink = url.toLocalFile();
        fileName = url.fileName();
        QFile::copy(fileLink,"D:/Qt projects/copiedfiles/"+fileName);
    }

    mainFileViewer->addFile(fileName,cellDate);
}

void calendarCell::enterEvent(QEvent *ev)
{
    if (!isIncurrentMonth) return;
    showFileButton->show();
    addEventButton->show();
    editColorButton->show();
}

void calendarCell::leaveEvent(QEvent *ev)
{
    showFileButton->hide();
    addEventButton->hide();
    editColorButton->hide();
}


