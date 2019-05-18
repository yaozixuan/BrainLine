#include "command.h"
#include "brainnode.h"

#include <QDebug>
#include <QApplication>

#include <math.h>


BaseUndoClass::BaseUndoClass(UndoContext context)
    : m_done(false)
    , m_context(context)
    , m_activeNode(context.m_activeNode)
{
    m_nodeList.push_back(m_activeNode);
}



InsertNodeCommand::InsertNodeCommand(UndoContext context)
    : BaseUndoClass(context)
{
    setText(QObject::tr("New Node added"));
    m_context.m_graphLogic->nodeLostFocus();

    // create new node which inherits the color and textColor
    m_node = m_context.m_graphLogic->nodeFactory();
    m_node->setColor(m_activeNode->color());
    m_node->setTextColor(m_activeNode->textColor());
    m_node->setHtml(QString(""));

    // new edge inherits colors and size from target
    m_edge = new BrainArc(m_activeNode, m_node);
    m_edge->setColor(m_node->color());
    m_edge->setWidth(m_node->scale()*2 + 1);
}

InsertNodeCommand::~InsertNodeCommand()
{
    if (!m_done)
    {
        delete m_edge;
        delete m_node;
    }
}

void InsertNodeCommand::undo()
{
    // remove node
    m_context.m_nodeList->removeAll(m_node);
    m_context.m_graphLogic->graphWidget()->scene()->removeItem(m_node);
    m_context.m_graphLogic->setActiveNode(m_activeNode);

    // remove edge
    m_edge->sourceNode()->removeEdge(m_edge);
    m_edge->destNode()->removeEdge(m_edge);
    m_context.m_graphLogic->graphWidget()->scene()->removeItem(m_edge);

//    m_context.m_graphLogic->reShowNumbers();
    m_done = false;
}

void InsertNodeCommand::redo()
{
    // add node
    m_context.m_graphLogic->graphWidget()->scene()->addItem(m_node);
    m_context.m_nodeList->append(m_node);
    m_node->setPos(m_context.m_pos);
    m_context.m_graphLogic->setActiveNode(m_node);

    if (m_context.m_graphLogic->graphWidget()->hasFocus())
        m_context.m_graphLogic->nodeEdited();

    // add edge
    m_edge->sourceNode()->addEdge(m_edge,true);
    m_edge->destNode()->addEdge(m_edge,false);
    m_context.m_graphLogic->graphWidget()->scene()->addItem(m_edge);

    m_done = true;
}

RemoveNodeCommand::RemoveNodeCommand(UndoContext context)
    : BaseUndoClass(context)
{
    setText(QObject::tr("Node removed"));

    // collect affected edges
    foreach(BrainNode *node, m_nodeList)
        foreach(BrainArc *edge, node->edges())
            if (m_edgeList.indexOf(edge) == -1)
                m_edgeList.push_back(edge);
}

void RemoveNodeCommand::undo()
{
    // add nodes
    foreach (BrainNode *node, m_nodeList)
    {
        m_context.m_graphLogic->graphWidget()->scene()->addItem(node);
        m_context.m_nodeList->append(node);
    }

    // add edges
    foreach (BrainArc *edge, m_edgeList)
    {
        edge->sourceNode()->addEdge(edge,true);
        edge->destNode()->addEdge(edge,false);
        m_context.m_graphLogic->graphWidget()->scene()->addItem(edge);
    }

    m_context.m_graphLogic->setActiveNode(m_activeNode);
}

void RemoveNodeCommand::redo()
{
    foreach(BrainNode *node, m_nodeList)
    {
        m_context.m_nodeList->removeAll(node);
        m_context.m_graphLogic->graphWidget()->scene()->removeItem(node);
    }

    foreach(BrainArc *edge, m_edgeList)
    {
        edge->sourceNode()->removeEdge(edge);
        edge->destNode()->removeEdge(edge);
        m_context.m_graphLogic->graphWidget()->scene()->removeItem(edge);
    }

    m_context.m_graphLogic->setActiveNode(0);
}

AddEdgeCommand::AddEdgeCommand(UndoContext context)
    : BaseUndoClass(context)
{
    setText(QObject::tr("New Arc added"));

    m_edge = new BrainArc(m_context.m_source, m_context.m_destination);
    m_edge->setColor(m_context.m_destination->color());
    m_edge->setWidth(m_context.m_destination->scale()*2 + 1);
}

void AddEdgeCommand::undo()
{
    m_context.m_source->removeEdge(m_edge);
    m_context.m_destination->removeEdge(m_edge);
    m_context.m_graphLogic->graphWidget()->scene()->removeItem(m_edge);

    m_context.m_graphLogic->setActiveNode(m_activeNode);
    m_done = false;
}

void AddEdgeCommand::redo()
{
    m_context.m_source->addEdge(m_edge, true);
    m_context.m_destination->addEdge(m_edge, false);

    m_context.m_graphLogic->graphWidget()->scene()->addItem(m_edge);

    m_context.m_graphLogic->setActiveNode(m_context.m_destination);
    m_done = true;
}

AddEdgeCommand::~AddEdgeCommand()
{
    if (!m_done)
        delete m_edge;
}

RemoveEdgeCommand::RemoveEdgeCommand(UndoContext context)
    : BaseUndoClass(context)
{
    setText(QObject::tr("Arc removed"));
    m_edge = m_context.m_source->edgeTo(m_context.m_destination);
}

