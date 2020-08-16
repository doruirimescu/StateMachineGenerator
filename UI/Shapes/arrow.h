#ifndef ARROW_H
#define ARROW_H

class QPaintDevice;
class QPoint;

class Arrow
{
private:
    QPoint *start, * end, *right, *left;
    QPaintDevice *paintDevice;
public:
    Arrow( const int x0, const int y0, const int x, const int y, const int w, QPaintDevice* const &pdev );
    void draw();
};

#endif // ARROW_H
