#include "manager.h"

Manager::~Manager()
{
    for( const auto & i : components )
    {
        delete i;
    }
    for( const auto & i : states )
    {
        delete i;
    }
}
void Manager::addComponent(Component *c)
{
    components.append(c);
}
void Manager::addState(State *s)
{
    states.append(s);
}

void Manager::print()
{
    for (const auto & i : components)
    {
        i->print();
    }
}
State * Manager::searchState(QPoint pos)
{//Search if there is a state at this position
    int r,x,y;
    for( const auto & i : states )
    {
        r = i->getRad();
        x = i->getPos().x();
        y = i->getPos().y();

        if( x + r >= pos.x() && x - r <= pos.x() && y + r >= pos.y() && y - r <= pos.y())
        {
            qInfo()<< "search state ";
            return i;
        }
    }
    return nullptr;
}
bool Manager::intersectState(QPoint pos)
{
    int r,x,y;
    for( const auto & i : states )
    {
        r = i->getRad();
        x = i->getPos().x();
        y = i->getPos().y();

        if( qFabs( pos.x() - x ) <= 2*r && qFabs( pos.y() - y ) <= 2*r )
        {
            qInfo()<< "Intersect state ";
            return true;
        }
    }
    return false;
}
