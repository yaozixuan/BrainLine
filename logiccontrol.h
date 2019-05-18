#ifndef LOGICCONTROL_H
#define LOGICCONTROL_H

#include <QObject>
#include <QUndoStack>

#include "brainnode.h"
#include "widgetcontrol.h"
//#include "commands.h"


class WidgetControl;

class InsertNodeCommand;
class RemoveNodeCommand;
class AddEdgeCommand;
class RemoveEdgeCommand;

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

    Node *nodeFactory();
    void setActiveNode(Node *node);
//    void setHintNode(Node *node);
//    void reShowNumbers();

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
//    void hintMode();
//    void insertPicture(const QString &picture); /// @todo Rewrite as an undo action

    void nodeChanged();
    void nodeSelected();
    void nodeMoved(QGraphicsSceneMouseEvent *event);
    void nodeLostFocus();

signals:

    void contentChanged(const bool& changed = true);
    void notification(const QString &msg);

//protected:
//    void mousePressEvent(QMouseEvent *event);

private:

    void moveNodeUp();
    void moveNodeDown();
    void moveNodeLeft();
    void moveNodeRight();

    // hint mode
//    void appendNumber(const int &unm);
//    void delNumber();
//    void applyNumber();

    void selectNode(Node *node);

    // functions on the edges
    QList<Edge *> allEdges() const;
    void addEdge(Node *source, Node *destination);      // undo command
    void removeEdge(Node* source, Node *destination);   // undo command

    // hint mode's nodenumber handling functions
//    void showNodeNumbers();
//    void showingAllNodeNumbers(const bool &show = true);
//    void showingNodeNumbersBeginWithNumber(const int &prefix,
//                                           const bool &show = true);

    WidgetControl *m_graphWidget;

    QList<Node *> m_nodeList;
    Node *m_activeNode;
//    bool m_showingNodeNumbers;
//    QString m_hintNumber;
//    Node *m_hintNode;
    bool m_editingNode;
    bool m_edgeAdding;
    bool m_edgeDeleting;

    std::map<int, void(LogicControl::*)(void)> m_memberMap;
    QUndoStack *m_undoStack;
};

#endif // LOGICCONTROL_H
