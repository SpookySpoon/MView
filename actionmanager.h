#pragma once
#include <QWidget>
#include <QMenu>
class TitleBarTest;
class ActionManager : public QObject
{
    Q_OBJECT
public:
    ActionManager(TitleBarTest* someW=nullptr);
    TitleBarTest* targetW=nullptr;
    void switchStates(bool state);
private:
    QMenu menu;
private slots:
    void callMenu(QPoint point);
    void chooseColor();
};
