#include "state.h"
void State::print()
{
    qInfo()<<" x: "<<pos.x()<<" y:"<<pos.y();
}
QPoint State::getPos()
{//Getter for state position
    return pos;
}
QPen State::getPen()
{//Getter for state pen
    return pen;
}
QColor State::getCol()
{//Getter for state color
    return col;
}
int State::getRad()
{
    return radius;
}
void State::setRad(int r)
{
    radius = r;
}
void State::setLabel(QString label)
{
    this->label = label;
}
void State::setCode(QString code)
{
    this->code = code;
}

QString State::getCode()
{
    return this->code;
}
