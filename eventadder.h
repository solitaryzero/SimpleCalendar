#ifndef EVENTADDER_H
#define EVENTADDER_H

#include <QObject>
#include <QWidget>
#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QRadioButton>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QSpacerItem>
#include <QMessageBox>
#include "eventdatabase.h"

class eventAdder : public QDialog
{
    Q_OBJECT

public:
    explicit eventAdder(const QDate &eventdate, QWidget *parent = 0);
    explicit eventAdder(eventData &ev, QWidget *parent = 0);
    eventData intendedEvent;
    bool isEditingPeriod;

signals:
    void readyToDeleteSingle(eventData ev,QDate date);
    void readyToDeletePeriod(eventData ev,QDate date);

public slots:
    void resetDialog(const QDate &eventdate);
    void resetDialog(const eventData &ev,const QDate &eventdate);
    virtual void accept();
    virtual void reject();
    void judgeDelete();
private:
    QVBoxLayout *outerLayout,*contentLayout;
    QHBoxLayout *typeLayout,*decisionLayout;
    QGridLayout *periodLayout;
    QGroupBox *typeBox,*periodBox,*contentBox;
    QRadioButton *typeButton[3],*periodButton[5];
    QLineEdit *customPeriod,*nameEdit;
    QPlainTextEdit *contentEdit;
    QLabel *name,*content;
    QPushButton *acceptButton,*cancelButton,*deleteButton;
    bool isValid();
    void buildWidget();
    QDate eventDate;

};

extern eventAdder *eventAddDialog;

#endif // EVENTADDER_H
