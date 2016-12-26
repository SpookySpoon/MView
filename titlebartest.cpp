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
    QByteArray geom = settings.value("Geometry").toByteArray();
    QPalette pal =palette();
    restoreGeometry(geom);
    pal.setColor(QPalette::Background,settings.value("color").value<QColor>());


    QPalette pal1 =palette();
    pal1.setColor(QPalette::Background,Qt::white);
    ui->widget->setPalette(pal1);
    this->setPalette(pal);

//    this->installEventFilter(this);


    new VirtualFrame(this);
    new ActionManager(this);
}

TitleBarTest::~TitleBarTest()
{
    QSettings settings(QString("%1\\%2").arg(QApplication::applicationDirPath()).arg("MViewsSettings.ini"),QSettings::IniFormat);
    settings.setValue("geometry", saveGeometry());
    settings.setValue("color", this->palette().color(QPalette::Background));
    delete ui;
}

void TitleBarTest::mousePressEvent(QMouseEvent *event)
{
    mPressedMark=true;
    QPoint cPos=event->pos();
    QPoint wPos=frameGeometry().topLeft();
    QWidget* chAt=this->childAt(cPos);
    if(!chAt)
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
        if(changeWidthLeft||changeWidthRight||changeHeightTop||changeHeightBottom)
        {
            return;
        }
        if (event->button() == Qt::LeftButton) {
            mPressed=true;
            pos = event->globalPos() - wPos;
            event->accept();
        }
    }
}
void TitleBarTest::mouseReleaseEvent(QMouseEvent *event)
{
    mPressedMark=false;
    if (event->button() == Qt::LeftButton) {
        mPressed=false;
    }
    changeWidthLeft=false;
    changeWidthRight=false;
    changeHeightTop=false;
    changeHeightBottom=false;
}

void TitleBarTest::paintEvent(QPaintEvent *event)
{
    event->region();
//    qDebug()<<event->type();
}




void TitleBarTest::mouseMoveEvent(QMouseEvent *event)
{
    QPoint cPos=event->globalPos();
    QPoint wPos=frameGeometry().topLeft();

    if (mPressed) {
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
    this->setWindowState(Qt::WindowMinimized);
}
