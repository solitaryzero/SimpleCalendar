#include "fileviewer.h"

fileData::fileData()
{
    fileName = "";
    time = QDate::currentDate();
}

fileData::fileData(QString fn,QDate t)
{
    fileName = fn;
    time = t;
}

fileLabel::fileLabel(QString name)
{
    this->setText(name);
    fileName =  "D:/Qt projects/copiedfiles/"+ name;
}

void fileLabel::setBackgroundColor(QColor color)
{
    setAutoFillBackground(true);
    QColor backgroundColor = QColor(color);
    QPalette palette = this->palette();
    palette.setColor(QPalette::Window,backgroundColor);
    setPalette(palette);
}

void fileLabel::mousePressEvent(QMouseEvent *event)
{
    QMimeData *QD = new QMimeData;
    QList<QUrl> urls;
    urls.append(QUrl::fromLocalFile(fileName));
    QD->setUrls(urls);
    QDrag *drag = new QDrag(this);
    drag->setMimeData(QD);
    drag->start();
}

fileViewer::fileViewer(QWidget *parent) : QWidget(parent)
{
    filePath = "D:/Qt projects/copiedfiles/";
    frame = new QLabel(this);
    fileVLayout = new QVBoxLayout();
}

void fileViewer::addFile(QString fileName,QDate requiredTime)
{
    fileData fd(fileName,requiredTime);
    fileNames.append(fd);
}

void fileViewer::showFile(QDate requiredTime)
{
    int i;
    delete(fileVLayout);
    delete(frame);
    frame = new QLabel();
    fileLabel *temp = new fileLabel("");
    frame->setWindowTitle(tr("View Files"));
    fileVLayout = new QVBoxLayout();
    for (i=0;i<fileNames.size();i++)
    {
        if (fileNames[i].time == requiredTime)
        {            
            fileVLayout->addWidget(temp = new fileLabel(fileNames[i].fileName));
            temp->setBackgroundColor("#66ccff");
            temp->setAlignment(Qt::AlignCenter);
            temp->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
        }
    }
    if (fileNames.size()==0)
    {
        fileVLayout->addWidget(new QLabel(tr("Sorry, but no file is in this day!")));
    }
    frame->setLayout(fileVLayout);
    frame->show();
}

fileViewer *mainFileViewer;
