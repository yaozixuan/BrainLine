#ifndef BRAINNODE
#define BRAINNODE

#include <QGraphicsTextItem>
#include <QTextCursor>
#include<QGraphicsDropShadowEffect>

#include "brainarc.h"
class LogicControl;

class BrainNode : public QGraphicsTextItem
{
    Q_OBJECT

public:

    BrainNode(LogicControl *graphLogic);
    ~BrainNode();

    // add/remove edges
    void addEdge(BrainArc *edge, bool startsFromThisNode);
    void deleteEdge(BrainNode *otherEnd);
    void deleteEdges();
    void removeEdge(BrainArc *edge);
    void removeEdges();


    // graph traversal
    QList<BrainArc *> edges() const;
    QList<BrainArc *> edgesFrom(const bool &excludeSecondaries = true) const;
    QList<BrainArc *> edgesToThis(const bool &excludeSecondaries = true) const;
    BrainArc * edgeTo(const BrainNode* node) const;
    QList<BrainNode *> subtree() const;
    bool isConnected(const BrainNode *node) const;

    // prop set/get
    void setBorder(const bool &hasBorder = true);
    void setEditable(const bool &editable = true);
    void setColor(const QColor &color);
    QColor color() const;
    void setTextColor(const QColor &color);
    QColor textColor() const;
    void setScale(const qreal &factor, const QRectF &sceneRect);

    // show numbers in hint mode
    void showNumber(const int &number, const bool& show = true,
                    const bool &numberIsSpecial = false);
    // insert picture to the cursor's current position
    void insertPicture(const QString &picture);

    // changing visibility from prot to pub
    // so GraphWidget::keyPressEvent can call it edit during editing
    void keyPressEvent(QKeyEvent *event);

    // calculetes the intersection of line and shape of this Node
    QPointF intersection(const QLineF &line, const bool &reverse = false) const;

    // returns with the biggest angle between the edges
    double calculateBiggestAngle() const;

    static const QPointF newNodeCenter;
    static const QPointF newNodeBottomRigth;

signals:

    void nodeChanged();
    void nodeSelected();
    void nodeEdited();
    void nodeMoved(QGraphicsSceneMouseEvent *event);
    void nodeLostFocus();

protected:

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    QPainterPath shape () const;
    void focusOutEvent(QFocusEvent *event);

private:

    double doubleModulo(const double &devided, const double &devisor) const;

    struct EdgeElement
    {
        BrainArc *edge;
        bool startsFromThisNode;
        EdgeElement(BrainArc *e, bool s) : edge(e), startsFromThisNode(s) {}
    };


    QList<EdgeElement> m_edgeList;
    LogicControl *m_graphLogic;
    int m_number;
    bool m_hasBorder;
    bool m_numberIsSpecial;
    QColor m_color;
    QColor m_textColor;
    QGraphicsDropShadowEffect *m_effect;

    static const double m_pi;
    static const double m_oneAndHalfPi;
    static const double m_twoPi;

    static const QColor m_gold;
};
//class BrainNode : public QGraphicsItem {
//    Q_OBJECT
//public:
//    BrainNode(/* graphlogic*/); // the underlying is the graphlogic
//    ~BrainNode();

//    //connect with different nodes with arcs
//    void addArc(BrainArc* arc, BrainNode* start); // need to start from this node
//    void deleteArc(BrainNode*head, BrainNode* tail);

//protected:
//private:

//};


#endif
