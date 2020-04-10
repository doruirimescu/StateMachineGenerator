#ifndef ACTION_H
#define ACTION_H
#include "state.h"

class Action
{
public:
    Action( State* s, State*e, QPoint sP, QPoint eP ) : start(s), end(e), startPos(sP), endPos(eP){};
    ~Action(){qInfo()<<"Action from state: "<<this->start->getLabel()<<" to state: "<<this->end->getLabel()<<"DEAD";}
    void print();
    State* getStart();
    State* getEnd();
    QPoint getStartPoint();
    QPoint getEndPoint();

    void setStart(State*);
    void setEnd(State*);
    void setStartPoint(QPoint*);
    void setEndPoint(QPoint*);
private:
    State* start;
    State* end;
    QPoint startPos;
    QPoint endPos;
};

#endif // ACTION_H
