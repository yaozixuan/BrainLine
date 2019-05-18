#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include "widgetcontrol.h" //改
#include "logiccontrol.h" //改

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_contentChanged(false)
{
    // setup ui
    ui->setupUi(this);
    connect(ui->actionNew, SIGNAL(triggered()), this, SLOT(newFile()));
    connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(openFile()));
    connect(ui->actionSave, SIGNAL(triggered()), this, SLOT(saveFile()));
    connect(ui->actionSave_as, SIGNAL(triggered()), this, SLOT(saveFileAs()));
    connect(ui->actionClose, SIGNAL(triggered()), this, SLOT(closeFile()));
    connect(ui->actionExport, SIGNAL(triggered()), this, SLOT(exportScene()));
    connect(ui->actionQuit, SIGNAL(triggered()), this, SLOT(quit()));
    //zoom in zoom out
    connect(ui->actionZoom_In, SIGNAL(triggered()), this, SLOT(ZoomIn()));
    connect(ui->actionZoom_Out, SIGNAL(triggered()), this, SLOT(ZoomOut()));
    //help information
    connect(ui->actionHelp_Information, SIGNAL(triggered()), this, SLOT(help()));

    // graphwidget is hided by def, new/open file will show it
    m_graphicsView = new WidgetControl(this);
    setCentralWidget(m_graphicsView);
    m_graphicsView->hide();

    // Connect controller to detect content change
    connect(m_graphicsView->logicControl(), SIGNAL(contentChanged(const bool&)),
            this, SLOT(contentChanged(const bool&)));

    // Show notification message on Status Bar
    connect(m_graphicsView, SIGNAL(notification(QString)),
            this, SLOT(statusBarMsg(QString)));

    connect(m_graphicsView->logicControl(), SIGNAL(notification(QString)),
            this, SLOT(statusBarMsg(QString)));


    // setup toolbars, don't show them

    setupEditToolbar();
    ui->undoToolBar->hide();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::statusBarMsg(const QString &msg)  // msg for readonly file and two of toolbars
{
    ui->statusBar->showMessage(msg, 5000);
}

void MainWindow::contentChanged(const bool& changed)
//changed，true是改变动作发生了，m_contentchanged此时还没有变化。然后改变m_。并+*给文件名。
//另一种情况是m_changed完成后，changed被设定为false，此时消除*并改变文件状态
{
    // only care about the transitions
    if (m_contentChanged == false && changed == true)
    {
        setWindowTitle(windowTitle().prepend("* "));
        m_contentChanged = true;

        QFileInfo fileInfo(m_filename);
        if (m_filename != tr("untitled") && fileInfo.isWritable())
            ui->actionSave->setEnabled(true);
    }
    else if (m_contentChanged == true && changed == false)
    {
        setWindowTitle(windowTitle().replace("*",NULL));
        m_contentChanged = false;
        ui->actionSave->setEnabled(false);
    }
}

void MainWindow::newFile()    //open a new widget, set the options' states. give a initial name
{
    if (!closeFile())
        return;

    m_graphicsView->newScene();

    ui->actionSave->setEnabled(false);
    ui->actionSave_as->setEnabled(true);
    ui->actionClose->setEnabled(true);
    ui->actionExport->setEnabled(true);
    contentChanged(false);
    m_filename = tr("untitled");
    setTitle(m_filename);

    m_graphicsView->setFocus();
}

void MainWindow::openFile(const QString &fileName)      //use QfileDialog to get the path, , acceptmode to open.
{
    if (!closeFile())
        return;

    QString currFilename(m_filename);
    if (fileName.isEmpty())
    {
        QFileDialog *filedialog = new QFileDialog(this);
        filedialog->setWindowTitle(tr("Open BrainLine"));
        filedialog->setDirectory("");
        filedialog->setNameFilter(tr("BrainLine (*bl)"));

        filedialog->setAcceptMode(QFileDialog::AcceptOpen);
        filedialog->setDefaultSuffix("bl");

        if (!filedialog->exec())
            return;

        m_filename = filedialog->selectedFiles().first();
    }
    else
    {
        m_filename = fileName;
    }

    QFileInfo fileInfo(m_filename);
    if (!fileInfo.isWritable())
        statusBarMsg(tr("Read-only file!"));

    if (!m_graphicsView->logicControl()->readContentFromXmlFile(m_filename))          //this is to read the content in the file
    {
        m_filename = currFilename;
        return;
    }


    ui->actionSave_as->setEnabled(true);                      //set its states, mainly for save option and contentChanged()
    ui->actionClose->setEnabled(true);
    ui->actionExport->setEnabled(true);
    ui->actionSave->setEnabled(false);
    ui->actionSave->setEnabled(false);

    contentChanged(false);

    fileInfo.isWritable() ?
         setTitle(m_filename) :
         setTitle(tr("readonly ").append(m_filename));

}

void MainWindow::saveFile(const bool &checkIfReadonly)   //use bool input and writable to decide save or not
{
    QFileInfo fileInfo(m_filename);
    if (checkIfReadonly && !fileInfo.isWritable())
    {
        statusBarMsg(tr("Read-only file!"));
        return;
    }

    m_graphicsView->logicControl()->writeContentToXmlFile(m_filename);     //the path to save
    contentChanged(false);

    m_undoStack->clear();
}

