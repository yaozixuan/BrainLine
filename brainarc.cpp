#include "brainarc.h"
#include "brainnode.h"
#include <QPainter>
#include <QtDebug>
#include "math.h" /*include acos*/

//constructor implementation
const double BrainArc::m_pi = 3.14159265358979323846264338327950288419717;
const qreal BrainArc::m_arrowSize = 6;

BrainArc::BrainArc(BrainNode* start, BrainNode* end)
    : m_start(start)
    , m_end(end)
    , m_angle(-1)
    , m_color(0,0,0)
    , m_color_1(147,112,219)
    , m_width(0.5)
    ,m_child(true)


{
    setAcceptedMouseButtons(0);
    setZValue(1);
    changePos();
}

BrainArc::~BrainArc(){
    //todo
    //是和node的function有关
}


BrainNode* BrainArc::getStart() const {
    return m_start;
}

BrainNode* BrainArc::getEnd() const {
    return m_end;
}

qreal BrainArc::getWidth() const {
    return m_width;
}

double BrainArc::getAngle() const {
    return m_angle;
}

//return the color for the arrow and connection line
QColor BrainArc::getColor() const {
    return m_color_1;
}

void BrainArc::setColor(const QColor &color) {
    m_color_1  =color;
    update();
}

void BrainArc::setWidth(const qreal& width) {
    /*set the width range between 1 and 100*/
    if (width >= 1 && width <= 100) {
        m_width = width;
        update();
    }
}

bool BrainArc::childNode() const{
    return m_child;
}

void BrainArc::setChildNode(const bool& child) {
    m_child = child;
    update();
}

void BrainArc::changePos(){

//    prepareGeometryChange();
//    QLine line(m_start->boundingRect.center(),
//               m_end->boundingRect.center());
//    m_startPoint = m_start->nodeBoudingRect().center();
//    m_endPoint = m_end->intersection(line, true);

}


//implement the bounding area for connection line
QRectF BrainArc::arcbBoundingArea() const {
    qreal boundWidth = 2;
    qreal smallAdjust = (boundWidth + m_arrowSize + m_width) / 2.0;

    return QRectF(m_startPoint, QSizeF(m_endPoint.x() - m_startPoint.x(),
                                      m_endPoint.y() - m_startPoint.y())).normalized()
        .adjusted(-smallAdjust, -smallAdjust, smallAdjust, smallAdjust);

}
//I still don't know what is QStyleOptionGraphicsItem
void BrainArc::connect(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget) {

    //unused macro to supprese unused widget
    Q_UNUSED(widget);

    //this does not actually draw a line
    QLineF line(m_startPoint, m_endPoint);

    //this one looks for x outside the current scope
    m_angle = ::asin(line.dy() / line.length());

    if(line.dy() >= 0) {
        m_angle = 2*m_pi - m_angle;
    }
    //
    if(getStart()->collidesWithItem(getEnd()))
        return;


    painter->setPen(QPen(m_color_1,
                         m_width,
                         m_child ?
                                 Qt::DashDotLine :
                                 Qt ::SolidLine,
                         Qt::SquareCap,
                         Qt::BevelJoin));
    painter->drawLine(line);

    if(line.length() < m_arrowSize)
        return;
    //draw the arrow( adpatable)
    painter->setPen(QPen(m_color_1,
                         m_width,
                         Qt::SolidLine,
                         Qt::RoundCap,
                         Qt::RoundJoin));
    //the setting of the painter for the line and arrow
    painter->setBrush(m_color_1);


    if (line.length() < m_arrowSize) {
        painter->drawLine(m_startPoint, m_endPoint);
        return;
    }

    QPointF endArrowP1 = m_endPoint + QPointF(sin(m_angle - m_pi / 3) * m_arrowSize,
                                              cos(m_angle - m_pi / 3) * m_arrowSize);

    QPointF endArrowP2 = m_endPoint + QPointF(sin(m_angle - m_pi + m_pi / 3 ) * m_arrowSize,
                                              cos(m_angle - m_pi + m_pi / 3 ) * m_arrowSize);

    painter->drawPolygon(QPolygonF() << line.p2()
                                     << endArrowP1
                                     << endArrowP2);

}









