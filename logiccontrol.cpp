#include <QtXml>
#include <QColorDialog>
#include <QApplication>
#include <QMessageBox>
#include <QScrollBar>

#include "command.h"
#include "logiccontrol.h"

LogicControl::LogicControl(WidgetControl *parent)
    : QObject(parent)
    , m_widget(parent)
    , m_activeNode(0)
    , m_editingNode(false)
    , m_edgeAdding(false)
    , m_edgeDeleting(false)
{}

WidgetControl *LogicControl::graphWidget() const
{
    return m_widget;
}

void LogicControl::setUndoStack(QUndoStack *stack)
{
    m_undoStack = stack;
}

void LogicControl::addFirstNode()
{
    BrainNode *node = nodeFactory();
    m_widget->scene()->addItem(node);
    m_nodeList.append(node);
    node->setHtml(QString("New Node")); // First Node

    m_activeNode = m_nodeList.first();
    m_activeNode->setBorder();
}

void LogicControl::removeAllNodes()
{
    foreach(BrainNode *node, m_nodeList)
        delete node;

    m_nodeList.clear();
    m_activeNode = 0;
//    m_hintNode = 0;
}

bool LogicControl::readContentFromXmlFile(const QString &fileName)
{
    // open & parse XML file
    QDomDocument doc("QtMindMap");
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly))
    {
        emit notification(tr("Couldn't read file."));
        return false;
    }

    if (!doc.setContent(&file))
    {
        emit notification(tr("Couldn't parse XML file."));
        file.close();
        return false;
    }
    file.close();

    QDomElement docElem = doc.documentElement();

    // add nodes
    QDomNodeList nodes = docElem.childNodes().item(0).childNodes();
    for (unsigned int i = 0; i < nodes.length(); i++)
    {
        QDomElement e = nodes.item(i).toElement();
        if(!e.isNull())
        {
            BrainNode *node = nodeFactory();
            m_widget->scene()->addItem(node);
            m_nodeList.append(node);
            node->setHtml(e.attribute("htmlContent"));
            node->setPos(e.attribute("x").toFloat(),
                         e.attribute("y").toFloat());
            node->setScale(e.attribute("scale").toFloat(),
                           m_widget->sceneRect());
            node->setColor(QColor(e.attribute("bg_red").toFloat(),
                                  e.attribute("bg_green").toFloat(),
                                  e.attribute("bg_blue").toFloat()));
            node->setTextColor(QColor(e.attribute("text_red").toFloat(),
                                      e.attribute("text_green").toFloat(),
                                      e.attribute("text_blue").toFloat()));
        }
    }

    // add edges
    QDomNodeList edges = docElem.childNodes().item(1).childNodes();
    for (unsigned int i = 0; i < edges.length(); i++)
    {
        QDomElement e = edges.item(i).toElement();
        if(!e.isNull())
        {
            BrainNode *source = m_nodeList[e.attribute("source").toInt()];
            BrainNode *destination = m_nodeList[e.attribute("destination").toInt()];

            BrainArc *edge = new BrainArc(source, destination);
            source->addEdge(edge, true);
            destination->addEdge(edge, false);

            edge->setColor(QColor(e.attribute("red").toFloat(),
                                  e.attribute("green").toFloat(),
                                  e.attribute("blue").toFloat()));
            edge->setWidth(e.attribute("width").toFloat());
//            edge->setSecondary(e.attribute("secondary").toInt() );

            m_widget->scene()->addItem(edge);
        }
    }

    // test the first node the active one
    m_activeNode = m_nodeList.first();
    m_activeNode->setBorder();
    m_activeNode->setFocus();

    m_widget->show();
    return true;
}

