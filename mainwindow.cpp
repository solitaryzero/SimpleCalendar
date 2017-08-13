#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    this->resize(1000,600);
    gObject = global();
    gObject.init();
    cw = new calendarWidget(QDate::currentDate(),this);
    setCentralWidget(cw);

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(QPixmap(":/icons/calendar.png"));
    trayIcon->setToolTip("Calendar!Yay!");
    trayIcon->show();

    newAct = new QAction(tr("Unpin"),this);
    newAct->setShortcut(QKeySequence(tr("Ctrl+P")));

    connect(cw->pinButton,SIGNAL(clicked(bool)),this,SLOT(fixMainWindow()));
    connect(this->newAct,SIGNAL(triggered(bool)),this,SLOT(unFixMainWindow()));
    connect(trayIcon,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this,SLOT(unFixMainWindow()));
    x = GetWindowLong((HWND)this->winId(), GWL_EXSTYLE);


}

MainWindow::~MainWindow()
{
    gObject.record();
}

void MainWindow::fixMainWindow()
{
    this->setWindowOpacity(0.5);
    SetWindowLong((HWND)this->winId(), GWL_EXSTYLE, x | WS_EX_TRANSPARENT | WS_EX_LAYERED);
}

void MainWindow::unFixMainWindow()
{
    this->setWindowOpacity(1);
    SetWindowLong((HWND)this->winId(), GWL_EXSTYLE, x);
}
