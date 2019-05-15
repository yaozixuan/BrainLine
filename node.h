#ifndef NODE_H
#define NODE_H

#include <QObject>
#include <QWidget>
#include <QGraphicsItem>
#include<QString>

class Node : public QObject, public QGraphicsItem
{
Q_OBJECT
Q_INTERFACES(QGraphicsItem)

public:
    Node();
    Node(const Node & other);

    virtual ~Node() override;

    virtual void addGraphicsEdge(Edge & edge);

    virtual void removeGraphicsEdge(Edge & edge);

    void adjustSize();

    virtual QRectF boundingRect() const override;

    using NodePtr = std::shared_ptr<Node>;
    virtual EdgePtr createAndAddGraphicsEdge(NodePtr targetNode);

    virtual void paint(QPainter * painter,
                       const QStyleOptionGraphicsItem * option, QWidget * widget = nullptr) override;

    //! Sets the Node and QGraphicsItem locations.
    virtual void setLocation(QPointF newLocation);

    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent * event) override;

    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent * event) override;

    virtual void hoverMoveEvent(QGraphicsSceneHoverEvent * event) override;

    static std::pair<QPointF, QPointF> getNearestEdgePoints(const Node & node1, const Node & node2);

    void setHandlesVisible(bool visible, bool all = true);

    QString text() const override;

    void setText(const QString & text) override;

    void setTextColor(const QColor & color) override;

    void setTextSize(int textSize) override;

    void setSelected(bool selected) override;
explicit node(QObject *parent = nullptr);

signals:
    void undoPointRequested();

private:

    void checkHandleVisibility(QPointF pos);

    void createEdgePoints();

    void createHandles();

    NodeHandle * hitsHandle(QPointF pos);

    void initTextField();

    bool isTextUnderflowOrOverflow() const;

    void updateEdgeLines();

    std::vector<NodeHandle *> m_handles;

    std::vector<Edge *> m_graphicsEdges;

    std::vector<QPointF> m_edgePoints;

    TextEdit * m_textEdit;

    QTimer m_handleVisibilityTimer;

    QPointF m_currentMousePos;

    bool m_mouseIn = false;
};

using NodePtr = std::shared_ptr<Node>;

#endif // NODE_H
