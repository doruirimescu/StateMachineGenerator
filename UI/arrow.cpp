#include "arrow.h"
#include <QColor>
#include <QImage>
#include <PropertyHelper.h>
#include <QPainter>
#include <QObject>
#include "maths.h"
Arrow::Arrow(int x0, int y0, int x, int y, int w, QPainter *painter)
{
    /* Complicated calculations for drawing a simple arrow. */
    int dx, dy;
    double sina, cosa, l;
    l = qSqrt( (x - x0) * (x - x0)  + (y - y0) * (y - y0) );
    sina = (y - y0) / l;
    cosa = (x - x0) / l;
    dx = ( (sina * w) / 2 );
    dy = ( (cosa * w) / 2 );

    start= QPoint(x0, y0);
    end  = QPoint(x, y);
    right= QPoint( dx + x0, (dy + y0) );
    left = QPoint( x0 - dx, (y0 - dy) );
    this->painter = painter;
}
void Arrow::draw()
{
    QBrush brush;
    QPainterPath path;

    QPolygon poly;
    poly<< start;
    poly<<right;
    poly<< end;
    poly<<left;

    path.clear();
    brush.setColor(Qt::black);
    brush.setStyle(Qt::SolidPattern);
    path.addPolygon(poly);
    painter->setRenderHint(QPainter::Antialiasing);
    painter->drawPolygon(poly);
    painter->fillPath(path, brush);
}
