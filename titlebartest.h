#ifndef TITLEBARTEST_H
#define TITLEBARTEST_H

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
    void mouseHoverEvent(QHoverEvent *evetn) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    bool mPressed=false;
    bool changeWidthLeft=false;
    bool changeWidthRight=false;
    bool changeHeightTop=false;
    bool changeHeightBottom=false;
    Ui::TitleBarTest *ui;
    QPoint pos;
};

#endif // TITLEBARTEST_H
