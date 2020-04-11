#ifndef SCRIBBLEAREA_H
#define SCRIBBLEAREA_H

#include <QColor>
#include <QImage>
#include <QPoint>
#include <QWidget>
#include "manager.h"
#include <QObject>
#include <PropertyHelper.h>
#include <QMouseEvent>
#include <QPainter>
#include <QtDebug>
#include <QtMath>
#include <QStaticText>
#include <QInputDialog>
#include <QApplication>
#include <QLabel>
#include "manager.h"
#include "maths.h"

#if defined(QT_PRINTSUPPORT_LIB)
#include <QtPrintSupport/qtprintsupportglobal.h>
#if QT_CONFIG(printdialog)
#include <QPrinter>
#include <QPrintDialog>

#endif
#endif

constexpr QPoint invalidPoint = QPoint(-1000, -1000);

class ScribbleArea : public QWidget
{
    Q_OBJECT
public:
    ScribbleArea(QWidget *parent = nullptr);
    ~ScribbleArea();
    void setPenColor(const QColor &newColor);
    void setStateColor(const QColor &newColor);
    void setPenWidth(int newWidth);
    int getGridSize();
    void setGridSize(int grid);
    int getStateRadius();
    void setStateRadius(int rad);

    bool isModified() const { return modified; }
    QColor penColor() const { return myPenColor; }
    int penWidth() const { return myPenWidth; }

    /* State variables for the menu */
    bool pState;        //If user is placing state
    bool pAction;       //If user is placing action
    bool pActionStart;  //If user is placing the startpoint of the action true, if endpoint is placed, false
    bool eState;        //If user is editing state
    bool eAction;       //If user is editing action

    /* Please, group these into a class. */
    State* actionStart;
    QPoint actionStartPoint;
    State* actionEnd;
    QPoint actionEndPoint;

public slots:
    void clearImage();
    void print();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    AUTO_PROPERTY(QString, myProperty);

    void resizeImage(QImage *image, const QSize &newSize);
    void drawGrid();
    void clearStates();
    void drawCircleTo(const QPoint &endPoint, int radius);
    void drawLine( const QPoint &start, const QPoint &end );
    void drawAnchor(const QPoint &endPoint);
    void drawActionLine( const QPoint &start, const QPoint & end );
    void drawState(State* s);
    void drawAction(Action* a);
    void drawArrow(int x0, int y0, int x, int y, int w, QPainter* painter);
    int roundToGrid(int);

    bool modified = false;
    bool scribbling = false;
    int myPenWidth = 2;
    QPoint prevPoint = QPoint(0,0); //Previous point where cursor was
    int gridSize  = 20;
    int circleRad = 60;

    Manager *m;

    QColor myPenColor = 0x3333FF;
    QColor myStateColor = 0xF5F5F5;
    QImage image;
    QPoint lastPoint;

    /* Used for drawing the arrow */
    QBrush brush;
    QPainterPath path;
};
//! [0]

#endif
