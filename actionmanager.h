#pragma once
#include <QWidget>


class ActionManager : public QObject
{
    Q_OBJECT
public:
    ActionManager(QWidget* someW=nullptr);
    QWidget* targetW=nullptr;
private slots:
    void chooseColor();
};
