#include <QColorDialog>
#include <QAction>
#include "actionmanager.h"


ActionManager::ActionManager(QWidget* someWidget)
    :QObject(someWidget), targetW(someWidget)
{
    QAction *colorChange = new QAction(tr("Color"), targetW);
    connect(colorChange, SIGNAL(triggered()), this, SLOT(chooseColor()));
    targetW->addAction(colorChange);
    targetW->setContextMenuPolicy(Qt::ActionsContextMenu);
}

void ActionManager::chooseColor()
{
    QColor color= QColorDialog::getColor(Qt::green, targetW);
    QPalette pal =targetW->palette();
    pal.setColor(QPalette::Background,color);
    targetW->setPalette(pal);
}
