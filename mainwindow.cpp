#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "widgetcontrol.h"
#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->actionNew, SIGNAL(triggered()), this, SLOT(Newfile()));
    connect(ui->actionZoom_In, SIGNAL(triggered()), this, SLOT(ZoomIn()));
    connect(ui->actionZoom_Out, SIGNAL(triggered()), this, SLOT(ZoomOut()));

    m_widgetcontrol = new WidgetControl(this);
    setCentralWidget(m_widgetcontrol);
    m_widgetcontrol->hide();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::Newfile(){
    m_widgetcontrol->NewBrainLine();
}

void MainWindow::ZoomIn(){
    m_widgetcontrol->ZoomIn();
//    std::cout << "Zoom In" << std::endl;
}

void MainWindow::ZoomOut(){
    m_widgetcontrol->ZoomOut();
//    std::cout << "Zoom out" << std::endl;
}
