#ifndef BRAINNODE
#define BRAINNODE

#include <QGraphicsTextItem>
#include <QTextCursor>
#include<QGraphicsDropShadowEffect>

#include "brainarc.h"

class BrainNode : public QGraphicsItem {
    Q_OBJECT
public:
    BrainNode(/* graphlogic*/); // the underlying is the graphlogic
    ~BrainNode();

    //connect with different nodes with arcs
    void addArc(BrainArc* arc, BrainNode* start); // need to start from this node
    void deleteArc(BrainNode*head, BrainNode* tail);

protected:
private:

};


#endif
