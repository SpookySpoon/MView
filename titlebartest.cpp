#include <QDebug>
#include <QDesktopWidget>
#include "titlebartest.h"
#include "ui_titlebartest.h"
#include "actionmanager.h"
#include "virtualframe.h"

TitleBarTest::TitleBarTest(QWidget *parent) :
    QWidget(parent, Qt::FramelessWindowHint | Qt::WindowSystemMenuHint),
    ui(new Ui::TitleBarTest)
{
    ui->setupUi(this);

    QPalette pal1 =palette();
    pal1.setColor(QPalette::Background,Qt::white);
    ui->widget->setPalette(pal1);

    am=new ActionManager(this);
    new VirtualFrame(this);
    ui->buttonClose->setIcon(QApplication::style()->standardIcon(QStyle::SP_TitleBarCloseButton));
    ui->buttonMinimize->setIcon(QApplication::style()->standardIcon(QStyle::SP_TitleBarMinButton));
//    ui->buttonClose->setStyle(QStyle::SP_DialogCloseButton);
}

TitleBarTest::~TitleBarTest()
{
    delete ui;
}

//Когда разворачивается окно на весь экран, белая область для начинка разворачивается по ширине, пряча границы для resize - а
void TitleBarTest::switcWMode(const Qt::WindowStates& wStat)
{
    if(wStat==Qt::WindowMaximized)
    {
        ui->buttonRestoreWin->setIcon(QApplication::style()->standardIcon(QStyle::SP_TitleBarNormalButton));
        ui->verticalLayout->setContentsMargins(0, 0, 0, 0);
        am->switchStates(true);
    }
    else
    {
        ui->buttonRestoreWin->setIcon(QApplication::style()->standardIcon(QStyle::SP_TitleBarMaxButton));
        ui->verticalLayout->setContentsMargins(5, 0, 5, 5);
        am->switchStates(false);
    }
}


void TitleBarTest::on_buttonClose_clicked()
{
    this->close();
}

void TitleBarTest::on_buttonRestoreWin_clicked()
{
    if(this->windowState()==Qt::WindowMaximized)
    {
        this->showNormal();
    }
    else
    {
        this->showMaximized();
    }
}

void TitleBarTest::on_buttonMinimize_clicked()
{
    this->setWindowState(Qt::WindowMinimized);
}
