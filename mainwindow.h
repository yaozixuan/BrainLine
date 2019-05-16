#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
class WidgetControl;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void Newfile();
    void ZoomIn();
    void ZoomOut();


private:
    Ui::MainWindow *ui;
    WidgetControl *m_widgetcontrol;
};

#endif // MAINWINDOW_H
