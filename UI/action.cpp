#include "action.h"

void Action::print()
{
    qInfo()<<"Action from state: "<<this->start->getLabel()<<" to state: "<<this->end->getLabel();
}
