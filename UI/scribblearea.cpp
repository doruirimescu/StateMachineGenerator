#include "scribblearea.h"



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
{/* Clear all states */
    m->states.clear();
}

int ScribbleArea::roundToGrid(int x)
{
    return (int) round( (float)x / (float)gridSize ) * gridSize;
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
     * The endpoint contains an arrow.
     */
    QPainter painter(&image);
    painter.setPen(QPen(QColor(0x000001), 1.1, Qt::SolidLine, Qt::RoundCap,
                        Qt::RoundJoin));

    /* First split happens horizontally */
    QPoint split1 = QPoint( start.x() + roundToGrid(( end.x() - start.x() ) / 2 ), start.y() );

    /* Second split happens vertically */
    QPoint split2 = split1;
    split2.setY(end.y());

    /* Draw lines */
    painter.drawLine(start, split1 );
    painter.drawLine(split1, split2);
    painter.drawLine(split2, end);

    /* Create an arrow pointing towards end, mind the arrow directions */

    /* Define arrow dimensions -> should be a property of each action for later customization ? */
    int arrowLength = gridSize;
    int arrowWidth = gridSize;

    int dirX = Maths::sign( end.x() - split2.x() );
    if( dirX != 0 )
    {/* Moving right dirX = 1, Moving left dirX = -1 */
        drawArrow( end.x() - arrowLength * dirX  , split2.y(), end.x(), end.y(), arrowWidth, &painter);
    }
    else
    {/* Moving up dirY = -1, Moving down dirY = 1 */
        int dirY = Maths::sign( end.y() - start.y() );
        drawArrow( split2.x(), end.y() - arrowLength * dirY, end.x(), end.y(), arrowWidth, &painter);
    }
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
    painter.drawEllipse( endPoint, 5, 5 );

    update();
    painter.end();
}

void ScribbleArea::drawArrow(int x0, int y0, int x, int y, int w, QPainter* painter)
{
    int dx, dy;
    double sina, cosa, l;
    l = qSqrt( (x - x0) * (x - x0)  + (y - y0) * (y - y0) );
    sina = (y-y0)/l;
    cosa = (x-x0)/l;
    dx = ( (sina * w) / 2 );
    dy = ( (cosa * w) / 2 );


    QPoint start= QPoint(x0, y0);
    QPoint end  = QPoint(x, y);
    QPoint right= QPoint(dx + x0, (dy + y0) );
    QPoint left = QPoint(x0 - dx, (y0 - dy) );

    QPolygon poly;
    poly<< start;
    poly<<right;
    poly<< end;
    poly<<left;

    path.clear();
    brush.setColor(Qt::black);
    brush.setStyle(Qt::SolidPattern);
    path.addPolygon(poly);
    painter->drawPolygon(poly);
    painter->fillPath(path, brush);

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
