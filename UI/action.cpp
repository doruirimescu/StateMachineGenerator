#include "action.h"

void Action::print()
{
    qInfo()<<"Action from state: "<<this->startState->getLabel()<<" to state: "<<this->endState->getLabel();
    qInfo()<<"Label: "<<label;
}
long Action::getDistance()const
{
    return pow( getStartPoint().x() - getEndPoint().x(), 2 ) +
           pow( getStartPoint().y() - getEndPoint().y(), 2 );
}
void Action::replaceStart()
{/* Action start state position has been replaced */
    if( splits.size() > 2 )
    {
        QPoint newPos = startState->getPos();
        if( startAnchor.compare("R") == 0)
        {
            startPos = newPos + QPoint( startState->getRad(), 0 );
            splits.replace(0, startPos);
        }
        else if( startAnchor.compare("L") == 0 )
        {
            startPos = newPos - QPoint( startState->getRad(), 0 );
            splits.replace(0, startPos);
        }
        else if( startAnchor.compare("U") == 0 )
        {
            startPos = newPos - QPoint( 0, startState->getRad() );
            splits.replace(0, startPos);
        }
        else if( startAnchor.compare("D") == 0 )
        {
            startPos = newPos + QPoint( 0, startState->getRad() );
            splits.replace(0, startPos);
        }
    }
}
void Action::replaceEnd()
{
    if( splits.size() > 2 )
    {
        QPoint newPos = endState->getPos();
        if( endAnchor.compare("R") == 0)
        {
            endPos = newPos + QPoint( endState->getRad(), 0 );
            splits.replace(splits.size() - 1, endPos);
        }
        else if( endAnchor.compare("L") == 0 )
        {
            endPos = newPos - QPoint( endState->getRad(), 0 );
            splits.replace(splits.size() - 1, endPos);
        }
        else if( endAnchor.compare("U") == 0 )
        {
            endPos = newPos - QPoint( 0, endState->getRad() );
            splits.replace(splits.size() - 1, endPos);
        }
        else if( endAnchor.compare("D") == 0 )
        {
            endPos = newPos + QPoint( 0, endState->getRad() );
            splits.replace(splits.size() - 1, endPos);
        }
    }
}
bool Action::hasAnchorAt(const QPoint &p)const
{
    if( p == startPos || p == endPos )
    {
        return true;
    }
    else
    {
        return false;
    }
}
