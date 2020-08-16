#include "line.h"
#include<QPainter>
#include<QPoint>
#include<QPen>

Line::Line(const QPoint* start, const QPoint* end, QPen* pen, QPaintDevice* const &dev): start(start), end(end), pen(pen), paintDevice(dev){};

void Line::draw()
{
    QPainter painter(paintDevice);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(*pen);
    painter.drawLine(*start, *end);
    painter.end();
}
