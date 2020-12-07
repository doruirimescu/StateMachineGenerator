#include "grid.h"
#include "line.h"

#include <QPen>
#include <QPoint>

Grid::Grid( QPen* const &pen, double opacity, int width, int height, int size, QPaintDevice* const &dev) :
    pen(pen), opacity(opacity), width(width), height(height), size(size), paintDevice(dev)
{
    construct();
}

void Grid::construct()
{
    /* Constructs a grid made of Line objects */
    gridLines.clear();
    for( int x = 0; x < width; x += size )
    {/* Draw horizontal lines */
        const QPoint start = QPoint(x, 0);
        const QPoint end = QPoint(x, height);
        std::unique_ptr<Line> ml (new Line( start, end, pen, paintDevice ));
        gridLines.push_back(std::move(ml));
    }

    for( int y = 0; y < height; y += size )
    {/* Draw vertical lines */
        const QPoint start = QPoint(0, y);
        const QPoint end = QPoint(width, y);
        std::unique_ptr<Line> ml (new Line( start, end, pen, paintDevice ));
        gridLines.push_back(std::move(ml));
    }
}
void Grid::resize(const int &w, const int &h)
{/* Reconstructs grid according to new width and height*/
    width = w;
    height = h;
    construct();
}

void Grid::draw()
{/* Draws the grid lines */
    for(auto i = gridLines.begin(); i != gridLines.end(); ++i)
    {
        (*i)->draw();
    }
}
