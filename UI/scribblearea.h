#ifndef SCRIBBLEAREA_H
#define SCRIBBLEAREA_H

#include <QColor>
#include <QImage>
#include <QPoint>
#include <QWidget>
#include "manager.h"
#include <QObject>
#include <PropertyHelper.h>
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
    bool pState;    //If user is placing state
    bool pAction;   //If user is placing action
    bool eState;    //If user is editing state
    bool eAction;   //If user is editing action

public slots:
    void clearImage();
    void print();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    //void mouseReleaseEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    AUTO_PROPERTY(QString, myProperty);
    void drawLine( const QPoint &start, const QPoint &end );
    void resizeImage(QImage *image, const QSize &newSize);
    void drawGrid();
    void clearStates();
    void drawCircleTo(const QPoint &endPoint, int radius);
    void drawAnchor(const QPoint &endPoint);
    void drawState(State* s);

    bool modified = false;
    bool scribbling = false;
    int myPenWidth = 2;
    QPoint prevPoint = QPoint(0,0);
    int gridSize  = 20;
    int circleRad = 60;

    Manager *m;

    QColor myPenColor = 0x3333FF;
    QColor myStateColor = 0xF5F5F5;
    QImage image;
    QPoint lastPoint;
};
//! [0]

#endif
