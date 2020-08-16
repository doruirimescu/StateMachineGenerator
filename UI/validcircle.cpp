#include "validcircle.h"
#include <QPen>

void ValidCircle::draw()
{
    /* Draw new circle */
    painter->setPen( QPen(*borderColor, borderWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin) );
    painter->setBrush(*fillColor);
    painter->setOpacity(0.2);
    painter->drawEllipse( *center, radius, radius );
    painter->end();
}
void ValidCircle::clear()
{
    /* Clear gridline where new circle is drawn */
    painter->setPen( QPen(QColor( 0xffffff ), borderWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin) );
    painter->setBrush( QColor(0xffffff) );
    painter->drawEllipse( *center, radius, radius );
}
