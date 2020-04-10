#include "scribblearea.h"

#include <QMouseEvent>
#include <QPainter>
#include <QtDebug>
#include <QtMath>
#include <QStaticText>
#include <QInputDialog>
#include <QApplication>
#include <QLabel>
#include "manager.h"

#if defined(QT_PRINTSUPPORT_LIB)
#include <QtPrintSupport/qtprintsupportglobal.h>
#if QT_CONFIG(printdialog)
#include <QPrinter>
#include <QPrintDialog>

#endif
#endif

/*
 * The area which reacts to user inputs
 */
ScribbleArea::ScribbleArea(QWidget *parent)
    : QWidget(parent)
{
    setAttribute(Qt::WA_StaticContents);
    setAttribute((Qt::WA_MouseTracking));

    /* Set menu state variables */
    pState  = false;
    pAction = false;
    eState  = false;
    eAction = false;

    actionStartPoint = invalidPoint;
    actionEndPoint   = invalidPoint;
    pActionStart = false;

    this->setProperty("myProperty","Property helper test");
    qInfo()<<this->myProperty();

    /* Create a manager */
    m = new Manager();
}
ScribbleArea::~ScribbleArea()
{
    qInfo()<<"Scribble";
    delete m;
}
int ScribbleArea::getStateRadius()
{/* Gets current state radius used to create states */
    return circleRad;
}
void ScribbleArea::setStateRadius( int rad )
{/* Sets new current state radius used to create states */
    this->circleRad = rad;
}
void ScribbleArea::setGridSize(int grid)
{/* Sets current grid size used to draw */
    this->gridSize = grid;
}
int ScribbleArea::getGridSize()
{/* Gets current grid size used to draw */
    return this->gridSize;
}
void ScribbleArea::setPenColor(const QColor &newColor)
{/* Gets current pen color used to draw state borders */
    myPenColor = newColor;
}
void ScribbleArea::setStateColor(const QColor &newColor)
{/* Sets new state background color */
    myStateColor = newColor;
}
void ScribbleArea::setPenWidth(int newWidth)
{/* Sets new pen width used to draw state borders */
    myPenWidth = newWidth;
}

void ScribbleArea::clearImage()
{
    image.fill(qRgb(255, 255, 255));
}
void ScribbleArea::clearStates()
{
    m->states.clear();
}

/* Mouse event methods */

void ScribbleArea::mousePressEvent(QMouseEvent *event)
{/* When user clicks */
    QApplication::restoreOverrideCursor();
    if (event->button() == Qt::LeftButton)
    {
        lastPoint = event->pos();
        if( pState && !m->intersectState(lastPoint) )
        {/* State placement is done */

            QString newLabel="";
            while( newLabel.compare("") ==0 )
            {
                newLabel = QInputDialog::getText(this, "State label", "Enter new state label");
            }

            m->addState(new State(newLabel, prevPoint,
                                  QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::SquareCap, Qt::RoundJoin), myStateColor, circleRad ));
            pState = false;
        }
        else if( eState )
        {/* State editing is opened */

            /* Try to find state at clicked position */
            State * s = m->searchState(event->pos());

            if( s != nullptr )
            {/* If clicking inside a valid state */
                QStringList editOptions;
                editOptions << "Edit label";
                editOptions << "Edit output code";
                editOptions << "Delete state";

                QInputDialog *dialog = new QInputDialog;
                dialog->setComboBoxEditable(false);

                QString chooseEdit = "Edit " + s->getLabel();
                QString option = dialog->getItem(this, chooseEdit, "What property do you want to edit?", editOptions, 0, false);

                qInfo() << option;
                if( option.compare(editOptions[0]) == 0)
                {/* edit label */
                    QString label = dialog->getText( this, "Edit label", "Enter the new value for the state label",QLineEdit::Normal, s->getLabel() );
                    s->setLabel( label );
                }
                else if( option.compare(editOptions[1]) == 0 )
                {/* Edit code */
                    QString code = dialog->getMultiLineText( this, "Edit code", "Enter the C++ code to \nrun on state output",s->getCode() );
                    s->setCode( code );
                }
                else if( option.compare(editOptions[2]) == 0 )
                {/* Delete state */
                    m->deleteState(s);
                    clearImage();
                }
            }
            eState = false;
        }
        else if( pAction == true )
        {/* If placing an action */
            if( pActionStart == false )
            {/* User wants to place the startpoint of the action */
                pActionStart = true;
                /* Create partial action*/
                m->addAction( new Action( actionStart, actionEnd, actionStartPoint, invalidPoint ) );
            }
            else if( pActionStart == true )
            {/* Action placement has finished, save action to state */
                Action* lastAct = m->getLastAction();
                lastAct->setEnd( actionEnd );
                lastAct->setEndPoint( &actionEndPoint );
                pActionStart = false;
                pAction = false;
                m->printActions();
            }
        }
    }
    drawGrid();
}

