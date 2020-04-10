#include "action.h"

void Action::print()
{
    qInfo()<<"Action from state: "<<this->start->getLabel()<<" to state: "<<this->end->getLabel();
}
State* Action::getStart()
{
    return this->start;
}
void Action::setStart(State * s)
{
    this->start = s;
}
State* Action::getEnd()
{
    return this->end;
}
void Action::setEnd(State * s)
{
    this->end = s;
}
QPoint Action::getStartPoint()
{
    return this->startPos;
}
void Action::setStartPoint(QPoint * p)
{
    this->startPos = *p;
}
QPoint Action::getEndPoint()
{
    return this->endPos;
}
void Action::setEndPoint(QPoint * p)
{
    this->endPos = *p;
}
