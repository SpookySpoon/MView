#pragma once
#include <QWidget>
#include <QTimer>
#include <QApplication>
#include <QDesktopWidget>

class resW: public QObject
{
    Q_OBJECT
public:
    resW(QWidget* someW, int coordX, int coordY, QObject* parent=nullptr)
        :QObject(parent), opaNahW(someW){
        connect(&timmy,SIGNAL(timeout()),this,SLOT(resizeSomeW()));
        connect(&timmy,SIGNAL(timeout()),this,SLOT(restartTimmy()));
        opaNahW->move(rec.left()+10,rec.top()+10);
        opaNahW->resize(1,1);
        opaNahW->show();
        restartTimmy();
    }
    ~resW(){}
public slots:
    void resizeSomeW()
    {
        QSize opaSize=opaNahW->size();
        opaSize.setHeight(std::min(opaSize.height()+20,rec.height()-20));
        opaSize.setWidth(std::min(opaSize.width()+20,rec.width()-20));
        opaNahW->resize(opaSize);
    }
    void restartTimmy()
    {
        timmy.start(5);
    }
private:
    QTimer timmy;
    QWidget* opaNahW;
    QRect rec = QApplication::desktop()->availableGeometry();
};


