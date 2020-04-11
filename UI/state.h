#ifndef STATE_H
#define STATE_H

#include <QString>
#include <QtDebug>
#include <QPoint>
#include <QPen>
#include <QString>
#include <QLabel>
class State
{
public:
    State( QString label, QPoint pos, QPen pen, QColor color, int radius ) :  pen(pen), pos(pos), col(color), radius(radius), label(label){};
    ~State(){ qInfo()<<this->label<<"dead!"; }

    QPoint getPos();
    QPen getPen();
    QColor getCol();
    void print();
    void setLabel(QString label);
    QString getLabel();
    void setCode(QString code);
    QString getCode();
    int getRad();
    void setRad(int r);

private:
    QString code; //code that runs when the state outputs
    QPen   pen; //for drawing borders
    QPoint pos; //for tracking position
    QColor col; //for drawing background
    int radius = 0; //radius of state
    QString label;

};

#endif // STATE_H
