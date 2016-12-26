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
//    QFileSystemModel model;
//    model.setRootPath("/");
//    QTreeView tView;
//    tView.setModel(&model);
////    tView.setSortingEnabled(true);
//    tView.header()->setSectionResizeMode(QHeaderView::ResizeToContents);
//    tView.resize(640, 480);
//    tView.show();
    return a.exec();
}
