#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->actionHelp_Information, SIGNAL(triggered()), this, SLOT(help()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::help(){
    /* New a QMessageBox to display the help information */
    QMessageBox *msgBox = new QMessageBox();
//    msgBox = new QMessageBox();
    msgBox->setWindowTitle(tr("About BrainLine"));
    msgBox->setText(tr("BrainLIne software written in Qt.<br><br>").
                    append(tr("Report bugs to:<br>")).
                    append(" <a href=\"mailto:zixuanyao@link.cuhk.edu.cn\">" \
                    "zixuanyao@link.cuhk.edu.cn</a>"));
    QPixmap pixmap(":/image/brainline.svg");
    msgBox->setIconPixmap(pixmap.scaled(85,85));
//    msgBox->setIcon(QMessageBox::NoIcon);

    msgBox->exec();
}
