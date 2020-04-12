#ifndef ACTION_H
#define ACTION_H
#include "state.h"

class Action
{
public:
    /* Constructor-destructor */
    Action( State* s, State* e, QPoint sP, QPoint eP ) : start(s), end(e), startPos(sP), endPos(eP){};
    ~Action(){qInfo()<<"Action from state: "<<this->start->getLabel()<<" to state: "<<this->end->getLabel()<<"DEAD";}

    void print();

    /* Start state setter-getter */
    void setStart(State* &s){ start = s; };
    State* getStart() const { return start; };

    /* End state setter-getter */
    void setEnd(State* &e){ end = e; };
    State* getEnd() const { return end; };

    /* Start anchor position setter-getter */
    void setStartPoint(QPoint* s){ startPos = *s; };
    QPoint getStartPoint() const { return startPos; };

    /* End anchor position setter-getter */
    void setEndPoint(QPoint *e){ endPos = *e; };
    QPoint getEndPoint() const { return endPos; };

private:
    State* start;
    State* end;
    QPoint startPos;
    QPoint endPos;
};

#endif // ACTION_H
