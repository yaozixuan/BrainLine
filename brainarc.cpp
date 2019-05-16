#include <QPainter>
#include <QDebug>

#include "brainarc.h"
#include "brainnode.h"
#include <math.h>

const double BrainArc::m_pi = 3.14159265358979323846264338327950288419717;
const qreal BrainArc::m_arrowSize = 6;

BrainArc::BrainArc(BrainNode* start, BrainNode* end)
      : m_start(start)
      , m_end(end)
      , m_angle(-1)
      , m_color(0,0,0)
      , m_color_1(147,112,219)
      , m_width(0.5) {
    setAcceptedMouseButtons(0);
    setZValue(1);

    changePos();
}

BrainArc::~BrainArc(){
    //destructing arc means to remove remove edge of both nodes
    m_start->removeArc(this);
    m_end->removeArc(this);
}

BrainNode* BrainArc::getStart() const {
    return m_start;
}

BrainNode* BrainArc::getEnd() const {
    return m_end;
}

double BrainArc::getAngle() const {
    return m_angle;
}

QColor BrainArc::getColor() const {
    return m_color_1;
}

void BrainArc::setColor(const QColor &color) {
    m_color_1 = color;
    //update the graphics attribute after color change
    update();
}

qreal BrainArc::getWidth() const {
    return m_width;
}


//width setter for width between 1 and 100
void BrainArc::setWidth(const qreal &width)
{
    if (width >= 1 && width <= 100) {
        m_width = width;
        update();
    }
}

void BrainArc::changePos() {
    prepareGeometryChange();

    //
    QLineF line(m_start->);

    m_startPoint = m_start->intersection(line, true)

}










