#pragma once

#include <QWidget>

namespace Ui {
class TitleBarTest;
}

class TitleBarTest : public QWidget
{
    Q_OBJECT

public:
    explicit TitleBarTest(QWidget *parent = 0);
    ~TitleBarTest();
protected:
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void contextMenuEvent(QContextMenuEvent *event) Q_DECL_OVERRIDE;
    bool eventFilter(QObject *watched, QEvent *event) Q_DECL_OVERRIDE;
private slots:
    void on_buttonClose_clicked();
    void on_puttonRestoreWin_clicked();
    void on_buttonMinimize_clicked();
    void chooseColor();
private:
    bool mPressed=false;
    bool mPressedMark=false;
    bool changeWidthLeft=false;
    bool changeWidthRight=false;
    bool changeHeightTop=false;
    bool changeHeightBottom=false;
    Ui::TitleBarTest *ui;
    QCursor opaCurs;
    QPoint pos;
};

