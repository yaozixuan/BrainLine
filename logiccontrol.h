#ifndef LOGICCONTROL_H
#define LOGICCONTROL_H

#include <QObject>
#include <QUndoStack>

#include "brainnode.h"
#include "widgetcontrol.h"

class WidgetControl;

class LogicControl : public QObject
{
    Q_OBJECT

public:

    explicit LogicControl(WidgetControl *parent = 0); // explicit 防止隐式转换
    WidgetControl *graphWidget() const; // Called m_context in Command.cpp
    void setUndoStack(QUndoStack *stack);

    void processKeyEvent(QKeyEvent *event);
    void addFirstNode();
    void removeAllNodes();

    bool readContentFromXmlFile(const QString &fileName);
    void writeContentToXmlFile(const QString &fileName);
    void writeContentToPngFile(const QString &fileName);

    BrainNode *nodeFactory();
    void setActiveNode(BrainNode *node);

    void moveNode(qreal x, qreal y); // undo command

public slots:

    // commands from toolbars:
    void insertNode();      // undo command
    void removeNode();      // undo command
    void nodeEdited();      /// @todo Rewrite as an undo action
    void scaleUp();         // undo command
    void scaleDown();       // undo command
    void nodeColor();       // undo command
    void nodeTextColor();   // undo command
    void addEdge();
    void removeEdge();

    void nodeChanged();
    void nodeSelected();
    void nodeMoved(QGraphicsSceneMouseEvent *event);
    void nodeLostFocus();

signals:

    void contentChanged(const bool& changed = true);
    void notification(const QString &msg);

private:

    void moveNodeUp();
    void moveNodeDown();
    void moveNodeLeft();
    void moveNodeRight();

    void selectNode(BrainNode *node);

    // functions on the edges
    QList<BrainArc *> allEdges() const;
    void addEdge(BrainNode *source, BrainNode *destination);      // undo command
    void removeEdge(BrainNode* source, BrainNode *destination);   // undo command

    WidgetControl *m_widget;

    QList<BrainNode *> m_nodeList;
    BrainNode *m_activeNode;
    bool m_editingNode;
    bool m_edgeAdding;
    bool m_edgeDeleting;
    QUndoStack *m_undoStack;
};

#endif // LOGICCONTROL_H
