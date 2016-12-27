#include <QApplication>
#include <QSettings>
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
    setUpFrameGeometry();
}

VirtualFrame::~VirtualFrame()
{
    saveFrameSettings();
}

//Тут каждому евенту назначается обрабатывающая функция. Все функции ниже.
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
    QWidget* chAt=someWid->childAt(cPos);
    if(!chAt)
    {
        if(someWid->windowState()!=Qt::WindowMaximized)
        {
            if(cPos.x()<borderWidth)
            {
                changeWidthLeft=true;
            }
            else if(someWid->width()-cPos.x()<borderWidth)
            {
                changeWidthRight=true;
            }
            if(cPos.y()<borderWidth)
            {
                changeHeightTop=true;
            }
            else if(someWid->height()-cPos.y()<borderWidth)
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
            pos = event->globalPos() - someWid->frameGeometry().topLeft();
            event->accept();
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
    if(event->globalPos().x()==0||event->globalPos().x()==rec.width()-1)
    {
        if(mPressed)
        {
            mPressed=false;
            someWid->resize(rec.width()/2,rec.height());
            someWid->move(event->globalPos().x()/2,0);
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
    if(someWid->windowState()!=Qt::WindowMaximized&&!maximizeWindow)
    {
        geom = someWid->saveGeometry();
    }
}

void VirtualFrame::mouseHover(QMouseEvent *event)
{
    QPoint cPos=event->globalPos();
    QPoint wPos=someWid->frameGeometry().topLeft();

    bool right = wPos.x()+someWid->width()-cPos.x()<borderWidth;
    bool left = cPos.x()-wPos.x()<borderWidth;
    bool top = cPos.y()-wPos.y()<borderWidth;
    bool bottom = wPos.y()+someWid->height()-cPos.y()<borderWidth;

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
    if(moveFrame(event->globalPos()))
    {
        event->accept();
        return;
    }
    resizeFrameWithMouse(event->globalPos());
}

bool VirtualFrame::moveFrame(const QPoint& mousePosition)
{
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
                pos.setX(std::min(pos.x()+borderWidth,someWid->width()/2));
            }
            if(pos.y()<borderWidth)
            {
                pos.setY(borderWidth);
            }
        }
        someWid->move(mousePosition - pos);
        return true;
    }
    return false;
}

void VirtualFrame::resizeFrameWithMouse(const QPoint& mousePosition)
{
    QPoint wPos=someWid->frameGeometry().topLeft();
    if(changeWidthLeft)
    {
        QRect opa=someWid->geometry();
        opa.setLeft(std::min(mousePosition.x(),wPos.x()+someWid->width()-someWid->minimumWidth()));
        someWid->setGeometry(opa);
    }
    if(changeWidthRight)
    {
        int dx=mousePosition.x()-wPos.x()-someWid->width();
        someWid->resize(someWid->width()+dx,someWid->height());
    }
    if(changeHeightTop)
    {
        QRect opa=someWid->geometry();
        opa.setTop(std::min(mousePosition.y(),wPos.y()+someWid->height()-someWid->minimumHeight()));
        someWid->setGeometry(opa);
    }
    if(changeHeightBottom)
    {
        int dy=mousePosition.y()-wPos.y()-someWid->height();
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

void VirtualFrame::setUpFrameGeometry()
{
    QSettings settings(QString("%1\\%2").arg(QApplication::applicationDirPath()).arg("MViewsSettings.ini"),QSettings::IniFormat);
    geom = settings.value("Geometry").toByteArray();
    QPalette pal =someWid->palette();
    someWid->restoreGeometry(geom);
    someWid->setWindowState(Qt::WindowStates(settings.value("winState").toInt()));
    someWid->switcWMode(someWid->windowState());
    pal.setColor(QPalette::Background,settings.value("color").value<QColor>());
    someWid->setPalette(pal);
    someWid->installEventFilter(this);
    someWid->setAttribute(Qt::WA_Hover);
    setBorderWidth(5);
}

void VirtualFrame::saveFrameSettings()
{
    QSettings settings(QString("%1\\%2").arg(QApplication::applicationDirPath()).arg("MViewsSettings.ini"),QSettings::IniFormat);
    settings.setValue("geometry", geom);
    settings.setValue("color", someWid->palette().color(QPalette::Background));
    settings.setValue("winState", QString("%1").arg(someWid->windowState()));
}
