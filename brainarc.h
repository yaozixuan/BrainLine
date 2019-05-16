#ifndef BRAINARC_H
#define BRAINARC_H
#include <QGraphicsItem>
#include <iostream>

class BrainNode;

class BrainArc : public QGraphicsItem
{
public:

    //arc constructor and destructor
    BrainArc(BrainNode* start, BrainNode* end);
    ~BrainArc();

    //an arc connects two nodes
    //using const so that function calls on the same objects
    BrainNode* getStart() const;
    BrainNode* getEnd() const;

    //get the angle of line using acos function
    //the angle used to draw connection line and arrow
    double getAngle() const;

    //color getters and setters for the connection line
    QColor getColor() const;
    void setColor(const QColor& color);

    //width getters and setters for the connection line
    qreal getWidth() const;
    void setWidth(const qreal& width);

    //change arc connection when node objects change
    void changePos();

    bool childNode() const;
    void setChildNode(const bool& child = true);

protected:
    //return the bounding area for arcs where that could be drawn upon
    QRectF arcbBoundingArea() const;
    //
    void connect(QPainter* painter,const QStyleOptionGraphicsItem* option, QWidget* widget);

private:
    //BrainNode member variable
    BrainNode* m_start;
    BrainNode* m_end;
    double m_angle; //m_abge = -1 是什么东西

    //BrainNode coordinates
    QPointF m_startPoint;
    QPoint m_endPoint;

    //BrainNode color
    QColor m_color;

    //Connection line and arrow color
    QColor m_color_1;

    //BrainNode width
    qreal m_width;

    //assign logical and temporal connection between start and end nodes;
    bool m_child;
    //const arrow size
    static const qreal m_arrowSize;
    static const double m_pi;

};

#endif // MODEL_ARC_H
