#include <QMouseEvent>
#include <QMessageBox>
#include "titlebartest.h"
#include "ui_titlebartest.h"

TitleBarTest::TitleBarTest(QWidget *parent) :
    QWidget(parent, Qt::FramelessWindowHint | Qt::WindowSystemMenuHint),
    ui(new Ui::TitleBarTest)
{
    ui->setupUi(this);
    QPalette pal =palette();
    pal.setColor(QPalette::Background,
                 Qt::black);
    QPalette pal1 =palette();
    pal1.setColor(QPalette::Background,
                 Qt::white);
    ui->widget->setPalette(pal1);
    this->setPalette(pal);

}

void TitleBarTest::mousePressEvent(QMouseEvent *event)
{
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


    bool right = wPos.x()+width()-cPos.x()<5;
    bool left = cPos.x()-wPos.x()<5;
    bool top = cPos.y()-wPos.y()<5;
    bool bottom = wPos.y()+height()-cPos.y()<5;
    QCursor opa;
    if((left&&top)||(right&&bottom))
    {
        opa.setShape(Qt::SizeFDiagCursor);
        QMessageBox opaB;
        opaB.exec();
    }
    if((right&&top)||(left&&bottom))
    {
        opa.setShape(Qt::SizeBDiagCursor);
        QMessageBox opaB;
        opaB.exec();
    }
    if(right||left)
    {
        opa.setShape(Qt::SizeHorCursor);
        QMessageBox opaB;
        opaB.exec();
    }
    if(top||bottom)
    {
        opa.setShape(Qt::SizeVerCursor);
        QMessageBox opaB;
        opaB.exec();
    }
    opa.setShape(Qt::SizeBDiagCursor);



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



}

TitleBarTest::~TitleBarTest()
{
    delete ui;
}


void TitleBarTest::on_pushButton_clicked()
{
    this->close();
}

void TitleBarTest::on_pushButton_2_clicked()
{
    QPoint wPos=frameGeometry().topLeft();
    QMessageBox opa;
    opa.setText(QString("x: %1 and y: %2").arg(wPos.x()).arg(wPos.y()));
    opa.exec();
}
