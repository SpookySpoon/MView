#include <QDesktopWidget>
#include <QPaintEngine>
#include <QContextMenuEvent>
#include <QSettings>
#include <QColorDialog>
#include <QHoverEvent>
#include <QDebug>
#include <QMouseEvent>
#include <QMessageBox>
#include <QApplication>
#include "titlebartest.h"
#include "ui_titlebartest.h"
#include "actionmanager.h"
#include "virtualframe.h"

TitleBarTest::TitleBarTest(QWidget *parent) :
    QWidget(parent, Qt::FramelessWindowHint | Qt::WindowSystemMenuHint),
    ui(new Ui::TitleBarTest)
{
    ui->setupUi(this);

    QSettings settings(QString("%1\\%2").arg(QApplication::applicationDirPath()).arg("MViewsSettings.ini"),QSettings::IniFormat);
    geom = settings.value("Geometry").toByteArray();
    QPalette pal =palette();
    restoreGeometry(geom);
    setWindowState(Qt::WindowStates(settings.value("winState").toInt()));
    switcWMode(windowState());
    pal.setColor(QPalette::Background,settings.value("color").value<QColor>());


    QPalette pal1 =palette();
    pal1.setColor(QPalette::Background,Qt::white);
    ui->widget->setPalette(pal1);
    this->setPalette(pal);


    new VirtualFrame(this);
    new ActionManager(this);
    qDebug()<<this->windowState();
}

TitleBarTest::~TitleBarTest()
{
    QSettings settings(QString("%1\\%2").arg(QApplication::applicationDirPath()).arg("MViewsSettings.ini"),QSettings::IniFormat);
    settings.setValue("geometry", geom);
    settings.setValue("color", this->palette().color(QPalette::Background));
    settings.setValue("winState", QString("%1").arg(windowState()));
    delete ui;
}

void TitleBarTest::mousePressEvent(QMouseEvent *event)
{
    QPoint cPos=event->pos();
    QPoint wPos=frameGeometry().topLeft();
    QWidget* chAt=this->childAt(cPos);
    if(!chAt)
    {
        if(this->windowState()!=Qt::WindowMaximized)
        {
            if(cPos.x()<5)
            {
                changeWidthLeft=true;
            }
            else if(width()-cPos.x()<5)
            {
                changeWidthRight=true;
            }
            if(cPos.y()<5)
            {
                changeHeightTop=true;
            }
            else if(height()-cPos.y()<5)
            {
                changeHeightBottom=true;
            }
            if((changeWidthLeft||changeWidthRight||changeHeightTop||changeHeightBottom))
            {
                return;
            }
        }
        if (event->button() == Qt::LeftButton) {
            mPressed=true;
            pos = event->globalPos() - wPos;
            event->accept();
            qDebug()<<pos;
        }
    }
}
void TitleBarTest::mouseReleaseEvent(QMouseEvent *event)
{
    changeWidthLeft=false;
    changeWidthRight=false;
    changeHeightTop=false;
    changeHeightBottom=false;
    QRect rec = QApplication::desktop()->availableGeometry();
    if(event->globalPos().x()==0)
    {
        if(mPressed)
        {
            mPressed=false;
            resize(rec.width()/2,rec.height());
            move(0,0);
            maximizeWindow=true;
            return;
        }
    }
    if(event->globalPos().x()==rec.width()-1)
    {
        if(mPressed)
        {
            mPressed=false;
            resize(rec.width()/2,rec.height());
            move(rec.width()/2,0);
            maximizeWindow=true;
            return;
        }
    }
    if(event->globalPos().y()==0)
    {
        if(mPressed)
        {
            mPressed=false;
            this->setWindowState(Qt::WindowMaximized);
            return;
        }
        else
        {
            move(this->frameGeometry().topLeft().x(),0);
            resize(width(),rec.height());
            maximizeWindow=true;
            return;
        }
    }
    mPressed=false;
    if(this->windowState()!=Qt::WindowMaximized)
    {
        geom = saveGeometry();
    }

}



void TitleBarTest::mouseMoveEvent(QMouseEvent *event)
{
    QPoint cPos=event->globalPos();
    QPoint wPos=frameGeometry().topLeft();
    if (mPressed) {
        if(this->windowState()==Qt::WindowMaximized||maximizeWindow)
        {
            double proportion = pos.x()/(double)width();
            int rightPos= width()-pos.x();
            this->setWindowState(Qt::WindowNoState);
            maximizeWindow=false;
            restoreGeometry(geom);
            if(proportion>0.5)
            {
                pos.setX(std::max(width()-rightPos,width()/2));
            }
            else
            {
                pos.setX(std::min(pos.x()+6,width()/2));
            }
            if(pos.y()<6)
            {
                pos.setY(6);
            }
        }
        move(event->globalPos() - pos);
        event->accept();
        return;
    }
    if(changeWidthLeft)
    {
        QRect opa=geometry();
        opa.setLeft(std::min(cPos.x(),wPos.x()+width()-minimumWidth()));
        this->setGeometry(opa);
    }
    if(changeWidthRight)
    {
        int dx=cPos.x()-wPos.x()-width();
        this->resize(width()+dx,height());
    }
    if(changeHeightTop)
    {
        QRect opa=geometry();
        opa.setTop(std::min(cPos.y(),wPos.y()+height()-minimumHeight()));
        this->setGeometry((opa));
    }
    if(changeHeightBottom)
    {
        int dy=cPos.y()-wPos.y()-height();
        this->resize(width(),height()+dy);
    }
    this->setCursor(opaCurs);
}

void TitleBarTest::switcWMode(const Qt::WindowStates& wStat)
{
    if(wStat==Qt::WindowMaximized)
    {
        ui->verticalLayout->setContentsMargins(0, 0, 0, 0);
    }
    else
    {
        ui->verticalLayout->setContentsMargins(5, 0, 5, 5);
    }
}


void TitleBarTest::contextMenuEvent(QContextMenuEvent *event)
{

}

void TitleBarTest::on_buttonClose_clicked()
{
    this->close();
}

void TitleBarTest::on_puttonRestoreWin_clicked()
{
    if(this->windowState()==Qt::WindowMaximized)
    {
        this->setWindowState(Qt::WindowNoState);
    }
    else
    {
        this->setWindowState(Qt::WindowMaximized);
    }
}

void TitleBarTest::on_buttonMinimize_clicked()
{
    QMessageBox opa;
    opa.setText(QString("%1").arg(this->windowState()));
    opa.exec();
//    this->setWindowState(Qt::WindowMinimized);
}
