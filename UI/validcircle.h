#ifndef VALIDCIRCLE_H
#define VALIDCIRCLE_H

#include "circle.h"
class ValidCircle : public Circle
{
public:
    ValidCircle(const QPoint* const &center, int radius, QPaintDevice* const &dev, QColor* const &fillColor, QColor* const &borderColor, int borderWidth) :
        Circle(center, radius, dev, fillColor, borderColor, borderWidth){};
    void draw() override;
    void clear() override;
};

#endif // VALIDCIRCLE_H