void LogicControl::writeContentToXmlFile(const QString &fileName)
{
    // create XML doc object
    QDomDocument doc("QtMindMap");

    QDomElement root = doc.createElement("qtmindmap");
    doc.appendChild( root );

    // nodes
    QDomElement nodes_root = doc.createElement("nodes");
    root.appendChild(nodes_root);
    foreach(BrainNode *node, m_nodeList)
    {
        QDomElement cn = doc.createElement("node");

        // no need to store ID: parsing order is preorder.
        // cn.setAttribute( "id", QString::number(m_nodeList.indexOf(node)));
        cn.setAttribute( "x", QString::number(node->pos().x()));
        cn.setAttribute( "y", QString::number(node->pos().y()));
        cn.setAttribute( "htmlContent", node->toHtml());
        cn.setAttribute( "scale", QString::number(node->scale()));
        cn.setAttribute( "bg_red", QString::number(node->color().red()));
        cn.setAttribute( "bg_green", QString::number(node->color().green()));
        cn.setAttribute( "bg_blue", QString::number(node->color().blue()));
        cn.setAttribute( "text_red", QString::number(node->textColor().red()));
        cn.setAttribute( "text_green",
                        QString::number(node->textColor().green()));
        cn.setAttribute( "text_blue",
                        QString::number(node->textColor().blue()));
        nodes_root.appendChild(cn);
    }

    //edges
    QDomElement edges_root = doc.createElement("edges");
    root.appendChild(edges_root);
    foreach(BrainArc *edge, allEdges())
    {
        QDomElement cn = doc.createElement("edge");
        cn.setAttribute( "source",
                      QString::number(m_nodeList.indexOf(edge->sourceNode())));
        cn.setAttribute( "destination",
                      QString::number(m_nodeList.indexOf(edge->destNode())));
        cn.setAttribute( "red", QString::number(edge->color().red()));
        cn.setAttribute( "green", QString::number(edge->color().green()));
        cn.setAttribute( "blue", QString::number(edge->color().blue()));
        cn.setAttribute( "width", QString::number(edge->width()));

        edges_root.appendChild(cn);
    }

    // write XML doc object to file
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly))
    {
        emit notification(tr("Couldn't open file to write."));
        return;
    }
    QTextStream ts( &file );
    ts << doc.toString();
    file.close();

    // show a statusBar message to the user
    emit notification(tr("Saved."));
}

void LogicControl::writeContentToPngFile(const QString &fileName)
{
    QImage img(m_widget->scene()->sceneRect().width(),
               m_widget->scene()->sceneRect().height(),
               QImage::Format_ARGB32_Premultiplied);
    QPainter painter(&img);

    painter.setRenderHint(QPainter::Antialiasing);

    // Strange that I have to set this, and scene->render() does not do this
    m_widget->scene()->setBackgroundBrush(WidgetControl::m_paperColor);

    m_widget->scene()->render(&painter);
    painter.setBackground(WidgetControl::m_paperColor);
    painter.end();

    img.save(fileName);

    // show a statusBar message to the user
    emit notification(tr("MindMap exported as ") + fileName);
    QMessageBox::information(NULL, tr("Path"), tr("MindMap exported as ") + fileName);
}

BrainNode * LogicControl::nodeFactory()
{
    BrainNode *node = new BrainNode(this);

    connect(node, SIGNAL(nodeChanged()), this, SLOT(nodeChanged()));
    connect(node, SIGNAL(nodeSelected()), this, SLOT(nodeSelected()));
    connect(node, SIGNAL(nodeEdited()), this, SLOT(nodeEdited()));
    connect(node, SIGNAL(nodeMoved(QGraphicsSceneMouseEvent*)),
            this, SLOT(nodeMoved(QGraphicsSceneMouseEvent*)));
    connect(node, SIGNAL(nodeLostFocus()), this, SLOT(nodeLostFocus()));

    return node;
}

void LogicControl::setActiveNode(BrainNode *node)
{
    if (m_activeNode!=0)
        m_activeNode->setBorder(false);

    m_activeNode = node;
    if (m_activeNode)
        m_activeNode->setBorder();
}

void LogicControl::insertNode()
{
    // checks
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
                 BrainNode::newNodeCenter);

    QRectF rect (m_widget->scene()->sceneRect().topLeft(),
                 m_widget->scene()->sceneRect().bottomRight()
                 - BrainNode::newNodeBottomRigth);

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

