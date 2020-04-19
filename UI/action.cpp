#include "action.h"

void Action::print()
{
    qInfo()<<"Action from state: "<<this->start->getLabel()<<" to state: "<<this->end->getLabel();
    qInfo()<<"Label: "<<label;
}

void Action::replaceStart()
{/* Action start state position has been replaced */
    QPoint newPos = start->getPos();

    if( startAnchor.compare("R") == 0)
    {
        startPos = newPos + QPoint( start->getRad(), 0 );
        splits.replace(0, startPos);
        QPoint s2 = splits.at(1);
        s2.setY( newPos.y());
        splits.replace(1, s2 );
    }
    else if( startAnchor.compare("L") == 0 )
    {
        startPos = newPos - QPoint( start->getRad(), 0 );
        splits.replace(0, startPos);
        QPoint s2 = splits.at(1);
        s2.setY( newPos.y());
        splits.replace(1, s2 );
    }
    else if( startAnchor.compare("U") == 0 )
    {
        startPos = newPos - QPoint( 0, start->getRad() );
        splits.replace(0, startPos);
        QPoint s2 = splits.at(1);
        s2.setY( newPos.y() - start->getRad() );
        splits.replace(1, s2 );  
    }
    else if( startAnchor.compare("D") == 0 )
    {
        startPos = newPos + QPoint( 0, start->getRad() );
        splits.replace(0, startPos);
        QPoint s2 = splits.at(1);
        s2.setY( newPos.y() + start->getRad() );
        splits.replace(1, s2 );
    }
}
void Action::replaceEnd()
{
    QPoint newPos = end->getPos();
    if( endAnchor.compare("R") == 0)
    {
        endPos = newPos + QPoint( end->getRad(), 0 );
        splits.replace(splits.size() - 1, endPos);
        QPoint s2 = splits.at( splits.size() - 2 );
        s2.setY( newPos.y() );
        splits.replace(splits.size() - 2, s2 );
    }
    else if( endAnchor.compare("L") == 0 )
    {
        endPos = newPos - QPoint( end->getRad(), 0 );
        splits.replace(splits.size() - 1, endPos);
        QPoint s2 = splits.at( splits.size() - 2 );
        s2.setY( newPos.y() );
        splits.replace(splits.size() - 2, s2 );
    }
    else if( endAnchor.compare("U") == 0 )
    {
        endPos = newPos - QPoint( 0, end->getRad() );
        splits.replace(splits.size() - 1, endPos);
        QPoint s2 = splits.at( splits.size() - 2 );
        s2.setY( newPos.y() - end->getRad() );
        splits.replace(splits.size() - 2, s2 );
    }
    else if( startAnchor.compare("D") == 0 )
    {
        endPos = newPos + QPoint( 0, end->getRad() );
        splits.replace(splits.size() - 1, endPos);
        QPoint s2 = splits.at( splits.size() - 2 );
        s2.setY( newPos.y() + end->getRad() );
        splits.replace(splits.size() - 2, s2 );
    }

}
