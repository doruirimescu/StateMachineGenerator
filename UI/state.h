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
    /* Constructor-destructor */
    State( QString label, QPoint pos, QPen pen, QColor color, int radius ) :  pen(pen), pos(pos), col(color), radius(radius), label(label){};
    ~State(){ qInfo()<<this->label<<"dead!"; }

    void print();

    /* State position setter-getter */
    void setPos(QPoint position) { pos = position; };
    QPoint getPos() const { return pos; };

    /* State pen setter-getter */
    void setPen(QPen p){ pen = p; };
    QPen getPen() const { return pen; };

    /* State color setter-getter */
    void setCol(QColor c){ col = c; };
    QColor getCol() const { return col; };

    /* Set label setter-getter */
    void setLabel(QString l){ label = l; };
    QString getLabel() const { return label; };

    /* State code setter-getter */
    void setCode(QString c){ code = c; };
    QString getCode() const { return code; };

    /* State radius setter-getter */
    void setRad(int r){ radius = r; };
    int getRad() const { return radius; };

    void bonundToDrawingArea( int width, int height, int gridSize);
private:
    QString code; //code that runs when the state outputs
    QPen   pen; //for drawing borders
    QPoint pos; //for tracking position
    QColor col; //for drawing background
    int radius = 0; //radius of state
    QString label;
};

#endif // STATE_H
