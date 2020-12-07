#include "shapeinterface.h"
#include <QPen>
#include <QPaintDevice>

ShapeInterface::ShapeInterface(QPen *pen, QPaintDevice* const &device) : pen(pen), paintDevice(device)
{
};
