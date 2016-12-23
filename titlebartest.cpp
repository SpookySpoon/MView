#include <QSettings>
#include <QColorDialog>
#include <QHoverEvent>
#include <QDebug>
#include <QMouseEvent>
#include <QMessageBox>
#include <QApplication>
#include "titlebartest.h"
#include "ui_titlebartest.h"

TitleBarTest::TitleBarTest(QWidget *parent) :
    QWidget(parent, Qt::FramelessWindowHint | Qt::WindowSystemMenuHint),
    ui(new Ui::TitleBarTest)
{
    ui->setupUi(this);
    QSettings settings(QString("%1\\%2").arg(QApplication::applicationDirPath()).arg("MViewsSettings.ini"),QSettings::IniFormat);
    QPoint initPos(settings.value("WindowPosX","").toInt(),settings.value("WindowPosY","").toInt());
    int tHeight = settings.value("WindowHeight","").toInt();
    int tWidth = settings.value("WindowWidth","").toInt();
    QStringList wColorRGB=settings.value("WindowColor","").toString().split("_");
    QPalette pal =palette();
    if(wColorRGB.count()!=3)
    {
        pal.setColor(QPalette::Background,Qt::black);
    }
    else
    {
        QColor wCollor=QColor(QString(wColorRGB.at(0)).toInt(),QString(wColorRGB.at(1))
                              .toInt(),QString(wColorRGB.at(2)).toInt());
        pal.setColor(QPalette::Background,wCollor);
    }
    if(tHeight==0||tWidth==0)
    {
        this->resize(500,500);
    }
    else
    {
        this->resize(tWidth,tHeight);
    }

    QPalette pal1 =palette();
    pal1.setColor(QPalette::Background,Qt::white);
    ui->widget->setPalette(pal1);
    this->setPalette(pal);
    this->installEventFilter(this);
    this->setAttribute(Qt::WA_Hover);
    QAction *colorChange = new QAction(tr("Color"), this);
    connect(colorChange, SIGNAL(triggered()), this, SLOT(chooseColor()));
    addAction(colorChange);
//    setContextMenuPolicy(Qt::ActionsContextMenu);
}

TitleBarTest::~TitleBarTest()
{
    QSettings settings(QString("%1\\%2").arg(QApplication::applicationDirPath()).arg("MViewsSettings.ini"),QSettings::IniFormat);
    settings.setValue("WindowHeight",this->height());
    settings.setValue("WindowWidth",this->width());
    QPoint wPos=frameGeometry().topLeft();
    settings.setValue("WindowPosX",wPos.x());
    settings.setValue("WindowPosY",wPos.y());
    QColor saveCol=this->palette().color(QPalette::Background);
    settings.setValue("WindowColor",QString("%1_%2_%3").arg(saveCol.red()).arg(saveCol.green()).arg(saveCol.blue()));
    delete ui;
}

void TitleBarTest::chooseColor()
{

    QColor color= QColorDialog::getColor(Qt::green, this);
    QPalette pal =palette();
    pal.setColor(QPalette::Background,
                 color);
    this->setPalette(pal);
}

void TitleBarTest::mousePressEvent(QMouseEvent *event)
{
    mPressedMark=true;
    QPoint cPos=event->pos();
    QPoint wPos=frameGeometry().topLeft();
    QWidget* chAt=this->childAt(cPos);
    if(!chAt)
    {
        if(cPos.x()<5)
        {
            changeWidthLeft=true;
        }
        else if(width()-cPos.x()<5)
        {
            changeWidthRight=true;
        }
        if(cPos.y()<5)
        {
            changeHeightTop=true;
        }
        else if(height()-cPos.y()<5)
        {
            changeHeightBottom=true;
        }
        if(changeWidthLeft||changeWidthRight||changeHeightTop||changeHeightBottom)
        {
            return;
        }
        if (event->button() == Qt::LeftButton) {
            mPressed=true;
            pos = event->globalPos() - wPos;
            event->accept();
        }
    }
}
void TitleBarTest::mouseReleaseEvent(QMouseEvent *event)
{
    mPressedMark=false;
    if (event->button() == Qt::LeftButton) {
        mPressed=false;
    }
    changeWidthLeft=false;
    changeWidthRight=false;
    changeHeightTop=false;
    changeHeightBottom=false;
}

void TitleBarTest::mouseMoveEvent(QMouseEvent *event)
{
    QPoint cPos=event->globalPos();
    QPoint wPos=frameGeometry().topLeft();

    if (mPressed) {
        move(event->globalPos() - pos);
        event->accept();
        return;
    }
    QSize nSize=size();
    int dx=0,dy=0;
    if(changeWidthLeft)
    {
        dx=wPos.x()-std::min(cPos.x(),wPos.x()+width()-minimumWidth())-dx;
        wPos.setX(std::min(cPos.x(),wPos.x()+width()-minimumWidth()));
        this->move(wPos);
    }
    if(changeWidthRight)
    {
        dx=cPos.x()-wPos.x()-width();
    }
    if(changeHeightTop)
    {
        dy=wPos.y()-std::min(cPos.y(),wPos.y()+height()-minimumHeight())-dy;
        wPos.setY(std::min(cPos.y(),wPos.y()+height()-minimumHeight()));
        this->move(wPos);
    }
    if(changeHeightBottom)
    {
        dy=cPos.y()-wPos.y()-height();
    }
    nSize.setWidth(nSize.width()+dx);
    nSize.setHeight(nSize.height()+dy);
    this->resize(nSize);

    this->setCursor(opaCurs);

}



bool TitleBarTest::eventFilter(QObject *object, QEvent *event)
{
    QPoint cPos=QCursor::pos();
    QPoint wPos=frameGeometry().topLeft();
    if (object == this && event->type() == QEvent::HoverMove&&!mPressedMark) {
        bool right = wPos.x()+width()-cPos.x()<5;
        bool left = cPos.x()-wPos.x()<5;
        bool top = cPos.y()-wPos.y()<5;
        bool bottom = wPos.y()+height()-cPos.y()<5;

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

        this->setCursor(opaCurs);
        return true;
    }
    return false;
}

void TitleBarTest::on_buttonClose_clicked()
{
    this->close();
}

void TitleBarTest::on_puttonRestoreWin_clicked()
{
    if(this->windowState()==Qt::WindowMaximized)
    {
        this->setWindowState(Qt::WindowNoState);
    }
    else
    {
        this->setWindowState(Qt::WindowMaximized);
    }
}

void TitleBarTest::on_buttonMinimize_clicked()
{
    this->setWindowState(Qt::WindowMinimized);
}
