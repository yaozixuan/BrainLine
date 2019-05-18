#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QSignalMapper>
#include <QUndoView>

//#include "graphwidget.h"
class WidgetControl; //改

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

    // show the Information on the status bar
    void statusBarMsg(const QString &msg);

    // indicate that content has changed, modify title, save actions
    void contentChanged(const bool &changed = true);

    // filemenu actions
    void newFile();
    void openFile(const QString &fileName = "");
    void saveFile(const bool &checkIfReadonly = true);
    bool saveFileAs();
    bool closeFile();
    void exportScene();
    void ZoomIn();
    void ZoomOut();
    void help();

    // toolbars
    void showUndoToolbar(const bool &show = true);

    // handle changed content at quit
    void quit();

protected:

    // handle changed content at exit events
    void closeEvent(QCloseEvent * event);

private:
    Ui::MainWindow *ui;
    WidgetControl *m_graphicsView;
    QString m_filename;
    bool m_contentChanged;

    QUndoStack *m_undoStack;
    QUndoView *m_undoView;
    QAction *m_undo;
    QAction *m_redo;
    QAction *m_mainToolbar;
    QAction *m_iconToolbar;
    QAction *m_undoToolbar;

    void setupEditToolbar();
    void setTitle(const QString &title);

};

#endif // MAINWINDOW_H

/** Responsibilities:
  * - Menu and toolbars, file operations
  * - Display info in statusbar
  * - Handle content change notifications
  * - Pass unhandled key events to GraphWidget
  */
