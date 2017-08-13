#include "eventadder.h"

eventAdder::eventAdder(const QDate &eventdate, QWidget *parent):
    QDialog(parent)
{
    eventDate = eventdate;
    buildWidget();

    typeBox->setEnabled(true);
    periodBox->setEnabled(true);
    customPeriod->setEnabled(false);
    deleteButton->hide();
    isEditingPeriod = false;
}

eventAdder::eventAdder(eventData &ev, QWidget *parent):
    QDialog(parent)
{
    eventDate = ev.eventStartDate;
    buildWidget();

    typeButton[2]->setChecked(true);
    if (ev.eventType == 1) typeButton[1]->setChecked(true);
    typeBox->setEnabled(false);

    switch(ev.eventPeriod)
    {
    case 1:
        periodButton[1]->setChecked(true);
        break;
    case 7:
        periodButton[2]->setChecked(true);
        break;
    case -1:
        periodButton[3]->setChecked(true);
        break;
    default:
        periodButton[4]->setChecked(true);
        customPeriod->setText(QString::number(ev.eventPeriod));
        break;
    }
    periodBox->setEnabled(false);

    nameEdit->setText(ev.eventName);
    contentEdit->setPlainText(ev.eventContent);
    deleteButton->setEnabled(true);
}

void eventAdder::buildWidget()
{
    outerLayout = new QVBoxLayout();

    typeLayout = new QHBoxLayout();
    typeBox = new QGroupBox(tr("type"));
    typeButton[1] = new QRadioButton(tr("single"),typeBox);
    typeButton[2] = new QRadioButton(tr("period"),typeBox);
    typeLayout->addWidget(typeButton[1]);
    typeLayout->addWidget(typeButton[2]);
    typeBox->setLayout(typeLayout);
    outerLayout->addWidget(typeBox);

    periodLayout = new QGridLayout();
    periodBox = new QGroupBox(tr("period"));
    periodButton[1] = new QRadioButton(tr("every day"),periodBox);
    periodButton[2] = new QRadioButton(tr("every week"),periodBox);
    periodButton[3] = new QRadioButton(tr("every month"),periodBox);
    periodButton[4] = new QRadioButton(tr("custom"),periodBox);
    customPeriod = new QLineEdit(periodBox);
    periodLayout->addWidget(periodButton[1],0,0);
    periodLayout->addWidget(periodButton[2],0,1);
    periodLayout->addWidget(periodButton[3],1,0);
    periodLayout->addWidget(periodButton[4],1,1);
    periodLayout->addWidget(customPeriod,1,2);
    periodLayout->setColumnStretch(0,1);
    periodLayout->setColumnStretch(1,1);
    periodBox->setLayout(periodLayout);
    outerLayout->addWidget(periodBox);

    contentLayout = new QVBoxLayout();
    contentBox = new QGroupBox(tr("content"));
    name = new QLabel(tr("name"));
    nameEdit = new QLineEdit();
    content = new QLabel(tr("content"));
    contentEdit = new QPlainTextEdit();
    contentLayout->addWidget(name);
    contentLayout->addWidget(nameEdit);
    contentLayout->addWidget(content);
    contentLayout->addWidget(contentEdit);
    contentBox->setLayout(contentLayout);
    outerLayout->addWidget(contentBox);

    decisionLayout = new QHBoxLayout();
    acceptButton = new QPushButton(tr("Accept"));
    cancelButton = new QPushButton(tr("Cancel"));
    deleteButton = new QPushButton(tr("Delete"));

    decisionLayout->addWidget(acceptButton);
    decisionLayout->addWidget(deleteButton);
    decisionLayout->addWidget(cancelButton);
    outerLayout->addLayout(decisionLayout);

    this->setLayout(outerLayout);

    connect(typeButton[2],SIGNAL(toggled(bool)),periodBox,SLOT(setEnabled(bool)));
    connect(periodButton[4],SIGNAL(toggled(bool)),customPeriod,SLOT(setEnabled(bool)));
    connect(acceptButton,SIGNAL(clicked(bool)),this,SLOT(accept()));
    connect(cancelButton,SIGNAL(clicked(bool)),this,SLOT(reject()));
    connect(deleteButton,SIGNAL(clicked(bool)),this,SLOT(judgeDelete()));
    connect(this,SIGNAL(readyToDeleteSingle(eventData,QDate)),mainEventDatabase,SLOT(deleteEvent(eventData,QDate)));
    connect(this,SIGNAL(readyToDeletePeriod(eventData,QDate)),mainEventDatabase,SLOT(deletePeriodEvent(eventData,QDate)));
}

