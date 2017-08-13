#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>
#include "global.h"
#include <windows.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void fixMainWindow();
    void unFixMainWindow();

private:
    Ui::MainWindow *ui;
    calendarWidget *cw;
    QAction *newAct;
    QSystemTrayIcon *trayIcon;
    global gObject;
    int x;
};

#endif // MAINWINDOW_H
