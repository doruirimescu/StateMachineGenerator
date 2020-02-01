#include "component.h"


Component::Component( QString label)
{
    this->label = label;
}
Component::~Component()
{
}

void Component::print()
{
    qInfo()<<" Label"<<label;
}
QString Component::getLabel()
{
    return label;
}