void ScribbleArea::mouseMoveEvent(QMouseEvent *event)
{
    int x, y;
    x = (int) round((float)event->pos().x()/(float)gridSize) * gridSize;
    y = (int) round((float)event->pos().y()/(float)gridSize) * gridSize;

    QPoint currentPoint = QPoint(x,y);
    clearImage();
    drawGrid();
    if( pState )
    {/* If moving mouse while placing state*/
        /* Draw at potentially new position */
        drawCircleTo( currentPoint, circleRad );

        /* Update old position with new position */
        prevPoint = currentPoint;
    }
    else if( pAction )
    {/* If placing first part of action */
        QString posInfo;
        QPoint possibleAnchorPoint = m->onStateBorder( event->pos(), posInfo );
        if( possibleAnchorPoint != invalidPoint )
        {
            /* If cursor is at any valid anchor point */
            drawAnchor( possibleAnchorPoint );

            if( pActionStart == false )
            {/* The user is placing an action startpoint */
                /* Save current state to the startpoint of the action */
                actionStart = m->searchState(possibleAnchorPoint);
                actionStartPoint = possibleAnchorPoint;
            }
            else if( pActionStart == true )
            {/* The user is placing an action endpoint */
                actionEnd = m->searchState(possibleAnchorPoint);
                actionEndPoint = possibleAnchorPoint;
                drawActionLine(actionStartPoint, possibleAnchorPoint);
            }
        }
        else if( pActionStart == true )
        {/* The user has placed the first anchor point */
            drawActionLine(actionStartPoint, currentPoint);
        }
    }
}


void ScribbleArea::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QRect dirtyRect = event->rect();
    painter.drawImage(dirtyRect, image, dirtyRect);
    painter.end();
}

void ScribbleArea::resizeEvent(QResizeEvent *event)
{
    if (width() > image.width() || height() > image.height()) {
        int newWidth = qMax(width() , image.width());
        int newHeight = qMax(height(), image.height());

        newWidth = (int)(newWidth / gridSize ) * gridSize;
        newHeight = (int)(newHeight / gridSize ) * gridSize;

        resizeImage(&image, QSize(newWidth, newHeight));
        update();
        qInfo()<<"RESIZE"<<image.width();
    }
    QWidget::resizeEvent(event);
    drawGrid();
}


/* Drawing methods */
void ScribbleArea::drawGrid()
{
    /* Every mouse update, redraw the whole thing */
    for( int x = 0; x < width(); x += gridSize )
    {/* Draw horizontal lines */
        drawLine( QPoint(x,0), QPoint(x,height()) );
    }
    for( int y = 0; y < height(); y += gridSize )
    {/* Draw vertical lines */
        drawLine( QPoint(0,y), QPoint(width(),y) );
    }

    /* Draw all the states */
    for( const auto & s : m->states )
    {
        drawState(s);
    }

    /* Draw all the actions */
    for( const auto & a: m->actions )
    {
        drawAction(a);
    }
}

