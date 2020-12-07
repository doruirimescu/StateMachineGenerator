#include "state.h"
#include<QApplication>
void State::print()
{
    qInfo()<<" x: "<<pos.x()<<" y:"<<pos.y();
}