void eventAdder::resetDialog(const QDate &eventdate)
{
    typeBox->setEnabled(true);
    typeButton[1]->setChecked(true);
    periodBox->setEnabled(false);
    periodButton[1]->setChecked(true);
    customPeriod->setText("");
    nameEdit->setText("");
    contentEdit->setPlainText("");
    intendedEvent = eventData();
    eventDate = eventdate;
    isEditingPeriod = false;
    deleteButton->hide();
}

void eventAdder::resetDialog(const eventData &ev,const QDate &eventdate)
{    
    intendedEvent = ev;

    typeButton[2]->setChecked(true);
    if (ev.eventType == 1)
    {
        typeButton[1]->setChecked(true);
    }
    typeBox->setEnabled(false);

    switch(ev.eventPeriod)
    {
    case 1:
        periodButton[1]->setChecked(true);
        break;
    case 7:
        periodButton[2]->setChecked(true);
        break;
    case -1:
        periodButton[3]->setChecked(true);
        break;
    default:
        periodButton[4]->setChecked(true);
        customPeriod->setText(QString::number(ev.eventPeriod));
        break;
    }
    periodBox->setEnabled(false);

    nameEdit->setText(ev.eventName);
    contentEdit->setPlainText(ev.eventContent);
    eventDate = eventdate;
    deleteButton->show();
}

bool eventAdder::isValid()
{
    if ((!typeButton[1]->isChecked()) && (!typeButton[2]->isChecked()))
    {
        return false;
    }

    if (typeButton[2]->isChecked() &&
        (!periodButton[1]->isChecked()) &&
        (!periodButton[2]->isChecked()) &&
        (!periodButton[3]->isChecked()) &&
        (!periodButton[4]->isChecked()))
    {
        return false;
    }

    if (typeButton[2]->isChecked() &&
        periodButton[4]->isChecked() &&
        ((customPeriod->text() == "") || (customPeriod->text().toInt()<=0)) )
    {
        return false;
    }

    if (nameEdit->text() == "")
    {
        return false;
    }

    if (contentEdit->toPlainText() == "")
    {
        return false;
    }

    return true;
}

void eventAdder::accept()
{
    if (isValid())
    {
        int type = 1;
        if (typeButton[2]->isChecked())
        {
            type = 2;
        }

        int period = 1;
        if (periodButton[2]->isChecked())
        {
            period = 7;
        }
        if (periodButton[3]->isChecked())
        {
            period = -1;
        }
        if (periodButton[4]->isChecked())
        {
            period = customPeriod->text().toInt();
        }

        intendedEvent = eventData(nameEdit->text(),contentEdit->toPlainText(),type,eventDate,0,period);
        QDialog::accept();
    }
    else
    {
        QMessageBox *errorBox =
        new QMessageBox(QMessageBox::Warning,tr("Error!"),tr("Some infomation is missing!"),QMessageBox::Ok,this);
        errorBox->show();
    }
}

void eventAdder::reject()
{
     QDialog::reject();
}

void eventAdder::judgeDelete()
{
    if (isEditingPeriod == true)
    {
        emit readyToDeletePeriod(intendedEvent,eventDate);
    }
    else
    {
        emit readyToDeleteSingle(intendedEvent,eventDate);
    }
    QDialog::reject();
}

eventAdder *eventAddDialog;
