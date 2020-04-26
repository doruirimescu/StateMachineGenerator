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

bool Manager::intersectState(QPoint pos, State * s, int gridSize)
{/* Check if specified position intersects any state besides s*/
    int r, x, y;
    for( const auto & i : states )
    {/* Check all the states */
        r = i->getRad();
        x = i->getPos().x();
        y = i->getPos().y();
        qreal dist = sqrt( qFabs( pos.x() - x ) * qFabs( pos.x() - x ) + qFabs( pos.y() - y ) * qFabs( pos.y() - y ) );
        if( i != s && dist < 2 * ( r + gridSize ) )
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

/* Maps starting state and ending state to corresponding actions */
void Manager::mapStateToActions()
{
    Action *a = actions.back();

    if( stateActionStartMap.find(a->getStart()) == stateActionStartMap.end() )
    {/* Mapping does not exist. Create it. */
        stateActionStartMap.insert( a->getStart(), {a} );
    }
    else
    {
        stateActionStartMap[ a->getStart() ].push_back(a);
    }

    if( stateActionEndMap.find( a->getEnd()) == stateActionEndMap.end() )
    {/* Mapping does not exist. Create it. */
        stateActionEndMap.insert( a->getEnd(), {a} );
        qInfo()<<(a->getEnd()->getLabel());
    }
    else
    {
        stateActionEndMap[ a->getEnd() ].push_back(a);
    }
}

/* Updates startpoint and endpoint of actions related to state s */
void Manager::updateActionStartEnd(State *s)
{
    for( auto act : stateActionStartMap[s] )
    {
        act->replaceStart();
    }
    for( auto act: stateActionEndMap[s] )
    {
        if( act->getEndPoint() != invalidPoint )
        {
            act->replaceEnd();
        }
    }
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
vector<Wall> MAPPGridState::walls = {};

void Manager::Astar(int gridSize, int width, int height)
{
    /*
     * Create walls around each state
     */
    for( const auto &s : states )
    {
        QPoint center = QPoint( s->getPos().x() / gridSize, s->getPos().y() / gridSize );
        int rad = s->getRad() / gridSize;

        for(int  x =  center.x() - rad; x <= center.x() + rad; x += 1 )
        {
            if( x!= center.x() )
            {
                MAPPGridState::walls.push_back( Wall( x, (center.y() + rad) ) );
                MAPPGridState::walls.push_back( Wall( x, (center.y() - rad) ) );
            }
            else
            {
                MAPPGridState::walls.push_back( Wall( x, center.y() + rad - 1 ) );
                MAPPGridState::walls.push_back( Wall( x, center.y() - rad + 1 ) );
            }
        }
        for( int y = center.y() - rad; y <= center.y() + rad; y += 1 )
        {
            if( y != center.y() )
            {
                MAPPGridState::walls.push_back( Wall( center.x() + rad, y ) );
                MAPPGridState::walls.push_back( Wall( center.x() - rad, y ) );
            }
            else
            {
                MAPPGridState::walls.push_back( Wall( center.x() + rad - 1, y ) );
                MAPPGridState::walls.push_back( Wall( center.x() - rad + 1, y ) );
            }
        }
    }
    /*
     * Go through each action, solve A*, add walls
     */
    for( auto &a : actions )
    {
        vector<Agent> agents;
        unsigned int sX = a->getStartPoint().x() / gridSize;
        unsigned int sY = a->getStartPoint().y() / gridSize;
        unsigned int gX = a->getEndPoint().x() / gridSize;
        unsigned int gY = a->getEndPoint().y() / gridSize;
        agents.emplace_back( Agent( sX, sY, gX, gY, a->getLabel() ) );
        MAPPGridState grid(agents, width, height, 0 );
        vector<MAPPGridState> results = Astar::astar(grid);

        if( results.back().getH() == 0 )
        {/* Found a good result */
            a->clearSplits();
            for( const auto r : results )
            {
                for( const auto agent : r.agents )
                {/* Add walls around arrow */
                    MAPPGridState::walls.push_back( Wall( agent.getX(), agent.getY() ) );
                    a->addSplit( QPoint( agent.getX() * gridSize, agent.getY() * gridSize ) );
                }
            }
        }
        else
        {
            /* There was no good result */
            a->clearSplits();
            a->addSplit(a->getStartPoint());
            int splitPoint = Maths::roundToGrid( a->getStartPoint().x() + ( a->getEndPoint().x() - a->getStartPoint().x() ) / 2, gridSize );
            a->addSplit( QPoint ( splitPoint, a->getStartPoint().y() ) );
            a->addSplit( QPoint ( splitPoint, a->getEndPoint().y() ) );
            a->addSplit( a->getEndPoint() );
        }
    }
    MAPPGridState::walls.clear();
}
