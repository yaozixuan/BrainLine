#include "logiccontrol.h"

LogicControl::LogicControl(WidgetControl *parent)
    : QObject(parent)
{
    m_memberMap.insert(std::pair<int, void(LogicControl::*)()>
                       (Qt::Key_Insert, &LogicControl::InsertNode));
}

void LogicControl::RemoveAllNodes(){

}

void LogicControl::AddFirstNode(){

}

void LogicControl::InsertNode(){

}
