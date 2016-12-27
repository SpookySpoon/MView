#include <QApplication>
#include <QDesktopWidget>
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
        case QEvent::MouseButtonPress:
            mousePress(static_cast<QMouseEvent*>(event));
            return true;
        case QEvent::MouseButtonRelease:
            mouseRelease(static_cast<QMouseEvent*>(event));
            return true;
        case QEvent::MouseMove:
            mouseMove(static_cast<QMouseEvent*>(event));
            return true;
        case QEvent::HoverMove:
            if(someWid->windowState()!=Qt::WindowMaximized)
            {
                mouseHover(static_cast<QMouseEvent*>(event));
            }
            return true;
        case QEvent::WindowStateChange:
            someWid->switcWMode(someWid->windowState());
            return true;
        default:
            return false;
    }
}


void VirtualFrame::mousePress(QMouseEvent *event)
{
    QPoint cPos=event->pos();
    QPoint wPos=someWid->frameGeometry().topLeft();
    QWidget* chAt=someWid->childAt(cPos);
    if(!chAt)
    {
        if(someWid->windowState()!=Qt::WindowMaximized)
        {
            if(cPos.x()<5)
            {
                changeWidthLeft=true;
            }
            else if(someWid->width()-cPos.x()<5)
            {
                changeWidthRight=true;
            }
            if(cPos.y()<5)
            {
                changeHeightTop=true;
            }
            else if(someWid->height()-cPos.y()<5)
            {
                changeHeightBottom=true;
            }
            if((changeWidthLeft||changeWidthRight||changeHeightTop||changeHeightBottom))
            {
                return;
            }
        }
        if (event->button() == Qt::LeftButton) {
            mPressed=true;
            pos = event->globalPos() - wPos;
            event->accept();
            qDebug()<<pos;
        }
    }
}

void VirtualFrame::mouseRelease(QMouseEvent *event)
{
    changeWidthLeft=false;
    changeWidthRight=false;
    changeHeightTop=false;
    changeHeightBottom=false;
    QRect rec = QApplication::desktop()->availableGeometry();
    if(event->globalPos().x()==0)
    {
        if(mPressed)
        {
            mPressed=false;
            someWid->resize(rec.width()/2,rec.height());
            someWid->move(0,0);
            maximizeWindow=true;
            return;
        }
    }
    if(event->globalPos().x()==rec.width()-1)
    {
        if(mPressed)
        {
            mPressed=false;
            someWid->resize(rec.width()/2,rec.height());
            someWid->move(rec.width()/2,0);
            maximizeWindow=true;
            return;
        }
    }
    if(event->globalPos().y()==0)
    {
        if(mPressed)
        {
            mPressed=false;
            someWid->setWindowState(Qt::WindowMaximized);
            return;
        }
        else
        {
            someWid->move(someWid->frameGeometry().topLeft().x(),0);
            someWid->resize(someWid->width(),rec.height());
            maximizeWindow=true;
            return;
        }
    }
    mPressed=false;
    if(someWid->windowState()!=Qt::WindowMaximized)
    {
        geom = someWid->saveGeometry();
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

void VirtualFrame::mouseMove(QMouseEvent *event)
{
    QPoint cPos=event->globalPos();
    QPoint wPos=someWid->frameGeometry().topLeft();
    if (mPressed) {
        if(someWid->windowState()==Qt::WindowMaximized||maximizeWindow)
        {
            double proportion = pos.x()/(double)someWid->width();
            int rightPos= someWid->width()-pos.x();
            someWid->setWindowState(Qt::WindowNoState);
            maximizeWindow=false;
            someWid->restoreGeometry(geom);
            if(proportion>0.5)
            {
                pos.setX(std::max(someWid->width()-rightPos,someWid->width()/2));
            }
            else
            {
                pos.setX(std::min(pos.x()+6,someWid->width()/2));
            }
            if(pos.y()<6)
            {
                pos.setY(6);
            }
        }
        someWid->move(event->globalPos() - pos);
        event->accept();
        return;
    }
    if(changeWidthLeft)
    {
        QRect opa=someWid->geometry();
        opa.setLeft(std::min(cPos.x(),wPos.x()+someWid->width()-someWid->minimumWidth()));
        someWid->setGeometry(opa);
    }
    if(changeWidthRight)
    {
        int dx=cPos.x()-wPos.x()-someWid->width();
        someWid->resize(someWid->width()+dx,someWid->height());
    }
    if(changeHeightTop)
    {
        QRect opa=someWid->geometry();
        opa.setTop(std::min(cPos.y(),wPos.y()+someWid->height()-someWid->minimumHeight()));
        someWid->setGeometry(opa);
    }
    if(changeHeightBottom)
    {
        int dy=cPos.y()-wPos.y()-someWid->height();
        someWid->resize(someWid->width(),someWid->height()+dy);
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