void LogicControl::removeNode()
{
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
//    context.m_hintNode = m_hintNode;

    QUndoCommand *insertNodeCommand = new RemoveNodeCommand(context);
    m_undoStack->push(insertNodeCommand);
}

void LogicControl::nodeEdited()
{
    if (!m_activeNode)
    {
        emit notification(tr("No active node."));
        return;
    }

    m_editingNode = true;
    m_activeNode->setEditable();
    m_widget->scene()->setFocusItem(m_activeNode);
}

void LogicControl::scaleUp()
{
    if (!m_activeNode)
    {
        emit notification(tr("No active node."));
        return;
    }

    if (m_activeNode->scale()+qreal(0.2) > qreal(4))
    {
        emit notification(tr("Too much scaling."));
        return;
    }

    UndoContext context;
    context.m_graphLogic = this;
    context.m_nodeList = &m_nodeList;
    context.m_activeNode = m_activeNode;
    context.m_scale = qreal(0.2);

    QUndoCommand *scaleNodeCommand = new ScaleNodeCommand(context);
    m_undoStack->push(scaleNodeCommand);
}

void LogicControl::scaleDown()
{
    if (!m_activeNode)
    {
        emit notification(tr("No active node."));
        return;
    }

    if (m_activeNode->scale()-qreal(0.2) < qreal(0.1))
    {
        emit notification(tr("Too much scaling."));
        return;
    }

    UndoContext context;
    context.m_graphLogic = this;
    context.m_nodeList = &m_nodeList;
    context.m_activeNode = m_activeNode;
    context.m_scale = qreal(-0.2);

    QUndoCommand *scaleNodeCommand = new ScaleNodeCommand(context);
    m_undoStack->push(scaleNodeCommand);
}

void LogicControl::nodeColor()
{
    if (!m_activeNode)
    {
        emit notification(tr("No active node."));
        return;
    }

    // popup a color selector dialogm def color is the curr one.
    QColorDialog dialog(m_widget);
    dialog.setWindowTitle(tr("Select node color"));
    dialog.setCurrentColor(m_activeNode->color());
    if (!dialog.exec())
        return;

    UndoContext context;
    context.m_graphLogic = this;
    context.m_nodeList = &m_nodeList;
    context.m_activeNode = m_activeNode;
    context.m_color = dialog.selectedColor();

    QUndoCommand *nodeColorCommand = new NodeColorCommand(context);
    m_undoStack->push(nodeColorCommand);
}

void LogicControl::nodeTextColor()
{
    if (!m_activeNode)
    {
        emit notification(tr("No active node."));
        return;
    }

    // popup a color selector dialogm def color is the curr one.
    QColorDialog dialog(m_widget);
    dialog.setWindowTitle(tr("Select text color"));
    dialog.setCurrentColor(m_activeNode->textColor());
    if (!dialog.exec())
        return;

    UndoContext context;
    context.m_graphLogic = this;
    context.m_nodeList = &m_nodeList;
    context.m_activeNode = m_activeNode;
    context.m_color = dialog.selectedColor();

    QUndoCommand *nodeTextColorCommand = new NodeTextColorCommand(context);
    m_undoStack->push(nodeTextColorCommand);
}

void LogicControl::addEdge()
{
    emit notification(tr("Add edge: select destination node."));
    m_edgeAdding = true;
}

void LogicControl::removeEdge()
{
    emit notification(tr("Delete edge: select other end-node."));
    m_edgeDeleting = true;
}

void LogicControl::nodeChanged()
{
    emit contentChanged();
}

void LogicControl::nodeSelected()
{
    // if node == 0 then nodeSelected invoked after a signal from a Node
    selectNode(dynamic_cast<BrainNode*>(QObject::sender()));
}

void LogicControl::nodeMoved(QGraphicsSceneMouseEvent *event)
{
    // move just the active Node, or it's subtree too?
    QList <BrainNode *> nodeList;
    nodeList.push_back(m_activeNode);

    foreach(BrainNode *node, nodeList)
        node->setPos(node->pos() + event->scenePos() - event->lastScenePos());
}

