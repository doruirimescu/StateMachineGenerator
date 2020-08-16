#ifndef SCRIBBLEAREA_H
#define SCRIBBLEAREA_H

#include <QApplication>
#include <QWidget>
#include <QLabel>
#include "view.h"

#if defined(QT_PRINTSUPPORT_LIB)
#include <QtPrintSupport/qtprintsupportglobal.h>
#if QT_CONFIG(printdialog)
#include <QPrinter>
#include <QPrintDialog>

#include "Shapes/grid.h"
#endif
#endif

class Manager;
class ScribbleArea : public QWidget
{
    Q_OBJECT
public:
    /* Constructor-destructor */
    ScribbleArea(QWidget *parent = nullptr);
    ~ScribbleArea();

    bool isModified() const { return modified; }

    /* State radius setter-getter */
    int getStateRadius() const { return view->getStateRadius(); };
    void setStateRadius(int rad) const { this->view->setStateRadius(rad); };

    /* Grid size setter-getter*/
    bool setGridSize(int size) const;
    int getGridSize() const {return grid->getSize();};

    /* Pen color setter-getter */
    QColor getPenColor() const { return view->getPenColor(); }
    void setPenColor(const QColor &newColor) const{ this->view->setPenColor(newColor); };

    /* State color setter-getter*/
    void setStateColor(const QColor &newColor) const { view->setStateColor(newColor); };
    QColor getStateColor() const{ return view->getStateColor();}

    /* Pen width setter-getter */
    void setPenWidth(int newWidth) const {view->setPenWidth(newWidth);}
    int getPenWidth() const { return view->getPenWidth(); }

    /* Called for code generation */
    void generateCode();

    /* Called for A* action rearrangement */
    void rearrangeActions();

    /* State variables for the menu */
    bool pState;        //If user is placing state
    bool mState;        //If user is moving state
    bool pAction;       //If user is placing action
    bool pActionStart;  //If user is placing the startpoint of the action true, if endpoint is placed, false
    bool eState;        //If user is editing state
    bool eAction;       //If user is editing action

    /* Please, group these into a class. */
    State* actionStart;
    QPoint actionStartPoint;
    State* actionEnd;
    QPoint actionEndPoint;
    QString actionStartAnchor;
    QString actionEndAnchor;
    State *movingState;

    /* Used for managing the grid */
    Grid *grid;


public slots:
    void print();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    void resizeImage(QImage *image, const QSize &newSize);
    void drawGrid();
    void drawActions();
    void drawStates();

    void clearStates();
    void updateCurrentPoint(int x, int y) { currentPoint = QPoint(x, y); };
    void boundToDrawingArea();

    bool modified = false;
    bool scribbling = false;

    Manager *m;
    QImage image;
    QPoint currentPoint = QPoint(0, 0);
    View * view;

    QPen *actionPen;
};
//! [0]

#endif
