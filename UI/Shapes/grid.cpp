#include "grid.h"
#include "line.h"

#include <QPen>
#include <QPoint>

Grid::Grid( QPen* const &pen, double opacity, int width, int height, int size, QPaintDevice* const &dev) :
    pen(pen), opacity(opacity), width(width), height(height), size(size), paintDevice(dev) {};

void Grid::draw()
{
    for( int x = 0; x < width; x += size )
    {/* Draw horizontal lines */
        const QPoint start = QPoint(x, 0);
        const QPoint end = QPoint(x, height);
        Line l( &start, &end, pen, paintDevice );
        l.draw();
    }

    for( int y = 0; y < height; y += size )
    {/* Draw vertical lines */
        const QPoint start = QPoint(0, y);
        const QPoint end = QPoint(width, y);
        Line l( &start, &end, pen, paintDevice );
        l.draw();
    }
}
