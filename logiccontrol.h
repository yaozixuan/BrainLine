#ifndef LOGICCONTROL_H
#define LOGICCONTROL_H

#include <QObject>
#include <QUndoStack>

//#include "node.h"
#include "widgetcontrol.h"

class LogicControl : public QObject
{
    Q_OBJECT

public:

    LogicControl(WidgetControl *parent = 0);

    void RemoveAllNodes();
    void AddFirstNode();

public slots:

    // commands from toolbars:
    void InsertNode();
//    void RemoveNode();

private:
    std::map<int, void(LogicControl::*)(void)> m_memberMap;

};

#endif // LOGICCONTROL_H