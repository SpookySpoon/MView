#include <QFileSystemModel>
#include <QHeaderView>
#include <QTreeView>
#include <QApplication>
#include "titlebartest.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TitleBarTest t;
    t.show();
    return a.exec();
}
