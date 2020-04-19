#ifndef SCRIBBLEAREA_H
#define SCRIBBLEAREA_H

#include <QInputDialog>
#include <QApplication>
#include <QWidget>
#include <QMouseEvent>
#include <QLabel>
#include "view.h"

#if defined(QT_PRINTSUPPORT_LIB)
#include <QtPrintSupport/qtprintsupportglobal.h>
#if QT_CONFIG(printdialog)
#include <QPrinter>
#include <QPrintDialog>

#endif
#endif


class ScribbleArea : public QWidget
{
    Q_OBJECT
public:
    /* Constructor-destructor */
    ScribbleArea(QWidget *parent = nullptr);
    ~ScribbleArea(){ delete m; delete view;};

    bool isModified() const { return modified; }

    /* State radius setter-getter */
    int getStateRadius() const { return view->getStateRadius(); };
    void setStateRadius(int rad) const { this->view->setStateRadius(rad); };

    /* Grid size setter-getter*/
    void setGridSize(int grid) const {this->view->setGridSize(grid);};
    int getGridSize() const {return this->view->getGridSize();};

    /* Pen color setter-getter */
    QColor getPenColor() const { return view->getPenColor(); }
    void setPenColor(const QColor &newColor) const{ this->view->setPenColor(newColor); };

    /* State color setter-getter*/
    void setStateColor(const QColor &newColor) const { view->setStateColor(newColor); };
    QColor getStateColor() const{ return view->getStateColor();}

    /* Pen width setter-getter */
    void setPenWidth(int newWidth) const {view->setPenWidth(newWidth);};
    int getPenWidth() const { return view->getPenWidth(); }

    /* Called for code generation */
    void generateCode();

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
    void drawArrow(int x0, int y0, int x, int y, int w, QPainter* painter);

    bool modified = false;
    bool scribbling = false;

    Manager *m;
    QImage image;
    QPoint currentPoint = QPoint(0, 0);

    View * view;
};
//! [0]

#endif
