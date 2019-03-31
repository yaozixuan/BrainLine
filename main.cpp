#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("BrainLine");
    w.setWindowIcon(QIcon("logo.jpg"));
    w.show();

    return a.exec();
}
