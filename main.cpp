#include <QDesktopWidget>
#include <QFileSystemModel>
#include <QTimer>
#include <QHeaderView>
#include <QTreeView>
#include <QApplication>
#include "titlebartest.h"
#include "resizermaf.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TitleBarTest t;
    t.show();

//    QWidget opaW(nullptr,Qt::FramelessWindowHint | Qt::WindowSystemMenuHint);
//    resW resizeMan(&opaW);

    return a.exec();
}
