#ifndef VIEW_H
#define VIEW_H

#include <QColor>
#include <QImage>
#include <PropertyHelper.h>
#include <QPainter>
#include <QtDebug>
#include <QStaticText>
#include <QObject>

#include "maths.h"

class Action;
class State;
class View : public QObject
{
    Q_OBJECT

/* Test slot */
public slots:
    void showOff(const QString &col){qInfo()<<"SHOWOFF!"<<col;};

public:
    View(QImage &img): image(img) {};

    void clearImage(){ image.fill(qRgb(255, 255, 255)); };

    /* State-related drawing */
    void drawState(const State *s);
    void drawStates(const QVector<State*> &states);

    /* Action-related drawing */
    void drawAnchor(const QPoint &endPoint);
    void drawPossibleAnchor(const QPoint &endPoint);
    void drawPossibleActionLine(const QPoint &start, const QPoint &end, int gridSize, QPen * pen);
    void drawAction(const Action *const a);
    void drawActionText(const Action *const a);
    void drawActionLine(const Action *const a);//in development

    /* State radius setter-getter */
    int getStateRadius(){ return stateRadius; };
    void setStateRadius(int rad){ stateRadius = rad; };

    /* Pen color setter-getter */
    QColor getPenColor() { return penColor; };
    void setPenColor(QColor col) { penColor = col; };

    /* State color setter-getter*/
    QColor getStateColor() { return stateColor; };
    void setStateColor(QColor col) { stateColor = col; };

    /* Pen width setter-getter */
    int getPenWidth() { return penWidth; };
    void setPenWidth(int width) { penWidth = width; };

private:
    QImage &image;

    /* Used for drawing states */
    QColor penColor = 0x3333FF;
    int penWidth = 2;
    QColor stateColor = 0xF5F5F5;
    int stateRadius = 60;

};

#endif // VIEW_H
