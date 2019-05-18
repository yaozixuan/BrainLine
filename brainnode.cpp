#include "brainnode.h"
#include "logiccontrol.h"

#include <QPainter>
#include <QStyleOption>
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QTextDocument>

const QPointF BrainNode::newNodeCenter = QPointF(4, 11.5);
const QPointF BrainNode::newNodeBottomRigth = QPointF(8, 23);

const double BrainNode::m_pi = 3.14159265358979323846264338327950288419717;
const double BrainNode::m_oneAndHalfPi = BrainNode::m_pi * 3.5;
const double BrainNode::m_twoPi = BrainNode::m_pi * 2.0;

const QColor BrainNode::m_gold(255,215,0);

BrainNode::BrainNode(LogicControl *graphLogic)
    : m_graphLogic(graphLogic)
    , m_number(-1)
    , m_hasBorder(false)
    , m_numberIsSpecial(false)
    , m_color(m_gold)
    , m_textColor(0,0,0)
    , m_effect(new QGraphicsDropShadowEffect(this))
{
    setFlag(ItemIsMovable);
    setFlag(ItemSendsGeometryChanges);
    setCacheMode(DeviceCoordinateCache);
    setDefaultTextColor(QColor(0,0,0));
    setZValue(2);
    setGraphicsEffect(m_effect);
    m_effect->setEnabled(false);
    m_effect->setOffset(qreal(4.0));
}

BrainNode::~BrainNode()
{
    deleteEdges();
}

void BrainNode::addEdge(BrainArc *edge, bool startsFromThisNode)
{
    m_edgeList.push_back(EdgeElement(edge, startsFromThisNode));
    edge->adjust();
}

void BrainNode::deleteEdge(BrainNode *otherEnd)
{
    for(QList<EdgeElement>::iterator it = m_edgeList.begin();
        it != m_edgeList.end(); it++)
    {
        if ((it->edge->sourceNode() == otherEnd /*&&
             it->edge->destNode() == this*/)
         || (it->edge->sourceNode() == this/* &&
             it->edge->destNode() == otherEnd*/))
        {
            delete it->edge;
            return;
        }
    }
}

void BrainNode::deleteEdges()
{
//    foreach (EdgeElement element, m_edgeList)
//        delete element.edge;
    for (QList<EdgeElement>::Iterator it = m_edgeList.begin();
         it != m_edgeList.end(); it++)
    {
        delete it->edge;
    }
}

void BrainNode::removeEdge(BrainArc *edge)
{
    for(QList<EdgeElement>::iterator it = m_edgeList.begin();
        it != m_edgeList.end(); it++)
    {
        if (it->edge == edge)
        {
            m_edgeList.erase(it);
            return;
        }
    }
}

//void Node::removeEdges()
//{
//    for(QList<EdgeElement>::iterator it = m_edgeList.begin();
//        it != m_edgeList.end(); it++)
//    {
//        m_edgeList.erase(it);
//    }
//}

QList<BrainArc *> BrainNode::edges() const
{
    QList<BrainArc *> list;

//    foreach(EdgeElement element, m_edgeList)
//        list.push_back(element.edge);
    for (QList<EdgeElement>::ConstIterator it = m_edgeList.begin();
         it != m_edgeList.end(); it++)
    {
        list.append(it->edge);
    }

    return list;
}

// edges from this Node. Exclude secondaries if needed (calc subtree)
QList<BrainArc *> BrainNode::edgesFrom(const bool &excludeSecondaries) const
{
    QList<BrainArc *> list;

    foreach(EdgeElement element, m_edgeList)
        if (element.startsFromThisNode && !excludeSecondaries)
            list.push_back(element.edge);

    return list;
}

// edges to this node (max 1 primary and any number of secondaries)
QList<BrainArc *> BrainNode::edgesToThis(const bool &excludeSecondaries) const
{
    QList<BrainArc *> list;

    foreach(EdgeElement element, m_edgeList)
        if (!element.startsFromThisNode && !excludeSecondaries)
            list.push_back(element.edge);

    return list;
}

// the edge from this Node to the parameter Node
BrainArc * BrainNode::edgeTo(const BrainNode *node) const
{
    foreach(EdgeElement element, m_edgeList)
        if ((element.edge->sourceNode() == node  ||
             element.edge->destNode() == node))
            return element.edge;

    return 0;
}

// return thue if this and the parameter Node is connected with an edge
bool BrainNode::isConnected(const BrainNode *node) const
{
    foreach (EdgeElement element, m_edgeList)
        if (element.edge->sourceNode() == node ||
                element.edge->destNode() == node)
            return true;

    return false;
}

