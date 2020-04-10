#include "manager.h"
/**
 * The manager manages components such as states
 * Can add state, remove state, search for state, check if position is on state border
 */

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
QPoint Manager::onStateBorder(QPoint pos)
{
    int r, x, y;
    for( const auto & i : states )
    {/* Go for each state and determine if cursor in border*/
        r = i->getRad();
        x = i->getPos().x();
        y = i->getPos().y();

        /* Absolute distances */
        float fabsX = qFabs( pos.x() - x );
        float fabsY = qFabs( pos.y() - y );

        float dist = sqrt( fabsX*fabsX + fabsY*fabsY );

        int db = 10;

        if( dist <= ( r + db ) && dist >= ( r - db )  )
        {// In or on border of state
            double atan = qAtan2( y -pos.y(), pos.x() - x );
            qInfo()<<atan;
            if( atan >= -M_PI/4 && atan <= M_PI/4 )
            {
                qInfo()<< "R";
                return QPoint(x+r, y);
            }
            else if( atan >= M_PI/4 && atan <= 3*M_PI/4 )
            {
                qInfo()<< "U";
                return QPoint(x, y-r);
            }
            else if( ( atan >= 3*M_PI/4 && atan <= M_PI ) || (atan >= -M_PI && atan <= -3*M_PI/4 ) )
            {
                qInfo()<< "L";
                return QPoint(x-r, y);
            }
            else if( atan >= -3*M_PI/4 && atan < -M_PI/4 )
            {
                qInfo()<< "D";
                return QPoint(x, y+r);
            }
        }
    }
    return QPoint(-1000,-1000);
}
