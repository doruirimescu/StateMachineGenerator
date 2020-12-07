#ifndef LINE_H
#define LINE_H

#include<QPoint>
#include "shapeinterface.h"

class QPen;
class QPaintDevice;
class Line : protected ShapeInterface
{
public:
    static int ctr;
    Line(const QPoint start, const QPoint end, QPen *pen, QPaintDevice *const &dev);
    ~Line(){};

    void draw()override;
private:
    QPoint start;
    QPoint end;
};
#endif // LINE_H

