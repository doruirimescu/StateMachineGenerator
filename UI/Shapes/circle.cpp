#include "circle.h"
#include <QPen>
#include <QPainter>
#include <QColor>
Circle::Circle(const QPoint* const &center, int radius, QPaintDevice* const &dev, QColor *fillColor, QPen *pen, const double opacity) :
    center(center), radius(radius), paintDevice(dev), fillColor(fillColor), pen(pen), opacity(opacity)
{
}
void Circle::draw()
{
    QPainter* painter;
    painter = new QPainter(paintDevice);
    painter->setRenderHint(QPainter::Antialiasing);

    /* Draw new circle */
    painter->setPen( *pen );
    painter->setBrush(*fillColor);
    painter->setOpacity(opacity);
    painter->drawEllipse( *center, radius, radius );
    painter->end();
}
void Circle::clear()
{
    QPainter* painter;
    painter = new QPainter(paintDevice);

    /* Clear gridline where new circle is drawn */
    painter->setPen( QPen(QColor( 0xffffff ), pen->width(), Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin) );
    painter->setBrush( QColor(0xffffff) );
    painter->drawEllipse( *center, radius, radius );
    painter->end();
}



