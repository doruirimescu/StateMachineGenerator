#ifndef GRID_H
#define GRID_H


class QPen;
class QPoint;
class QPaintDevice;

class Grid
{
public:
    Grid( QPen* const &pen, double opacity, int width, int height, int size, QPaintDevice* const &dev);
    void draw();
    void setPen(QPen* const &p){ pen = p;};
    void setOpacity(const double opac){opacity = opac;};
    void setSize(int sz){ size = sz;};
    int getSize()const{ return size; };
    void resize(const int &w, const int &h){ width = w; height = h;};

private:
    QPen* pen;
    double opacity;
    int width;
    int height;
    int size;
    QPaintDevice* paintDevice;
};

#endif // GRID_H
