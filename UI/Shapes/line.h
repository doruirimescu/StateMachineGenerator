#ifndef LINE_H
#define LINE_H


class QPoint;
class QPen;
class QPaintDevice;

class Line
{
public:
    Line(const QPoint* start, const QPoint* end, QPen* pen, QPaintDevice* const &dev);
    void draw();
private:
    const QPoint* start;
    const QPoint* end;
    const QPen* pen;
    QPaintDevice *paintDevice;
};

#endif // LINE_H
