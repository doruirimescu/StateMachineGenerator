#ifndef STATE_H
#define STATE_H

#include <QString>
#include <QtDebug>
#include <QPoint>
#include <QPen>
#include "component.h"

class State : public Component
{
public:
    State( QString label, int x, int y, QPen pen, QColor col, int radius ) : Component(label){ pos.setX(x); pos.setY(y); this->pen = pen; this->col = col; this->radius = radius;};
    ~State(){ qInfo()<<"dead"; }

    QPoint getPos();
    QPen getPen();
    QColor getCol();
    void print();
    void setLabel(QString label);
    int getRad();

private:
    QPen   pen; //for drawing borders
    QPoint pos; //for tracking position
    QColor col; //for drawing background
    int radius = 0; //radius of state
};

#endif // STATE_H
