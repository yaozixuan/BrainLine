#include "mainwindow.h"
#include <QApplication>
#include <QPushButton>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("BrainLine");
    w.setWindowIcon(QIcon(":/image/brainline.svg"));
    w.show();

    return a.exec();
}