void LogicControl::nodeLostFocus()
{
    if (m_editingNode)
    {
        m_editingNode = false;
        if (m_activeNode)
        {
            m_activeNode->setEditable(false);
            m_activeNode->update();
        }
        return;
    }

    if (m_edgeAdding)
    {
        m_edgeAdding = false;
        emit notification(tr("Edge adding cancelled."));
        return;
    }

    if (m_edgeDeleting)
    {
        m_edgeDeleting = false;
        emit notification(tr("Edge deleting cancelled."));
        return;
    }
}

void LogicControl::moveNodeUp()
{
    QApplication::keyboardModifiers() & Qt::ControlModifier ?
        moveNode(qreal(0),qreal(-20)) :
        m_widget->verticalScrollBar()->setValue(
                    m_widget->verticalScrollBar()->value()-20);
}

void LogicControl::moveNodeDown()
{
    QApplication::keyboardModifiers() & Qt::ControlModifier ?
        moveNode(qreal(0),qreal(20)) :
                m_widget->verticalScrollBar()->setValue(
                            m_widget->verticalScrollBar()->value()+20);
}

void LogicControl::moveNodeLeft()
{
    QApplication::keyboardModifiers() & Qt::ControlModifier ?
        moveNode(qreal(-20),qreal(0)) :
                m_widget->horizontalScrollBar()->setValue(
                            m_widget->horizontalScrollBar()->value()-20);
}

void LogicControl::moveNodeRight()
{
    QApplication::keyboardModifiers() & Qt::ControlModifier ?
        moveNode(qreal(20),qreal(0)) :
                m_widget->horizontalScrollBar()->setValue(
                            m_widget->horizontalScrollBar()->value()+20);
}

void LogicControl::moveNode(qreal x, qreal y)
{
    if (!m_activeNode)
    {
        emit notification(tr("No active node."));
        return;
    }

    UndoContext context;
    context.m_graphLogic = this;
    context.m_nodeList = &m_nodeList;
    context.m_activeNode = m_activeNode;
    context.m_x = x;
    context.m_y = y;

    QUndoCommand *moveCommand = new MoveCommand(context);
    m_undoStack->push(moveCommand);
}

void LogicControl::selectNode(BrainNode *node)
{
    if (m_edgeAdding)
    {
        addEdge(m_activeNode, node);
        m_edgeAdding = false;
    }
    else if (m_edgeDeleting)
    {
        removeEdge(m_activeNode, node);
        m_edgeDeleting = false;
    }
    else
    {
        setActiveNode(node);
    }
}

QList<BrainArc *> LogicControl::allEdges() const
{
    QList<BrainArc *> list;

    // GraphWidget has a list of Nodes only.
    // Each Node maintains a list of it's own Edges.
    // We iterate through the list of Nodes and call Node::edgesFrom() on them.
    // edgesFrom(exludeSecundaries=false) return a list of edges (including
    // secondary edges) which starts from this Node.
    foreach(BrainNode * node, m_nodeList)
        list.append(node->edgesFrom(false));

    return list;
}


void LogicControl::addEdge(BrainNode *source, BrainNode *destination)
{

    if (source->isConnected(destination))
    {
        emit notification(tr("There is already an Arc."));
        return;
    }

    UndoContext context;
    context.m_graphLogic = this;
    context.m_nodeList = &m_nodeList;
    context.m_activeNode = m_activeNode;
    context.m_source = source;
    context.m_destination = destination;

    QUndoCommand *addEdgeCommand = new AddEdgeCommand(context);
    m_undoStack->push(addEdgeCommand);
}

void LogicControl::removeEdge(BrainNode *source, BrainNode *destination)
{
    if (!source->isConnected(destination))
    {
        emit notification(tr("There is no Arc."));
        return;
    }

    UndoContext context;
    context.m_graphLogic = this;
    context.m_nodeList = &m_nodeList;
    context.m_activeNode = m_activeNode;
    context.m_source = source;
    context.m_destination = destination;

    QUndoCommand *removeEdgeCommand = new RemoveEdgeCommand(context);
    m_undoStack->push(removeEdgeCommand);

}
