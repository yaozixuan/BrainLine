#include "widgetcontrol.h"
#include "logiccontrol.h"
#include "brainnode.h"
#include "brainarc.h"
#include "mainwindow.h"
#include <QMenu>

const QColor WidgetControl::m_paperColor(248,248,255); // Set background color

WidgetControl::WidgetControl(MainWindow *parent)
    : QGraphicsView(parent)
//    , m_parent(parent)
{
    m_scene = new QGraphicsScene(this);
    m_scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    m_scene->setSceneRect(-400, -400, 800, 800);
    setScene(m_scene);

    m_logic = new LogicControl(this);

    // Right Click Menu, menu content and connect signal and slots
    m_RightMenu = new QMenu;
    m_AddNode = new QAction(tr("Add Node"), this);
    connect(m_AddNode, SIGNAL(triggered()), m_logic, SLOT(insertNode()));
    m_RightMenu->addAction(m_AddNode);

    m_RemoveNode = new QAction(tr("Remove Node"), this);
    connect(m_RemoveNode, SIGNAL(triggered()), m_logic, SLOT(removeNode()));
    m_RightMenu->addAction(m_RemoveNode);

    m_AddArc = new QAction(tr("Add Arc"), this);
    connect(m_AddArc, SIGNAL(triggered()), m_logic, SLOT(addEdge()));
    m_RightMenu->addAction(m_AddArc);

    m_RemoveArc = new QAction(tr("Remove Arc"), this);
    connect(m_RemoveArc, SIGNAL(triggered()), m_logic, SLOT(removeEdge()));
    m_RightMenu->addAction(m_RemoveArc);

    m_NodeColor = new QAction(tr("Edit Node Color"), this);
    connect(m_NodeColor, SIGNAL(triggered()), m_logic, SLOT(nodeColor()));
    m_RightMenu->addAction(m_NodeColor);

    m_TextColor = new QAction(tr("Edit Text Color"),this);
    connect(m_TextColor, SIGNAL(triggered()), m_logic, SLOT(nodeTextColor()));
    m_RightMenu->addAction(m_TextColor);
}

void WidgetControl::newScene()
{
    m_logic->removeAllNodes();
    m_logic->addFirstNode();

    this->show();
}

void WidgetControl::closeScene()
{
    m_logic->removeAllNodes();
    this->hide();

}

LogicControl *WidgetControl::logicControl() const
{
    return m_logic;
}

void WidgetControl::zoomIn()
{
    scale(1 + 0.2, 1 + 0.2);
}

void WidgetControl::zoomOut()
{
    scale(1 - 0.2, 1 - 0.2);
}

/* Key Press Event will be dealt with here, creating shortcut */
void WidgetControl::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Plus)
    {
        zoomIn();
        return;
    }

    if (event->key() == Qt::Key_Minus)
    {
        zoomOut();
        return;
    }

    if (event->key() == Qt::Key_Insert){
        m_logic->insertNode();
    }

    if (event->key() == Qt::Key_Delete){
        m_logic->removeNode();
    }

    if (event->key() == Qt::Key_A){
        m_logic->addEdge();
    }

    if (event->key() == Qt::Key_D){
        m_logic->removeEdge();
    }

    QGraphicsView::keyPressEvent(event); //继续保留QGraphicsView原有的键盘点击事件.

}

// Create Right Click Menu
void WidgetControl::mousePressEvent(QMouseEvent *event){
    if (event->button() == Qt::RightButton)
    {
        m_RightMenu->exec(event->globalPos());
    }
        //继续保留QGraphicsView原有的鼠标点击事件.
    QGraphicsView::mousePressEvent(event);
}

void WidgetControl::mouseDoubleClickEvent(QMouseEvent *event){
    if(event->button() == Qt::LeftButton){
        m_logic->nodeEdited();
    }
    if(event->button() == Qt::RightButton){
        m_logic->insertNode();
    }
    QGraphicsView::mouseDoubleClickEvent(event);
}

void WidgetControl::wheelEvent(QWheelEvent *event)
{
    if (Qt::ControlModifier & event->modifiers()){ // Press control + wheel modify the node
        if (event->delta() > 0) m_logic->scaleUp();
        else m_logic->scaleDown();
    }
    else { // modify the graph
        if (event->delta() > 0) zoomIn();
        else zoomOut();
        }
}

void WidgetControl::drawBackground(QPainter *painter, const QRectF &rect)
{
    Q_UNUSED(rect);

    painter->fillRect(m_scene->sceneRect(), WidgetControl::m_paperColor); // Set background color
    painter->setBrush(Qt::NoBrush);
    painter->drawRect(m_scene->sceneRect());
}

void WidgetControl::scaleView(qreal factor)
{
    // don't allow to scale up/down too much
//    qreal viewScale = transform().m11() + 1 + factor;
//    if (viewScale < qreal(1) || viewScale > qreal(10))
//    {
//        emit notification(tr("Too much zooming."));
//        return;
//    }

    scale(1 + factor, 1 + factor);
}
