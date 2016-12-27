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
    ~VirtualFrame();
    void setBorderWidth(const qint16 &bWidth);
    qint16 getBorderWidth() const;
private:
    void setUpFrameGeometry();
    void saveFrameSettings();
    void resizeFrameWithMouse(const QPoint& mousePosition);
    bool moveFrame(const QPoint& mousePosition);
    void mousePress(QMouseEvent *event);
    void mouseHover(QMouseEvent *event);
    void mouseMove(QMouseEvent *event);
    void mouseRelease(QMouseEvent *event);



    bool mPressed=false;
    bool maximizeWindow=false;
    bool changeWidthLeft=false;
    bool changeWidthRight=false;
    bool changeHeightTop=false;
    bool changeHeightBottom=false;

    QByteArray geom;
    qint16 borderWidth;
    TitleBarTest* someWid;
    QPoint pos;
    QCursor opaCurs;
protected:
    bool eventFilter(QObject *watched, QEvent *event) Q_DECL_OVERRIDE;
};
