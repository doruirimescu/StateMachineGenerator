#include "scribblearea.h"

#include <QMouseEvent>
#include <QPainter>
#include <QtDebug>
#include <QtMath>
#include <QStaticText>
#include <QInputDialog>
#include "manager.h"

#if defined(QT_PRINTSUPPORT_LIB)
#include <QtPrintSupport/qtprintsupportglobal.h>
#if QT_CONFIG(printdialog)
#include <QPrinter>
#include <QPrintDialog>

#endif
#endif

ScribbleArea::ScribbleArea(QWidget *parent)
    : QWidget(parent)
{
    setAttribute(Qt::WA_StaticContents);
    setAttribute((Qt::WA_MouseTracking));

    /* Set menu state variables */
    pState = false;
    pAction=false;
    eState = false;
    eAction = false;

    /*Create a manager*/
    m = new Manager();
}
ScribbleArea::~ScribbleArea()
{
    qInfo()<<"Scribble";
    delete m;
}

void ScribbleArea::setPenColor(const QColor &newColor)
{
    myPenColor = newColor;
}
void ScribbleArea::setStateColor(const QColor &newColor)
{
    myStateColor = newColor;
}
void ScribbleArea::setPenWidth(int newWidth)
{
    myPenWidth = newWidth;
}

void ScribbleArea::clearImage()
{
    m->states.clear();
    image.fill(qRgb(255, 255, 255));
    modified = true;
    update();
}

void ScribbleArea::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        lastPoint = event->pos();
        if( pState )
        {// State placement is done
            m->addState(new State("State",previousX, previousY, QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::SquareCap, Qt::RoundJoin), myStateColor, circleRad ));
            pState = false;
        }
        else if( eState )
        {//State editing is opened
            //If clicking inside a valid state
            State * s = m->searchState(event->pos());
            if( s != nullptr )
            {
                QString label = QInputDialog::getText(this, QString("Edit state label"), QString("New Label"),QLineEdit::Normal,QString(s->getLabel()),nullptr,0);
                s->setLabel(label);
            }
            eState = false;
        }
    }
}

void ScribbleArea::mouseMoveEvent(QMouseEvent *event)
{
    int x, y;
    if( event->pos().x() > previousX )
    {
        x = (int) ceil(event->pos().x()/gridSize) * gridSize;
    }
    else
    {
        x = (int) floor(event->pos().x()/gridSize) * gridSize;
    }
    if( event->pos().y() > previousY )
    {
        y = (int) ceil(event->pos().y()/gridSize) * gridSize;
    }
    else
    {
        y = (int) floor(event->pos().y()/gridSize) * gridSize;
    }
    if( pState )
    {
        //draw at new position
        drawCircleTo( QPoint(x,y) );
    }
    //update old positions
    previousX = x;
    previousY = y;
    drawGrid();
}

void ScribbleArea::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && scribbling)
    {
        scribbling = false;
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
    {//draw vertical lines
        drawLine( QPoint(x,0), QPoint(x,height()) );
    }
    for( int y = 0; y < height(); y += gridSize )
    {//draw vertical lines
        drawLine( QPoint(0,y), QPoint(width(),y) );
    }

    /* Draw all the states */
    for( const auto & s : m->states )
    {
        drawState(s);
    }
}

void ScribbleArea::drawLine(const QPoint &start, const QPoint &end)
{
    QPainter painter(&image);
    painter.setPen(QPen(QColor("black"), 1, Qt::SolidLine, Qt::RoundCap,
                        Qt::RoundJoin));
    painter.drawLine(start, end);
    update();
}
void ScribbleArea::drawState(State *s)
{
    QPainter painter(&image);

    /* Get state original pen */
    painter.setPen( s->getPen() );
    painter.setBrush( s->getCol() );
    painter.drawEllipse( s->getPos(), circleRad, circleRad );

    /* Get a bounding rectangle for the text */
    QString text;
    text.append( QString(s->getLabel() ) );
    QRectF rect = painter.boundingRect( s->getPos().x(), s->getPos().y(),50, 50, Qt::AlignHCenter, text );
    painter.drawStaticText( s->getPos().x() - rect.width()/2, s->getPos().y() - rect.height()/2, QStaticText( text ) ) ;

    update();
    painter.end();
}

void ScribbleArea::drawCircleTo(const QPoint &endPoint)
{
    QPainter painter(&image);

    //delete previous ellipse
    painter.setPen( QPen(QColor( 0xffffff ), myPenWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin) );
    painter.drawEllipse( QPoint(previousX, previousY), circleRad, circleRad );

    //draw current ellipse
    painter.setPen( QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin) );
    painter.drawEllipse( endPoint, circleRad, circleRad );

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
