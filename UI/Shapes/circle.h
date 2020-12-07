#ifndef CIRCLE_H
#define CIRCLE_H
class QPoint;
class QPaintDevice;
class QPen;
class QColor;

class Circle
{
public:
    Circle(const QPoint* const &center, int radius, QPaintDevice* const &dev, QColor *fillColor, QPen *pen, const double opacity);
    void draw();
    void clear();

protected:
    const QPoint *center;
    const int radius;
    QPaintDevice *paintDevice;
    QColor *fillColor;
    QPen *pen;
    const double opacity;
};

#endif // CIRCLE_H
