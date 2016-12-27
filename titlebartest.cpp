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
        ui->verticalLayout->setContentsMargins(0, 0, 0, 0);
        am->switchStates(true);
    }
    else
    {
        ui->verticalLayout->setContentsMargins(5, 0, 5, 5);
        am->switchStates(false);
    }
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
