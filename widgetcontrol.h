#ifndef WIDGETCONTROL_H
#define WIDGETCONTROL_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QKeyEvent>
#include <QGraphicsSceneMouseEvent>

class MainWindow;
class LogicControl;

/* Deal with:
 * 1. New a BrainLine (clear & add first node) Close BrainLine
 * 2. Handle Zoom in / out effects
 * 3. Key press event
 */

class WidgetControl : public QGraphicsView
{
//    Q_OBJECT

public:
    WidgetControl(MainWindow * parent = 0);

    void NewBrainLine();
    void CloseBrainLine();

    static const QColor m_paperColor;

public slots:

    void ZoomIn();
    void ZoomOut();


signals:

protected:

    void keyPressEvent(QKeyEvent *event);
    void wheelEvent(QWheelEvent *event);
    void drawBackground(QPainter *painter, const QRectF &rect);

private:

//    void Zoom(qreal factor);
//    MainWindow *b_parent;
    QGraphicsScene *m_scene;
    LogicControl *m_logic;
};

#endif // WIDGETCONTROL_H
