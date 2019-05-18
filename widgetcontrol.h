#ifndef WIDGETCONTROL_H
#define WIDGETCONTROL_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QKeyEvent>
#include <QGraphicsSceneMouseEvent>
#include <QMenu>

class MainWindow;
class LogicControl;

/* Deal with:
 * 1. New a BrainLine (clear & add first node) Close BrainLine
 * 2. Handle Zoom in / out effects
 * 3. Key press event
 */

class WidgetControl : public QGraphicsView
{
    Q_OBJECT

public:

    WidgetControl(MainWindow *parent = 0);

    void newScene();
    void closeScene();

    LogicControl *logicControl() const;

    static const QColor m_paperColor;

public slots:

    void zoomIn();
    void zoomOut();

signals:

//    void  notification(const QString &msg);

protected:

    void keyPressEvent(QKeyEvent *event);
    void wheelEvent(QWheelEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void drawBackground(QPainter *painter, const QRectF &rect);

private:

    void scaleView(qreal factor);


    MainWindow *m_parent;
    QGraphicsScene *m_scene;
    LogicControl *m_logic;

    QMenu *m_RightMenu;
    QAction *m_AddNode;
    QAction *m_RemoveNode;
    QAction *m_AddArc;
    QAction *m_RemoveArc;
    QAction *m_NodeColor;
    QAction *m_TextColor;
};

#endif // WIDGETCONTROL_H
