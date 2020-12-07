#ifndef SHAPEINTERFACE_H
#define SHAPEINTERFACE_H

class QPen;
class QPaintDevice;

class ShapeInterface
{
    /* Abstract class to be inherited by other shapes */
public:
    ShapeInterface(QPen *pen, QPaintDevice* const &device);
    virtual void draw() = 0;

protected:
    QPen *pen;
    QPaintDevice *paintDevice;
};

#endif // SHAPEINTERFACE_H
