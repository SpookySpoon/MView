#include <QMouseEvent>
#include <QColorDialog>
#include <QAction>
#include <QEvent>
#include <QDebug>
#include "actionmanager.h"
#include "titlebartest.h"


ActionManager::ActionManager(TitleBarTest* someWidget)
    :QObject(someWidget), targetW(someWidget)
{
    QAction *colorChange = new QAction(tr("Color"), targetW);
    QAction *close = new QAction(tr("Close"), targetW);
    QAction *restore = new QAction(tr("Restore"), targetW);
    QAction *minimize = new QAction(tr("Minimize"), targetW);
    QAction *maximize = new QAction(tr("Maximize"), targetW);

    connect(colorChange, SIGNAL(triggered()), this, SLOT(chooseColor()));
    connect(close, SIGNAL(triggered()), targetW, SLOT(close()));
    connect(restore, SIGNAL(triggered()), targetW, SLOT(on_puttonRestoreWin_clicked()));
    connect(minimize, SIGNAL(triggered()), targetW, SLOT(on_buttonMinimize_clicked()));
    connect(maximize, SIGNAL(triggered()), targetW, SLOT(on_puttonRestoreWin_clicked()));

    menu.addAction(colorChange);
    menu.addAction(restore);
    menu.addAction(minimize);
    menu.addAction(maximize);
    menu.addSeparator();
    menu.addAction(close);

    targetW->installEventFilter(this);
    targetW->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(targetW,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(callMenu(QPoint)));
}

void ActionManager::callMenu(QPoint e)
{
    int border =5;
    int right=targetW->geometry().right()-border;
    int left=targetW->geometry().left()+border;
    int top=targetW->geometry().top()+border;
    int bottom=targetW->geometry().bottom()-border;
    QPoint point=targetW->frameGeometry().topLeft()+e;
    if(!(point.x()>left&&point.x()<right))
    {return;}
    if(!(point.y()<bottom&&point.y()>top))
    {return;}
    QWidget* child=targetW->childAt(e);
    if(!child)
    {
        menu.popup(point);
    }
}


void ActionManager::chooseColor()
{
    QColor color= QColorDialog::getColor(Qt::green, targetW);
    QPalette pal =targetW->palette();
    if(color.isValid())
    {
        pal.setColor(QPalette::Background,color);
        targetW->setPalette(pal);
    }
}

void ActionManager::switchStates(bool state)
{
    if(state)
    {
        menu.actions().at(1)->setEnabled(true);
        menu.actions().at(3)->setEnabled(false);
    }
    else
    {
        menu.actions().at(1)->setEnabled(false);
        menu.actions().at(3)->setEnabled(true);
    }
}
