#ifndef MOUSEEVENT_H
#define MOUSEEVENT_H

#include <QMainWindow>
#include <QObject>
#include <QWidget>

#include <QPointF>

class Node;

class MouseEvent
{
public:

    enum class Event
    {
        None,
        MoveNode,
        CreateOrConnect,
        Scroll
    };

    MouseEvent();

    void clear();

    Node* sourceNode() const;
    void setSourceNode(Node* node, Event event);

    Event event() const;

    QPointF sourcePos() const;
    void setSourcePos(const QPointF & sourcePos);

    QPointF sourcePosOnNode() const;
    void setSourcePosOnNode(const QPointF & sourcePosOnNode);

private:

    Node* m_sourceNode = nullptr;

    QPointF m_sourcePos;

    QPointF m_sourcePosOnNode;

    Event m_event = Event::None;
};

#endif // MOUSEEVENT_H
