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
void Manager::deleteState(State *s)
{
    for(int i = 0; i < states.size(); ++i )
    {
        if( states[ i ] == s )
        {
            states.erase(states.begin()+i);
        }
    }
    delete s;
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

        if( qFabs( pos.x() - x ) < 2*r*1.1 && qFabs( pos.y() - y ) < 2*r*1.1 )
        {
            qInfo()<< "Intersect state ";
            return true;
        }
    }
    return false;
}
QString Manager::onStateBorder(QPoint pos)
{
    int r,x,y;
    for( const auto & i : states )
    {
        r = i->getRad();
        x = i->getPos().x();
        y = i->getPos().y();

        int signX = qFabs( pos.x() - x ) / ( pos.x() - x);
        int signY = qFabs( pos.y() - y ) / ( pos.y() - y);

        float fabsX = qFabs( pos.x() - x );
        float fabsY = qFabs( pos.y() - y );

        float dist = sqrt( fabsX*fabsX + fabsY*fabsY );

        if( dist <= ( r + 1 ) && dist >= ( r - 1 )  )
        {// In or on border of state
            if( fabsY <= 10 && fabsX > 15 )
            {
                if( signX >= 0  )
                {//Right border
                    return "r";
                }
                else if ( fabsX > 10 )
                {//Left border
                    return "l";
                }
            }
            else if( fabsX <= 10 && fabsY > 15 )
            {
                if( signY <=0 )
                {//Up border
                    return "u";
                }
                else
                {
                    return "d";
                }
            }
        }
    }
    return "f";
}
