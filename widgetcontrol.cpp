#include "widgetcontrol.h"
#include "logiccontrol.h" //改

//#include <QDebug>

#include "brainnode.h"
#include "brainarc.h"
#include "mainwindow.h"

//#include <math.h>
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

    m_graphlogic = new LogicControl(this);

    // Right Click Menu, menu content and connect signal and slots
    m_RightMenu = new QMenu;
    m_AddNode = new QAction(tr("Add Node"), this);
    connect(m_AddNode, SIGNAL(triggered()), m_graphlogic, SLOT(insertNode()));
    m_RightMenu->addAction(m_AddNode);

    m_RemoveNode = new QAction(tr("Remove Node"), this);
    connect(m_RemoveNode, SIGNAL(triggered()), m_graphlogic, SLOT(removeNode()));
    m_RightMenu->addAction(m_RemoveNode);

    m_AddArc = new QAction(tr("Add Arc"), this);
    connect(m_AddArc, SIGNAL(triggered()), m_graphlogic, SLOT(addEdge()));
    m_RightMenu->addAction(m_AddArc);

    m_RemoveArc = new QAction(tr("Remove Arc"), this);
    connect(m_RemoveArc, SIGNAL(triggered()), m_graphlogic, SLOT(removeEdge()));
    m_RightMenu->addAction(m_RemoveArc);

    m_NodeColor = new QAction(tr("Edit Node Color"), this);
    connect(m_NodeColor, SIGNAL(triggered()), m_graphlogic, SLOT(nodeColor()));
    m_RightMenu->addAction(m_NodeColor);

    m_TextColor = new QAction(tr("Edit Text Color"),this);
    connect(m_TextColor, SIGNAL(triggered()), m_graphlogic, SLOT(nodeTextColor()));
    m_RightMenu->addAction(m_TextColor);

//    setCacheMode(CacheBackground);
//    setViewportUpdateMode(BoundingRectViewportUpdate);
//    setRenderHint(QPainter::Antialiasing);
//    setTransformationAnchor(AnchorUnderMouse);
//    setMinimumSize(400, 400);


}

void WidgetControl::newScene()
{
    m_graphlogic->removeAllNodes();
    m_graphlogic->addFirstNode();

    this->show();
}

void WidgetControl::closeScene()
{
    m_graphlogic->removeAllNodes();
    this->hide();

}

LogicControl *WidgetControl::logicControl() const
{
    return m_graphlogic;
}

void WidgetControl::zoomIn()
{
    scaleView(qreal(0.2));
}

void WidgetControl::zoomOut()
{
    scaleView(qreal(-0.2));
}


// MainWindow::keyPressEvent passes all keyevent to here, except
// Ctrl + m (show/hide mainToolBar) and Ctrl + i (show/hide statusIconsToolbar)
void WidgetControl::keyPressEvent(QKeyEvent *event)
{
    // if GraphLogic handles the event then stop.
//    if (m_graphlogic->processKeyEvent(event))
//        return;

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
        m_graphlogic->insertNode();
    }

    if (event->key() == Qt::Key_Delete){
        m_graphlogic->removeNode();
    }

    if (event->key() == Qt::Key_A){
        m_graphlogic->addEdge();
    }

    if (event->key() == Qt::Key_D){
        m_graphlogic->removeEdge();
    }

    m_graphlogic->processKeyEvent(event);

    QGraphicsView::keyPressEvent(event); //继续保留原有函数

}

// Create Right Click Menu
void WidgetControl::mousePressEvent(QMouseEvent *event){
    if (event->button() == Qt::RightButton)
    {
        m_RightMenu->exec(event->globalPos());
    }
        //要继续保留QListWidget原有的点击事件.
    QGraphicsView::mousePressEvent(event);
}

void WidgetControl::mouseDoubleClickEvent(QMouseEvent *event){
    if(event->button() == Qt::LeftButton){
        m_graphlogic->nodeEdited();
    }
    if(event->button() == Qt::RightButton){
        m_graphlogic->insertNode();
    }
    QGraphicsView::mouseDoubleClickEvent(event);
}

void WidgetControl::wheelEvent(QWheelEvent *event)
{
    event->modifiers() & Qt::ControlModifier ?
                (event->delta() > 0 ?
                        m_graphlogic->scaleUp() :
                        m_graphlogic->scaleDown()) :
                (event->delta() > 0 ?
                        zoomIn() :
                        zoomOut());
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
