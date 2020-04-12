#ifndef VIEW_H
#define VIEW_H

#include <QColor>
#include <QImage>
#include <PropertyHelper.h>
#include <QPainter>
#include <QtDebug>
#include <QStaticText>
#include <QObject>
#include "manager.h"
#include "maths.h"

class View : public QObject
{
    Q_OBJECT

/* Test slot */
public slots:
    void showOff(const QString &col){qInfo()<<"SHOWOFF!"<<col;};

public:
    View(QImage &img): image(img) {};

    /* State-related drawing */
    void drawState(const State *s);
    void drawCircleTo(const QPoint &endPoint);
    void deleteCircleFrom(const QPoint &endPoint);

    /* Action-related drawing */
    void drawAnchor(const QPoint &endPoint);
    void drawActionLine(const QPoint &start, const QPoint &end);
    void drawArrow(int x0, int y0, int x, int y, int w, QPainter *painter);
    void drawAction(const Action *a);

    /* Line used for grids */
    void drawLine(const QPoint &start, const QPoint &end);

    /* State radius setter-getter */
    int getStateRadius(){ return stateRadius; };
    void setStateRadius(int rad){ stateRadius = rad; };

    /* Grid size setter-getter*/
    int getGridSize() { return gridSize; };
    void setGridSize(int grid) { gridSize = grid; };

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
    int gridSize = 20;

    /* Used for drawing states */
    QColor penColor = 0x3333FF;
    int penWidth = 2;
    QColor stateColor = 0xF5F5F5;
    int stateRadius = 60;

    /* Used for drawing the action arrow */
    QBrush brush;
    QPainterPath path;
};

#endif // VIEW_H
