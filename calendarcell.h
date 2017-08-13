#ifndef CALENDARCELL_H
#define CALENDARCELL_H

#include "eventdatabase.h"
#include "eventcell.h"
#include "fileviewer.h"
#include "eventadder.h"
#include "colormanager.h"
#include <QGridLayout>
#include <QColorDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QDrag>
#include <QMouseEvent>
#include <QTextEdit>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QFileInfo>

class calendarCell : public QWidget
{
    Q_OBJECT

public:
    explicit calendarCell(const QDate &requiredDate, const QDate &firstDate, int row, int column, QWidget *parent = 0);

    void mousePressEvent(QMouseEvent *ev);
    void setCellDate(const QDate &dst) { cellDate = dst; }
    const QDate getCellDate() { return cellDate; }

    int atRow, atColumn;
    QColor backgroundColor;

protected:
    void resizeEvent(QResizeEvent *ev);
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
    void enterEvent(QEvent *ev);
    void leaveEvent(QEvent *ev);

signals:
    void clicked(int i,int j);
    void callWidgetUpdate();

public slots:
    void cellUpdate(const QDate &newDate, const QDate &newFirstDate);
    void editEvent(int id);
    void addEvent();
    void showFile();
    void editColor();

private:
    QDate cellDate;
    QWidget *cellCore,*header;
    QLabel *dayz;
    QList<eventCell*> eventList;
    QVBoxLayout *cellLayout,*outerLayout;
    QHBoxLayout *iconLayout;
    QPushButton *showFileButton,*addEventButton,*editColorButton;
    void generateEventCells(QList<eventData> evs);
    void getBackgroundColor();
    void setBackgroundColor(int r, int g, int b);
    void setBackgroundColor(QColor color);
    bool readFile(const QString &fileName);
    bool isIncurrentMonth;
    QTextEdit *textEdit;
    QString fileName,fileLink;
};

#endif // CALENDARCELL_H