void BrainNode::setBorder(const bool &hasBorder)
{
   m_hasBorder = hasBorder;
   m_effect->setEnabled(hasBorder);

   update();
}

void BrainNode::setEditable(const bool &editable)
{
    if (!editable)
    {
        setTextInteractionFlags(Qt::NoTextInteraction);
        return;
    }

    setTextInteractionFlags(Qt::TextEditable);

    // set cursor to the end
    QTextCursor c = textCursor();
    c.setPosition(c.document()->toPlainText().length());
    setTextCursor(c);
}

void BrainNode::setColor(const QColor &color)
{
    m_color = color;
    update();
}

QColor BrainNode::color() const
{
    return m_color;
}

void BrainNode::setTextColor(const QColor &color)
{
    m_textColor = color;
    update();
}

QColor BrainNode::textColor() const
{
    return m_textColor;
}

void BrainNode::setScale(const qreal &factor,const QRectF &sceneRect)
{
    // cannot scale out the Node from the scene
    if (!sceneRect.contains(pos() +
                            boundingRect().bottomRight() * scale() * factor))
        return;

    prepareGeometryChange();
    QGraphicsTextItem::setScale(factor + scale());

    // scale edges to this Node too
    foreach(EdgeElement element, m_edgeList)
    {
        if (!element.startsFromThisNode)
            element.edge->setWidth(element.edge->width() + factor );

        element.edge->adjust();
    }
}

void BrainNode::showNumber(const int &number,
                      const bool& show,
                      const bool &numberIsSpecial)
{
    m_number = show ? number : -1;
    m_numberIsSpecial = numberIsSpecial;
    update();
}

void BrainNode::insertPicture(const QString &picture)
{
    QTextCursor c = textCursor();

    // strange, picture looks bad when node is scaled up
    c.insertHtml(QString("<img src=").append(picture).
                 append(" width=15 height=15></img>"));

    foreach (EdgeElement element, m_edgeList) element.edge->adjust();
    emit nodeChanged();
}

QPointF BrainNode::intersection(const QLineF &line, const bool &reverse) const
{

    /// @note What a pity, the following does not work,
    /// doing it with brute (unaccurate) force

    //    QPainterPath nodeShape(shape());
    //    nodeShape.translate(pos());

    //    QPainterPath l;
    //    l.moveTo(line.p1());
    //    l.lineTo(line.p2());

    //    return nodeShape.intersected(l);


    QPainterPath path;
    path.addRoundedRect(sceneBoundingRect(), 28.0, 28.0);

    if (reverse)
    {
        for (qreal t = 1; t!=0; t-=0.01)
            if (!path.contains(line.pointAt(t)))
                return line.pointAt(t);
    }
    else
    {
        for (qreal t = 0; t!=1; t+=0.01)
            if (!path.contains(line.pointAt(t)))
                return line.pointAt(t);
    }

    return QPointF(0,0);
}

double BrainNode::calculateBiggestAngle() const
{
    // in no edge, return with 12 o'clock
    if (m_edgeList.empty())
        return BrainNode::m_oneAndHalfPi;

    // if there is only one edge, return with it's extension
    if (m_edgeList.size()==1)
        return m_edgeList.first().startsFromThisNode ?
                    BrainNode::m_pi - m_edgeList.first().edge->angle() :
                    BrainNode::m_twoPi - m_edgeList.first().edge->angle();

    // put angles of every edges from this node to a list
    QList<double> tmp;
    for(QList<EdgeElement>::const_iterator it = m_edgeList.begin();
        it != m_edgeList.end(); it++)
    {
        tmp.push_back(it->startsFromThisNode ?
                 it->edge->angle() :
                 doubleModulo(BrainNode::m_pi + it->edge->angle(), BrainNode::m_twoPi));
    }
    qSort(tmp.begin(), tmp.end());

    // find the biggest diffrence, store prev angle
    double prev(tmp.first());
    double max_prev(tmp.last());
    double max(BrainNode::m_twoPi - tmp.last() + tmp.first());

    for(QList<double>::const_iterator it = ++tmp.begin(); it!=tmp.end(); it++)
    {
        if (*it - prev > max )
        {
            max = *it - prev;
            max_prev = prev;
        }
        prev = *it;
    }

    // return with prev angle + max diff / 2
    return BrainNode::m_twoPi - doubleModulo(max_prev + max / 2, BrainNode::m_twoPi);
}

