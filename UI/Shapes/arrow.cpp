#include "arrow.h"
#include<QPoint>
#include <QPainter>
#include "maths.h"
Arrow::Arrow(const int x0, const int y0, const int x, const int y, const int w, QPaintDevice* const &pdev) : paintDevice(pdev)
{
    /* Complicated calculations for drawing a simple arrow. */
    int dx, dy;
    double sina, cosa, l;


    l = qSqrt( (x - x0) * (x - x0)  + (y - y0) * (y - y0) );
    sina = (y - y0) / l;
    cosa = (x - x0) / l;
    dx = ( (sina * w) / 2 );
    dy = ( (cosa * w) / 2 );

    start= new QPoint(x0, y0);
    end  = new QPoint(x, y);
    right= new QPoint( dx + x0, (dy + y0) );
    left = new QPoint( x0 - dx, (y0 - dy) );
}
void Arrow::draw()
{
    QPainter* painter = new QPainter(paintDevice);
    QBrush brush;
    QPainterPath path;
    QPolygon poly;

    poly<< *start;
    poly<< *right;
    poly<< *end;
    poly<< *left;

    path.clear();

    brush.setColor(Qt::black);
    brush.setStyle(Qt::SolidPattern);

    path.addPolygon(poly);

    painter->setRenderHint(QPainter::Antialiasing);
    painter->drawPolygon(poly);
    painter->fillPath(path, brush);
    painter->end();
}