void RemoveEdgeCommand::undo()
{
    m_context.m_source->addEdge(m_edge, true);
    m_context.m_destination->addEdge(m_edge, false);

    m_context.m_graphLogic->graphWidget()->scene()->addItem(m_edge);

    m_context.m_graphLogic->setActiveNode(m_activeNode);
}

void RemoveEdgeCommand::redo()
{
    m_context.m_source->removeEdge(m_edge);
    m_context.m_destination->removeEdge(m_edge);
    m_context.m_graphLogic->graphWidget()->scene()->removeItem(m_edge);

    m_context.m_graphLogic->setActiveNode(m_context.m_activeNode);
}

MoveCommand::MoveCommand(UndoContext context)
    : BaseUndoClass(context)
{
    setText(QObject::tr("Move Node"));
}

void MoveCommand::undo()
{
    foreach(BrainNode *node, m_nodeList)
        node->moveBy(-m_context.m_x, -m_context.m_y);

    m_context.m_graphLogic->setActiveNode(m_activeNode);
}

void MoveCommand::redo()
{
    foreach(BrainNode *node, m_nodeList)
        node->moveBy(m_context.m_x, m_context.m_y);

    m_context.m_graphLogic->setActiveNode(m_activeNode);
}

bool MoveCommand::mergeWith(const QUndoCommand *command)
{
    if (command->id() != id())
        return false;

    const MoveCommand *moveCommand = static_cast<const MoveCommand *>(command);

    if (m_context.m_activeNode != moveCommand->m_context.m_activeNode)
        return false;

    m_context.m_x += moveCommand->m_context.m_x;
    m_context.m_y += moveCommand->m_context.m_y;

//    setText(QObject::tr("Node \"").append(
//                m_context.m_activeNode == m_context.m_nodeList->first() ?
//                    QObject::tr("Base node") :
//                    m_context.m_activeNode->toPlainText()).
//            append("\" moved (%1, %2)").arg(m_context.m_x).arg(m_context.m_y).
//            append(m_subtree ? QObject::tr(" with subtree") : QString("")));

    return true;
}

int MoveCommand::id() const
{
    return MoveCommandId;
}

NodeColorCommand::NodeColorCommand(UndoContext context)
    : BaseUndoClass(context)
{
    setText(QObject::tr("Change Node Color"));
    foreach(BrainNode *node, m_nodeList)
        m_colorMap[node] = node->color();
}

void NodeColorCommand::undo()
{
    foreach(BrainNode *node, m_nodeList)
    {
        node->setColor(m_colorMap[node]);
        foreach (BrainArc * edge, node->edgesToThis(false))
            edge->setColor(m_colorMap[node]);
    }

    m_context.m_graphLogic->setActiveNode(m_activeNode);
}

void NodeColorCommand::redo()
{
    foreach(BrainNode *node, m_nodeList)
    {
        node->setColor(m_context.m_color);
        foreach (BrainArc * edge, node->edgesToThis(false))
            edge->setColor(m_context.m_color);
    }

    m_context.m_graphLogic->setActiveNode(m_activeNode);
}

NodeTextColorCommand::NodeTextColorCommand(UndoContext context)
    : BaseUndoClass(context)
{
    setText(QObject::tr("Change Text Color"));
    foreach(BrainNode *node, m_nodeList)
        m_colorMap[node] = node->textColor();
}

void NodeTextColorCommand::undo()
{
    foreach(BrainNode *node, m_nodeList)
        node->setTextColor(m_colorMap[node]);

    m_context.m_graphLogic->setActiveNode(m_activeNode);
}

void NodeTextColorCommand::redo()
{
    foreach(BrainNode *node, m_nodeList)
        node->setTextColor(m_context.m_color);

    m_context.m_graphLogic->setActiveNode(m_activeNode);
}

ScaleNodeCommand::ScaleNodeCommand(UndoContext context)
    : BaseUndoClass(context)
{
    setText(QObject::tr("Scale Node"));
}

void ScaleNodeCommand::undo()
{
    foreach(BrainNode *node, m_nodeList)
        node->setScale(qreal(-m_context.m_scale), m_context.m_graphLogic->graphWidget()->sceneRect());

    m_context.m_graphLogic->setActiveNode(m_activeNode);
}

void ScaleNodeCommand::redo()
{
    foreach(BrainNode *node, m_nodeList)
        node->setScale(m_context.m_scale, m_context.m_graphLogic->graphWidget()->sceneRect());

    m_context.m_graphLogic->setActiveNode(m_activeNode);
}

bool ScaleNodeCommand::mergeWith(const QUndoCommand *command)
{
    if (command->id() != id())
        return false;

    const ScaleNodeCommand *scaleNodeCommand = static_cast<const ScaleNodeCommand *>(command);

    if (m_context.m_activeNode != scaleNodeCommand->m_context.m_activeNode)
        return false;

    m_context.m_scale += scaleNodeCommand->m_context.m_scale;

//    setText(QObject::tr("Node \"").append(
//                m_context.m_activeNode == m_context.m_nodeList->first() ?
//                    QObject::tr("Base node") :
//                    m_context.m_activeNode->toPlainText()).
//            append("\" scaled (%1%)").arg(int((1+m_context.m_scale)*100)).
//            append(m_subtree ? QObject::tr(" with subtree") : QString("")));

    return true;
}

int ScaleNodeCommand::id() const
{
    return ScaleCommandId;
}