void BrainNode::keyPressEvent(QKeyEvent *event)
{
    // cursor movements
    switch (event->key()) {

    case Qt::Key_Left:
    {
        QTextCursor c = textCursor(); // textcursor() return just a copy
        c.movePosition(
                    event->modifiers() == Qt::ControlModifier ?
                    QTextCursor::PreviousWord :
                    QTextCursor::PreviousCharacter);
        setTextCursor(c);
        break;
    }
    case Qt::Key_Right:
    {
        QTextCursor c = textCursor();
        c.movePosition(
                    event->modifiers() == Qt::ControlModifier ?
                    QTextCursor::NextWord :
                    QTextCursor::NextCharacter);
        setTextCursor(c);
        break;
    }
    case Qt::Key_Up:
    {
        QTextCursor c = textCursor();
        c.movePosition(QTextCursor::Up);
        setTextCursor(c);
        break;
    }
    case Qt::Key_Down:
    {
        QTextCursor c = textCursor();
        c.movePosition(QTextCursor::Down);
        setTextCursor(c);
        break;
    }

    default:

        // not cursor movement: editing
        QGraphicsTextItem::keyPressEvent(event);

        foreach (EdgeElement element, m_edgeList) element.edge->adjust();
        emit nodeChanged();
    }

    ///@note leaving editing mode is done with esc, handled by graphwidget
}

void BrainNode::paint(QPainter *painter,
                 const QStyleOptionGraphicsItem *option,
                 QWidget *w)
{
    // draw background in hint mode. num == -1 : not in hint mode
    // if m_numberIsSpecial (can be selected with enter) bg is green, not yellow
    if (m_number != -1)
    {
        painter->setPen(Qt::transparent);
        painter->setBrush(m_numberIsSpecial ? Qt::green : Qt::yellow);
        painter->drawRoundedRect(boundingRect(), 20.0, 15.0);
    }
    else
    {
        m_hasBorder ?
            painter->setPen(QPen(QBrush(Qt::black), 1)) : // border is scaled
            painter->setPen(Qt::transparent);

        painter->setBrush(m_color);
        painter->drawRoundedRect(boundingRect(), 20.0, 15.0);

    }
    painter->setBrush(Qt::NoBrush);

    // the text itself
    setDefaultTextColor(m_textColor);
    QGraphicsTextItem::paint(painter, option, w);


    // print num to topleft corner in hint mode.
    if (m_number != -1)
    {
        painter->setPen(Qt::white);
        painter->setBackground(Qt::red);
        painter->setBackgroundMode(Qt::OpaqueMode);
        painter->drawText(boundingRect().topLeft()+QPointF(0,11),
                          QString("%1").arg(m_number));
    }
}

QVariant BrainNode::itemChange(GraphicsItemChange change, const QVariant &value)
{
    switch (change) {

    case ItemPositionChange:
    {
        // Node is about to move, check borders
        QPointF newPos = value.toPointF();

        // the fence is reduced with the size of the node
        QRectF rect (scene()->sceneRect().topLeft(),
                     scene()->sceneRect().bottomRight() -
                     boundingRect().bottomRight() * scale() );

        if (!rect.contains(newPos))
        {
            // Keep the item inside the scene rect.
            newPos.setX(qMin(rect.right(), qMax(newPos.x(), rect.left())));
            newPos.setY(qMin(rect.bottom(), qMax(newPos.y(), rect.top())));
            return newPos;
        }

         /// undo call here

        break;
    }
    case ItemPositionHasChanged:

        // Notify parent, adjust edges that a move has happended.
        foreach (EdgeElement element, m_edgeList) element.edge->adjust();
        emit nodeChanged();
        break;

    default:
        break;
    };

    return QGraphicsItem::itemChange(change, value);
}

void BrainNode::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        emit nodeSelected();
    }

    QGraphicsItem::mousePressEvent(event);
}

void BrainNode::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);

    emit nodeEdited();
}

void BrainNode::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseReleaseEvent(event);
}

// notify parent so subtree can be moved too if necessary
void BrainNode::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QPointF diff(event->scenePos() - event->lastScenePos());
    m_graphLogic->moveNode(diff.x(), diff.y());
}

QPainterPath BrainNode::shape () const
{
    QPainterPath path;
    path.addRoundedRect(boundingRect(), 20.0, 15.0);
    return path;
}

// leave editing mode when user clicks on the view elsewhere for example
void BrainNode::focusOutEvent(QFocusEvent *event)
{
    Q_UNUSED(event);
    setEditable(false);

    emit nodeLostFocus();
}

// there is no such thing as modulo operator for double :P
double BrainNode::doubleModulo(const double &devided, const double &devisor) const
{
    return devided - static_cast<double>(devisor * static_cast<int>(devided
                                                                  / devisor));
}
