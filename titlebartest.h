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
    void switcWMode(const Qt::WindowStates& wState);

protected:
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void contextMenuEvent(QContextMenuEvent *event) Q_DECL_OVERRIDE;
private slots:
    void on_buttonClose_clicked();
    void on_puttonRestoreWin_clicked();
    void on_buttonMinimize_clicked();
private:
    bool mPressed=false;
    int deb=0;
    bool maximizeWindow=false;

    bool changeWidthLeft=false;
    bool changeWidthRight=false;
    bool changeHeightTop=false;
    bool changeHeightBottom=false;
    QByteArray geom;
    Ui::TitleBarTest *ui;
    QCursor opaCurs;
    QPoint pos;


};

