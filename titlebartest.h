#pragma once

#include <QWidget>

class ActionManager;
namespace Ui {
class TitleBarTest;
}

class TitleBarTest : public QWidget
{
    Q_OBJECT

public:
    explicit TitleBarTest(QWidget *parent = 0);
    ~TitleBarTest();
    void switcWMode(const Qt::WindowStates& wState);
private slots:
    void on_buttonClose_clicked();
    void on_puttonRestoreWin_clicked();
    void on_buttonMinimize_clicked();
private:
    Ui::TitleBarTest *ui;
    ActionManager* am;
};

