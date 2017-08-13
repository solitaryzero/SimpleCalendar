#include "eventcell.h"

eventCell::eventCell()
{
    this->setText("");
    setBackgroundColor(0,0,255);
    number = 0;
}


eventCell::eventCell(const eventData &ev, int num, QWidget *parent):
    QLabel(parent)
{
    this->setText(ev.eventName);
    cellEvent = ev;
    setBackgroundColor("#55aaff");
    number = num;
}

void eventCell::cellUpdate(const eventData &ev)
{
    this->setText(ev.eventName);
    cellEvent = ev;
}

void eventCell::cellUpdate(const QColor &color)
{
    setBackgroundColor(color);
}

void eventCell::setBackgroundColor(int r, int g, int b)
{
    setAutoFillBackground(true);
    backgroundColor = QColor(r,g,b);
    QPalette palette = this->palette();
    palette.setColor(QPalette::Window,backgroundColor);
    setPalette(palette);
}

void eventCell::setBackgroundColor(QColor color)
{
    setAutoFillBackground(true);
    backgroundColor = QColor(color);
    QPalette palette = this->palette();
    palette.setColor(QPalette::Window,backgroundColor);
    setPalette(palette);
}

void eventCell::mousePressEvent(QMouseEvent *ev)
{
    emit clicked(number);
}