bool MainWindow::saveFileAs()     //the same logic to find, different in acceptmode
{
    QFileDialog *filedialog = new QFileDialog(this);

    filedialog->setWindowTitle(tr("Save BrainLine"));
    filedialog->setDirectory("");
    filedialog->setNameFilter(tr("BrainLine (*.bl)"));

    filedialog->setAcceptMode(QFileDialog::AcceptSave);
    filedialog->setDefaultSuffix("bl");
    if (!filedialog->exec())
        return false;

    m_filename = filedialog->selectedFiles().first();
    saveFile(false);
    setTitle(m_filename);
    return true;

}

bool MainWindow::closeFile()        //give a msgbox to choose, switch cases to go on
{
    if (m_contentChanged)
    {
        QMessageBox msgBox(this);
        msgBox.setWindowTitle(tr("Save brainline - BrainLine"));
        msgBox.setText(tr("Do you want to save your changes?"));
        msgBox.setStandardButtons(QMessageBox::Save |
                                  QMessageBox::Discard |
                                  QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Save);
        int ret = msgBox.exec();

        switch (ret) {
        case QMessageBox::Save:
        {
            if (m_filename == tr("untitled"))
            {
                if (!saveFileAs())
                        return false;
            }
            else
            {
                saveFile();
            }

            break;
        }
        case QMessageBox::Discard:
            break;
        case QMessageBox::Cancel:
            return false;
        default:
            break;
        }
    }

    ui->actionSave->setEnabled(false);
    ui->actionSave_as->setEnabled(false);
    ui->actionClose->setEnabled(false);
    ui->actionExport->setEnabled(false);
    m_contentChanged = false;
    setTitle("");
    m_graphicsView->closeScene();
    m_undoStack->clear();
    showUndoToolbar(false);
    return true;
}

void MainWindow::exportScene() // get path
{
    QFileDialog *picdialog = new QFileDialog(this);

    picdialog->setWindowTitle(tr("Export as a picture"));
    picdialog->setDirectory("");
    picdialog->setNameFilter(tr("Picture (*.png)"));

    picdialog->setAcceptMode(QFileDialog::AcceptSave);
    picdialog->setDefaultSuffix("png");

     if (picdialog->exec())
     {
         m_graphicsView->logicControl()->writeContentToPngFile(
                                            picdialog->selectedFiles().first());
     }
}

void MainWindow::ZoomIn(){
    m_graphicsView->zoomIn();
//    std::cout << "Zoom In" << std::endl;
}

void MainWindow::ZoomOut(){
    m_graphicsView->zoomOut();
//    std::cout << "Zoom out" << std::endl;
}

void MainWindow::help(){
    /* New a QMessageBox to display the help information */
    QMessageBox *msgBox = new QMessageBox();
//    msgBox = new QMessageBox();
    msgBox->setWindowTitle(tr("About BrainLine"));
    msgBox->setText(tr("BrainLIne software written in Qt.<br><br>").
                    append(tr("Report bugs to:<br>")).
                    append(" <a href=\"mailto:zixuanyao@link.cuhk.edu.cn\">" "zixuanyao@link.cuhk.edu.cn</a>"));
    QPixmap pixmap(":/image/brainline.svg");
    msgBox->setIconPixmap(pixmap.scaled(85,85));
//    msgBox->setIcon(QMessageBox::NoIcon);

    msgBox->exec();
}
//void MainWindow::help() // give msgbox for help information
//{
//    QMessageBox msgBox;
//    msgBox.setWindowTitle(tr("About BrainLine"));
//    msgBox.setText(tr("BrainLine software written in Qt."));
//    msgBox.setTextFormat(Qt::RichText);
//    msgBox.setInformativeText(tr("Report bugs to:").append(" <a href=\"mailto:zixuanyao@link.cuhk.edu.cn\">" "zixuanyao@link.cuhk.edu.cn</a>"));
//    QPixmap pixMap(":/brainline.svg");
//    msgBox.setIconPixmap(pixMap.scaled(50,50));
//    msgBox.exec();
//}

void MainWindow::showUndoToolbar(const bool &show)
{
    ui->undoToolBar->setVisible(show ? !ui->undoToolBar->isVisible() : false);
}

void MainWindow::quit()               //quit program without request
{
//    if (m_contentChanged && !closeFile())
//        return;

    QApplication::instance()->quit();
}

void MainWindow::closeEvent(QCloseEvent * event) //press the x 关闭button and will jump out the closefile function
{
    m_contentChanged && !closeFile() ?
                event->ignore() :
                event->accept();
}

void MainWindow::setupEditToolbar()
{
        m_undoStack = new QUndoStack(this);
        m_undoView = new QUndoView(m_undoStack,this);
        ui->undoToolBar->addWidget(m_undoView);

        m_undo = m_undoStack->createUndoAction(this, tr("&Undo"));
        m_undo->setShortcuts(QKeySequence::Undo);
        m_redo = m_undoStack->createRedoAction(this, tr("&Redo"));
        m_redo->setShortcuts(QKeySequence::Redo);
        ui->menuView->addAction(m_undo);
        ui->menuView->addAction(m_redo);

        ui->menuView->addSeparator();

        m_undoToolbar = new QAction(tr("undo toolbar"), this);
        m_undoToolbar->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_U));
        connect(m_undoToolbar, SIGNAL(triggered()),
                this, SLOT (showUndoToolbar()));

        ui->menuView->addAction(m_undoToolbar);

        m_graphicsView->logicControl()->setUndoStack(m_undoStack);
}

void MainWindow::setTitle(const QString &title) //clear the old path if there is no title
{
    title.isEmpty() ?
         setWindowTitle("BrainLine") :
         setWindowTitle(QString(title).append(" - BrainLine"));
}
