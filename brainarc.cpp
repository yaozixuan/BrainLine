#include <QPainter>
#include <QDebug>


#include "brainarc.h"
#include "brainnode.h"

#include <math.h>

const double BrainArc::m_pi = 3.14159265358979323846264338327950288419717;
const double BrainArc::m_twoPi = 2.0 * BrainArc::m_pi;
const qreal BrainArc::m_arrowSize = 7;

BrainArc::BrainArc(BrainNode *sourceNode, BrainNode *destNode)
    : m_sourceNode(sourceNode)
    , m_destNode(destNode)
    , m_angle(-1)
    , m_color(0,0,0)
    , m_width(1)
{
    // does not interact with user
    setAcceptedMouseButtons(0);
    setZValue(1);

    adjust();
}

BrainArc::~BrainArc()
{
    m_sourceNode->removeEdge(this);
    m_destNode->removeEdge(this);
}

BrainNode * BrainArc::sourceNode() const
{
    return m_sourceNode;
}

BrainNode * BrainArc::destNode() const
{
    return m_destNode;
}

double BrainArc::angle() const
{
    return m_angle;
}

QColor BrainArc::color() const
{
    return m_color;
}

void BrainArc::setColor(const QColor &color)
{
    m_color = color;
    update();
}

qreal BrainArc::width() const
{
    return m_width;
}

void BrainArc::setWidth(const qreal &width)
{
    if (width < 1 || width > 100)
        return;

    m_width = width;
    update();
}

void BrainArc::adjust()
{
    prepareGeometryChange();

    QLineF line(m_sourceNode->sceneBoundingRect().center(),
                m_destNode->sceneBoundingRect().center());

    m_destPoint = m_destNode->intersection(line, true);
    m_sourcePoint = m_sourceNode->sceneBoundingRect().center();
}

QRectF BrainArc::boundingRect() const
{
    if (!m_sourceNode || !m_destNode)
        return QRectF();

    qreal penWidth = 1;
    qreal extra = (penWidth + m_arrowSize  + m_width) / 2.0;

    return QRectF(m_sourcePoint,
                  QSizeF(m_destPoint.x() - m_sourcePoint.x(),
                         m_destPoint.y() - m_sourcePoint.y()))
                            .normalized().adjusted(-extra, -extra,
                                                   extra, extra);
}

void BrainArc::paint(QPainter *painter,
                 const QStyleOptionGraphicsItem *,
                 QWidget *w)
{
    Q_UNUSED(w);

    // calculate angle
    QLineF line(m_sourcePoint, m_destPoint);

    m_angle = ::acos(line.dx() / line.length());
    if (line.dy() >= 0)
        m_angle = BrainArc::m_twoPi - m_angle;

    // no need to draw when the nodes overlap
    if (sourceNode()->collidesWithItem(destNode()))
        return;

    // Draw the line itself
    painter->setPen(QPen(m_color, m_width, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter->drawLine(line);

    if (line.length() < m_arrowSize)
        return;

    // Draw the arrow
    painter->setPen(QPen(m_color,
                         m_width,
                         Qt::SolidLine,
                         Qt::RoundCap,
                         Qt::RoundJoin));

    painter->setBrush(m_color);
    qreal arrowSize = m_arrowSize + m_width;

    // no need to draw the arrow if the nodes are too close
    if (line.length() < arrowSize)
    {
        painter->drawLine(m_sourcePoint, m_destPoint);
        return;
    }

    QPointF destArrowP1 = m_destPoint +
                          QPointF(sin(m_angle - BrainArc::m_pi / 3) * arrowSize,
                                  cos(m_angle - BrainArc::m_pi / 3) * arrowSize);
    QPointF destArrowP2 = m_destPoint +
              QPointF(sin(m_angle - BrainArc::m_pi + BrainArc::m_pi / 3) * arrowSize,
                      cos(m_angle - BrainArc::m_pi + BrainArc::m_pi / 3) * arrowSize);


    painter->drawPolygon(QPolygonF() << line.p2()
                                     << destArrowP1
                                     << destArrowP2);
}
