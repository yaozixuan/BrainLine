#include "widgetcontrol.h"
#include "logiccontrol.h"
#include "mainwindow.h"
#include <QGraphicsView>

const QColor WidgetControl::m_paperColor(248,248,255); // Set background color

WidgetControl::WidgetControl(MainWindow *parent)
//    : QGraphicsView(parent)
{
    m_scene = new QGraphicsScene(this);
    m_scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    m_scene->setSceneRect(-400, -400, 800, 800);
    setScene(m_scene);

    m_logic = new LogicControl(this); // New a pointer to call LogicControl functions
}

void WidgetControl::NewBrainLine()
{
    m_logic->RemoveAllNodes();
    m_logic->AddFirstNode();

    this->show();
}

void WidgetControl::CloseBrainLine()
{
    m_logic->RemoveAllNodes();

    this->hide();
}

void WidgetControl::ZoomIn()
{
//    Zoom(qreal(0.2));
    scale(1 + 0.2, 1 + 0.2);
}

void WidgetControl::ZoomOut()
{
//    Zoom(qreal(-0.2));
    scale(1 - 0.2, 1 - 0.2);
}

//void WidgetControl::Zoom(qreal factor){
//    // don't allow to scale up/down too much
////    qreal viewScale = transform().m11() + 1 + factor;
////    if (viewScale < qreal(1) || viewScale > qreal(10))
////    {
////        emit notification(tr("Too much zooming."));
////        return;
////    }

//    scale(1 + factor, 1 + factor);
//}

void WidgetControl::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Insert){
        m_logic->InsertNode();
    }

    if (event->key() == Qt::Key_Delete){
        m_logic->RemoveNode();
    }

//    if (event->key() == Qt::Key_Escape)
//    {
//        m_logic->NodeLostFocus();
////        return true;
//    }
//    if (m_editingNode)
//    {
//        m_activeNode->keyPressEvent(event);
////        return true;
//    }
}

void WidgetControl::drawBackground(QPainter *painter, const QRectF &rect)
{
    Q_UNUSED(rect);

    painter->fillRect(m_scene->sceneRect(), WidgetControl::m_paperColor); // Set background color
    painter->setBrush(Qt::NoBrush);
    painter->drawRect(m_scene->sceneRect());
}

void WidgetControl::wheelEvent(QWheelEvent *event)
{
//    event->modifiers() & Qt::ControlModifier ?
//                (event->delta() > 0 ? m_logic->scaleUp() : m_logic->scaleDown()) : // Zoom Node
                (event->delta() > 0 ? ZoomIn() : ZoomOut()); // Zoom Widget
}

