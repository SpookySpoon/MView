#pragma once

#include <QWidget>
class QMouseEvent;
class QContextMenuEvent;
class QWindowStateChangeEvent;
class TitleBarTest;
class VirtualFrame: public QObject
{
    Q_OBJECT
public:
    explicit VirtualFrame(TitleBarTest* someWid);
    void setBorderWidth(const qint16 &bWidth);
    qint16 getBorderWidth() const;
private:
    void mouseHover(QMouseEvent *event);
    void mouseMove(QMouseEvent *event);
    void mousePress(QMouseEvent *event);
    void mouseRelease(QMouseEvent *event);
    void contextMenu(QContextMenuEvent *event);
    qint16 borderWidth;
    TitleBarTest* someWid;
protected:
    bool eventFilter(QObject *watched, QEvent *event) Q_DECL_OVERRIDE;


};
