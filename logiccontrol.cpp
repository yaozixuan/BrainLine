#include "logiccontrol.h"

LogicControl::LogicControl(WidgetControl *parent)
    : QObject(parent)
{
    m_memberMap.insert(std::pair<int, void(LogicControl::*)()>
                       (Qt::Key_Insert, &LogicControl::InsertNode));
}

void LogicControl::RemoveAllNodes(){

}

void LogicControl::AddFirstNode(){

}

void LogicControl::InsertNode(){
    if (!m_activeNode)
    {
        emit notification(tr("No active node."));
        return;
    }

    // get the biggest angle between the edges of the Node.
    double angle(m_activeNode->calculateBiggestAngle());

    // let the distance between the current and new Node be 100 pixels
    qreal length(100);

    QPointF pos(m_activeNode->sceneBoundingRect().center() +
                 QPointF(length * cos(angle), length * sin(angle)) -
                 Node::newNodeCenter);

    QRectF rect (m_graphWidget->scene()->sceneRect().topLeft(),
                 m_graphWidget->scene()->sceneRect().bottomRight()
                 - Node::newNodeBottomRigth);

    if (!rect.contains(pos))
    {
        emit notification(tr("New node would be placed outside of the scene."));
        return;
    }

    UndoContext context;
    context.m_graphLogic = this;
    context.m_nodeList = &m_nodeList;
    context.m_activeNode = m_activeNode;
    context.m_pos = pos;


    QUndoCommand *insertNodeCommand = new InsertNodeCommand(context);
    m_undoStack->push(insertNodeCommand);
}

void LogicControl::RemoveNode(){
    if (!m_activeNode)
    {
        emit notification(tr("No active node."));
        return;
    }

    if (m_activeNode == m_nodeList.first())
    {
        emit notification(tr("Base node cannot be deleted."));
        return;
    }

    UndoContext context;
    context.m_graphLogic = this;
    context.m_nodeList = &m_nodeList;
    context.m_activeNode = m_activeNode;
    context.m_hintNode = m_hintNode;

    QUndoCommand *insertNodeCommand = new RemoveNodeCommand(context);
    m_undoStack->push(insertNodeCommand);
}