void ScribbleArea::drawLine(const QPoint &start, const QPoint &end)
{
    QPainter painter(&image);
    painter.setPen(QPen(QColor(0xE0E0E0), 1.1, Qt::SolidLine, Qt::RoundCap,
                        Qt::RoundJoin));
    painter.drawLine(start, end);
    update();
}

void ScribbleArea::drawActionLine(const QPoint &start, const QPoint &end)
{
    /*
     * Draw an action line split into 2 parts.
     */
    QPainter painter(&image);
    painter.setPen(QPen(QColor(0x000001), 1.1, Qt::SolidLine, Qt::RoundCap,
                        Qt::RoundJoin));

    QPoint split1 = QPoint( start.x() + ( end.x() - start.x() ) / 2, start.y() );
    QPoint split2 = split1;
    split2.setY(end.y());
    painter.drawLine(start, split1 );
    painter.drawLine(split1, split2);
    painter.drawLine(split2, end);
    update();
}
void ScribbleArea::drawState(State *s)
{
    QPainter painter(&image);
    /* Get state original pen */
    painter.setPen( s->getPen() );
    painter.setBrush(s->getCol());
    painter.drawEllipse( s->getPos(), s->getRad(), s->getRad() );

    /* Get a bounding rectangle for the text */
    QString text;
    text.append( QString(s->getLabel() ) );
    QRectF rect = painter.boundingRect( s->getPos().x(), s->getPos().y(),50, 50, Qt::AlignHCenter, text );

    painter.drawStaticText( s->getPos().x() - rect.width()/2, s->getPos().y() - rect.height()/2, QStaticText( text ) ) ;
    update();
    painter.end();
}

void ScribbleArea::drawAction(Action *a)
{
    if( a->getEndPoint() == invalidPoint )
    {
        drawAnchor(a->getStartPoint());
    }
    else
    {
        drawAnchor(a->getStartPoint());
        drawAnchor(a->getEndPoint());
        drawActionLine( a->getStartPoint(), a->getEndPoint());
    }
}
void ScribbleArea::drawCircleTo(const QPoint &endPoint, int radius)
{
    QPainter painter(&image);

    /* Delete previous ellipse */
    painter.setPen( QPen(QColor( 0xffffff ), myPenWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin) );
    painter.setBrush(QColor(0xffffff));
    painter.drawEllipse( prevPoint, radius, radius );

    /* Draw current ellipse */
    painter.setPen( QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin) );
    painter.setBrush(myStateColor);
    painter.setOpacity(0.2);
    painter.drawEllipse( endPoint, radius, radius );

    update();
    painter.end();
}

void ScribbleArea::drawAnchor(const QPoint &endPoint)
{
    QPainter painter(&image);

    /* Draw ellipse */
    painter.setPen( QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin) );
    painter.setBrush(QColor(0x33001A));
    painter.drawEllipse( endPoint, 10, 10 );

    update();
    painter.end();
}

void ScribbleArea::resizeImage(QImage *image, const QSize &newSize)
{
    if (image->size() == newSize)
        return;

    QImage newImage(newSize, QImage::Format_RGB32);
    newImage.fill(qRgb(255, 255, 255));
    QPainter painter(&newImage);
    painter.drawImage(QPoint(0, 0), *image);
    *image = newImage;
}

void ScribbleArea::print()
{
#if QT_CONFIG(printdialog)
    QPrinter printer(QPrinter::HighResolution);

    QPrintDialog printDialog(&printer, this);
//! [21] //! [22]
    if (printDialog.exec() == QDialog::Accepted) {
        QPainter painter(&printer);
        QRect rect = painter.viewport();
        QSize size = image.size();
        size.scale(rect.size(), Qt::KeepAspectRatio);
        painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
        painter.setWindow(image.rect());
        painter.drawImage(0, 0, image);
    }
#endif // QT_CONFIG(printdialog)
}
//! [22]
