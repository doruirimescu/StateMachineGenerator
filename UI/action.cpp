#include "action.h"

void Action::print()
{
    qInfo()<<"Action from state: "<<this->start->getLabel()<<" to state: "<<this->end->getLabel();
    qInfo()<<"Label: "<<label;
}

void Action::replaceStart()
{/* Action start state position has been replaced */
    if( splits.size() > 2 )
    {
        QPoint newPos = start->getPos();
        if( startAnchor.compare("R") == 0)
        {
            startPos = newPos + QPoint( start->getRad(), 0 );
            splits.replace(0, startPos);
        }
        else if( startAnchor.compare("L") == 0 )
        {
            startPos = newPos - QPoint( start->getRad(), 0 );
            splits.replace(0, startPos);
        }
        else if( startAnchor.compare("U") == 0 )
        {
            startPos = newPos - QPoint( 0, start->getRad() );
            splits.replace(0, startPos);
        }
        else if( startAnchor.compare("D") == 0 )
        {
            startPos = newPos + QPoint( 0, start->getRad() );
            splits.replace(0, startPos);
        }
    }
}
void Action::replaceEnd()
{
    if( splits.size() > 2 )
    {
        QPoint newPos = end->getPos();
        if( endAnchor.compare("R") == 0)
        {
            endPos = newPos + QPoint( end->getRad(), 0 );
            splits.replace(splits.size() - 1, endPos);
        }
        else if( endAnchor.compare("L") == 0 )
        {
            endPos = newPos - QPoint( end->getRad(), 0 );
            splits.replace(splits.size() - 1, endPos);
        }
        else if( endAnchor.compare("U") == 0 )
        {
            endPos = newPos - QPoint( 0, end->getRad() );
            splits.replace(splits.size() - 1, endPos);
        }
        else if( endAnchor.compare("D") == 0 )
        {
            endPos = newPos + QPoint( 0, end->getRad() );
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
