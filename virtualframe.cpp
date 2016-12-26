#include <QDebug>
#include <QMessageBox>
#include <QMouseEvent>
#include <QContextMenuEvent>
#include <QWindowStateChangeEvent>
#include "virtualframe.h"
#include "titlebartest.h"

VirtualFrame::VirtualFrame(TitleBarTest* parent)
    :QObject(parent), someWid(parent)
{
    someWid->installEventFilter(this);
    someWid->setAttribute(Qt::WA_Hover);

}

bool VirtualFrame::eventFilter(QObject *, QEvent *event)
{

    switch (event->type())
    {
//        case QEvent::Paint:
//            return true;
        case QEvent::HoverMove:
            if(someWid->windowState()!=Qt::WindowMaximized)
            {
                mouseHover(static_cast<QMouseEvent*>(event));
            }
            return true;
        case QEvent::WindowStateChange:
            qDebug()<<"change";
            someWid->switcWMode(someWid->windowState());
            return true;
        default:
            return false;
    }
}

void VirtualFrame::mouseHover(QMouseEvent *event)
{
    QCursor opaCurs;
    QPoint cPos=event->globalPos();
    QPoint wPos=someWid->frameGeometry().topLeft();

    bool right = wPos.x()+someWid->width()-cPos.x()<5;
    bool left = cPos.x()-wPos.x()<5;
    bool top = cPos.y()-wPos.y()<5;
    bool bottom = wPos.y()+someWid->height()-cPos.y()<5;
    if((left&&top)||(right&&bottom))
    {
        opaCurs.setShape(Qt::SizeFDiagCursor);
    }
    else if((right&&top)||(left&&bottom))
    {
        opaCurs.setShape(Qt::SizeBDiagCursor);
    }
    else if(right||left)
    {
        opaCurs.setShape(Qt::SizeHorCursor);
    }
    else if(top||bottom)
    {
        opaCurs.setShape(Qt::SizeVerCursor);
    }
    else
    {
        opaCurs.setShape(Qt::ArrowCursor);
    }
    someWid->setCursor(opaCurs);
}











void VirtualFrame::setBorderWidth(const qint16 &bWidth)
{
    borderWidth=bWidth;
}



qint16 VirtualFrame::getBorderWidth() const
{
    return borderWidth;
}
