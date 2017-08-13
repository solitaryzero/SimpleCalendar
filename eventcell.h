#ifndef EVENTCELL_H
#define EVENTCELL_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include "eventdatabase.h"
#include "eventadder.h"

class eventCell : public QLabel
{
    Q_OBJECT
public:
    explicit eventCell();
    explicit eventCell(const eventData &ev, int num, QWidget *parent = 0);
    virtual void mousePressEvent(QMouseEvent *ev);
    QColor backgroundColor;
    eventData cellEvent;
    int number;

signals:
    void clicked(int);

public slots:
    void cellUpdate(const eventData &ev);
    void cellUpdate(const QColor &color);

private:
    void setBackgroundColor(int r, int g, int b);
    void setBackgroundColor(QColor color);
};

#endif // EVENTCELL_H
