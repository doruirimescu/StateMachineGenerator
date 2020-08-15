#ifndef ARROW_H
#define ARROW_H

#include<QPainter>
class Arrow
{
private:
    QPoint start, end, right, left;
    QPainter *painter;
public:
    Arrow(int x0, int y0, int x, int y, int w, QPainter *painter);
    void draw();
};

#endif // ARROW_H
