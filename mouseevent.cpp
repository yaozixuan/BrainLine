#include "mouseevent.h"
#include "brainnode.h"


MouseEvent::MouseEvent()
{
}

MouseEvent::Event MouseEvent::event() const
{
    return m_event;
}

void MouseEvent::clear()
{
    m_sourceNode = nullptr;
    m_sourcePos = QPointF();
    m_sourcePosOnNode = QPointF();
    m_event = Event::None;
}

void MouseEvent::setSourceNode(Node * node, MouseEvent::Event event)
{
    m_sourceNode = node;
    m_event = event;
}

Node * MouseEvent::sourceNode() const
{
    return m_sourceNode;
}

void MouseEvent::setSourcePos(const QPointF & sourcePos)
{
    m_sourcePos = sourcePos;
}

void MouseEvent::setSourcePosOnNode(const QPointF & sourcePos)
{
    m_sourcePosOnNode = sourcePos;
}

QPointF MouseEvent::sourcePos() const
{
    return m_sourcePos;
}

QPointF MouseEvent::sourcePosOnNode() const
{
    return m_sourcePosOnNode;
}
