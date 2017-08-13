#ifndef FILEVIEWER_H
#define FILEVIEWER_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QString>
#include <QList>
#include <QDate>
#include <QDrag>
#include <QTextEdit>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QFileInfo>
#include <QVBoxLayout>
#include <QDebug>

class fileData
{
public:
    fileData();
    fileData(QString fn,QDate t);
    QString fileName;
    QDate time;
};

class fileLabel : public QLabel
{
    Q_OBJECT

protected:
    void mousePressEvent(QMouseEvent *event);

public:
    fileLabel(QString name);
    QString fileName;
    void setBackgroundColor(QColor color);
};

class fileViewer : public QWidget
{
    Q_OBJECT
public:
    explicit fileViewer(QWidget *parent = 0);
    QList<fileData> fileNames;
    QString filePath;

signals:

public slots:
    void addFile(QString fileName,QDate requiredTime);
    void showFile(QDate requiredTime);
private:
    QWidget *frame;
    QVBoxLayout *fileVLayout;
};

extern fileViewer *mainFileViewer;

#endif // FILEVIEWER_H
