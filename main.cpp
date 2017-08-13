#include "mainwindow.h"
#include <QApplication>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QMessageBox *lan = new QMessageBox(QMessageBox::Question,"Language","Use Chinese？",QMessageBox::Yes | QMessageBox::No);
    if (lan->exec() == QMessageBox::Yes)
    {
        qDebug() << "here";
        QTranslator *translator = new QTranslator;
        translator->load("tr_chinese");
        a.installTranslator(translator);
    }
    MainWindow w;
    w.show();

    return a.exec();
}
