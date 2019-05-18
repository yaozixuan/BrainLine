#ifndef COMMAND_H
#define COMMAND_H

#include <QUndoCommand>
#include <exception>

#include "logiccontrol.h"

class LogicControl;


struct UndoContext
{
    LogicControl *m_graphLogic;
    BrainNode *m_activeNode;
    BrainNode *m_hintNode;
    QList <BrainNode *> *m_nodeList;
    QPointF m_pos;
    QColor m_color;
    BrainNode *m_source;
    BrainNode *m_destination;
    bool m_secondary;
    qreal m_x;
    qreal m_y;
    bool m_subtree;
    qreal m_scale;

    UndoContext(LogicControl *graphLogic = 0,
                BrainNode *activeNode = 0,
                BrainNode *hintNode = 0,
                QList <BrainNode *> *nodeList = 0,
                QPointF pos = QPointF(),
                QColor color = QColor(),
                BrainNode *source = 0,
                BrainNode *destination = 0,
                bool secondary = false,
                qreal x = 0,
                qreal y = 0,
                bool subtree = false,
                qreal scale = 0)
        : m_graphLogic(graphLogic)
        , m_activeNode(activeNode)
        , m_hintNode(hintNode)
        , m_nodeList(nodeList)
        , m_pos(pos)
        , m_color(color)
        , m_source(source)
        , m_destination(destination)
        , m_secondary(secondary)
        , m_x(x)
        , m_y(y)
        , m_subtree(subtree)
        , m_scale(scale)
    {}
};

class BaseUndoClass : public QUndoCommand
{
public:

    enum MergeableCommandId
    {
        MoveCommandId = 0,
        ScaleCommandId
    };

    BaseUndoClass(UndoContext context);

protected:

    bool m_done;
    UndoContext m_context;
    BrainNode *m_activeNode;
    QList <BrainNode *> m_nodeList;
    bool m_subtree;
};


class InsertNodeCommand : public BaseUndoClass
{

public:

    InsertNodeCommand(UndoContext context);
    ~InsertNodeCommand();

    void undo();
    void redo();

private:

    BrainNode *m_node;
    BrainArc *m_edge;
};

class RemoveNodeCommand : public BaseUndoClass
{

public:

    RemoveNodeCommand(UndoContext context);

    void undo();
    void redo();

private:

    BrainNode *m_hintNode;
    QList <BrainArc *> m_edgeList;
};

class AddEdgeCommand : public BaseUndoClass
{

public:

    AddEdgeCommand(UndoContext context);
    ~AddEdgeCommand();

    void undo();
    void redo();

private:

    BrainArc *m_edge;
};

class RemoveEdgeCommand : public BaseUndoClass
{

public:

    RemoveEdgeCommand(UndoContext context);

    void undo();
    void redo();

private:

    BrainArc *m_edge;
};

class MoveCommand : public BaseUndoClass
{

public:

    MoveCommand(UndoContext context);

    void undo();
    void redo();

    bool mergeWith(const QUndoCommand *command);
    int id() const;
};

class NodeColorCommand : public BaseUndoClass
{

public:

    NodeColorCommand(UndoContext context);

    void undo();
    void redo();

private:

    QMap<BrainNode*, QColor> m_colorMap;
};

class NodeTextColorCommand : public BaseUndoClass
{

public:

    NodeTextColorCommand(UndoContext context);

    void undo();
    void redo();

private:

    QMap<BrainNode*, QColor> m_colorMap;
};

class ScaleNodeCommand : public BaseUndoClass
{

public:

    ScaleNodeCommand(UndoContext context);

    void undo();
    void redo();

    bool mergeWith(const QUndoCommand *command);
    int id() const;
};

#endif // COMMAND_H
