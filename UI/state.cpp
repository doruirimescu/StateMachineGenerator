#include "state.h"
void State::print()
{
    qInfo()<<" x: "<<pos.x()<<" y:"<<pos.y();
}
QPoint State::getPos()
{/* Get the state position */
    return pos;
}
QPen State::getPen()
{/* Get the state pen */
    return pen;
}
QColor State::getCol()
{/* Get the state color */
    return col;
}
int State::getRad()
{/* Get the state radius */
    return radius;
}
void State::setRad(int r)
{/* Set the state radius */
    radius = r;
}
void State::setLabel(QString label)
{/* Set the state label */
    this->label = label;
}
void State::setCode(QString code)
{/* Set the code that the state is supposed to be running */
    this->code = code;
}

QString State::getCode()
{/* Set the code that the state is supposed to be running  */
    return this->code;
}
