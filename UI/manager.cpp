#include "manager.h"
/**
 * The manager manages components such as states
 * Can add state, remove state, search for state, check if position is on state border
 */

Manager::~Manager()
{
    for( const auto & i : actions )
    {
        delete i;
    }
    for( const auto & i : states )
    {
        delete i;
    }
}

/* State-related management */
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
            qInfo()<<"Deleting";
            for( int j = 0; j < actions.size(); ++j )
            {
                if( actions[j]->getStart() == s || actions[j]->getEnd() == s )
                {/* If action deals with state for deleting, replace action with nullptr*/
                    delete actions[j];
                    actions[j] = nullptr;
                }
            }
            states.erase(states.begin() + i);
            break;
        }
    }
    /* Erase nullptr action from actions */
    actions.erase(std::remove(actions.begin(), actions.end(), nullptr), actions.end());
    delete s;
}
void Manager::printStates()
{
    for (const auto & i : states)
    {
        i->print();
    }
}
State * Manager::searchState(QPoint pos)
{/* Search if there is a state at this position */
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
    {/* Check all the states */
        r = i->getRad();
        x = i->getPos().x();
        y = i->getPos().y();

        if( qFabs( pos.x() - x ) < 2 * r && qFabs( pos.y() - y ) < 2 * r )
        {
            qInfo()<< "Intersect state ";
            return true;
        }
    }
    return false;
}
bool Manager::intersectState(QPoint pos, State * s)
{
    int r,x,y;
    for( const auto & i : states )
    {/* Check all the states */
        r = i->getRad();
        x = i->getPos().x();
        y = i->getPos().y();

        if( i != s && qFabs( pos.x() - x ) < 2 * r && qFabs( pos.y() - y ) < 2 * r )
        {
            qInfo()<< "Intersect state ";
            return true;
        }
    }
    return false;
}

QPoint Manager::onStateBorder(QPoint pos , QString &posInfo)
{
    int r, x, y;
    for( const auto & i : states )
    {/* Go for each state and determine if cursor in border */
        r = i->getRad();
        x = i->getPos().x();
        y = i->getPos().y();

        /* Absolute distances */
        float fabsX = qFabs( pos.x() - x );
        float fabsY = qFabs( pos.y() - y );

        float dist = sqrt( fabsX*fabsX + fabsY*fabsY );

        int db = 10;

        if( dist <= ( r + db ) && dist >= ( r - db )  )
        {/* If inside or on border of state */
            /* Get cursor angle to find out in which quadrant it is*/
            double atan = qAtan2( y -pos.y(), pos.x() - x );
            if( atan >= MINUS_PI_OVER_4 && atan <= PI_OVER_4 )
            {
                posInfo = QString("R");
                return QPoint(x+r, y);
            }
            else if( atan >= PI_OVER_4 && atan <= THREE_PI_OVER_4 )
            {
                posInfo = QString("U");
                return QPoint(x, y-r);
            }
            else if( ( atan >= THREE_PI_OVER_4 && atan <= M_PI ) || (atan >= -M_PI && atan <= MINUS_THREE_PI_OVER_4 ) )
            {
                posInfo = QString("L");
                return QPoint(x-r, y);
            }
            else if( atan >= MINUS_THREE_PI_OVER_4 && atan < MINUS_PI_OVER_4 )
            {
                posInfo = QString("D");
                return QPoint(x, y+r);
            }
        }
    }
    return QPoint(-1000,-1000);
}

/* Action-related management */

void Manager::addAction(Action *a)
{
    actions.append(a);
}
Action* Manager::getLastAction()
{
    return actions.back();
}
void Manager::printActions()
{
    for ( const auto & i : actions )
    {
        i->print();
    }
}
void Manager::deleteAction(Action *a)
{
    for( int i = 0; i < actions.size(); ++i )
    {
        if( actions[ i ] == a )
        {
            actions.erase(actions.begin() + i);
        }
    }
    delete a;
}
