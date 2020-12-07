#include "Shapes/line.h"
#include "scribblearea.h"
#include<manager.h>
#include <QSet>
#include <QMouseEvent>
/*
 * The area which reacts to user inputs
 */


ScribbleArea::ScribbleArea(QWidget *parent) : QWidget(parent)
{
    grid = new Grid( new QPen(QColor(0xE0E0E0), 1.1, Qt::SolidLine, Qt::RoundCap,
                                               Qt::RoundJoin), 1.0, width(), height(), 20, &image);
    setAttribute(Qt::WA_StaticContents);
    setAttribute((Qt::WA_MouseTracking));

    view = new View(image);
    /* Set menu state variables */
    pState  = false;
    pAction = false;
    eState  = false;
    eAction = false;
    mState  = false;

    actionStartPoint = invalidPoint;
    actionEndPoint   = invalidPoint;
    pActionStart     = false;

    movingState = nullptr;

    /* Create a manager */
    m = new Manager();

    actionPen = new QPen(QPen(QColor(0x000001), 1.1, Qt::SolidLine, Qt::RoundCap,
                              Qt::RoundJoin));
}

ScribbleArea::~ScribbleArea()
{
    delete m;
    delete view;
    delete grid;
}

void ScribbleArea::clearStates()
{/* Clear all states */
    m->states.clear();
}

void ScribbleArea::generateCode()
{
    QString filename="DataR.txt";
    QFile file( filename );
    if ( file.open(QIODevice::ReadWrite) )
    {
        QTextStream stream( &file );

        QSet<QString> labels;
        for( auto const & a : m->actions )
        {
           labels.insert(a->getLabel());
        }
        stream<<"#Label\t\t\t\t";
        for( auto const & l : labels )
        {
             stream<<l<<"\t\t\t\t";
        }
        stream<<endl;

        int ctrL = 0, ctrS = 0;
        stream<<"[";
        for( auto const &s : m->states )
        {
            ctrS ++;
            stream<<"( \""<<s->getLabel()<<"\"";
            ctrL = 0;
            for( auto const &l : labels )
            {
                bool found = false;
                for( auto const &a : m->actions )
                {
                    if( a->getLabel().compare( l ) == 0 )
                    {
                        if( a->getStart() == s )
                        {
                            stream<<",\""<<a->getEnd()->getLabel()<<"\"";
                            found = true;
                            break;
                        }
                    }
                }
                if( !found )
                {
                    stream<<",-1";
                }
                ctrL++;
                if( ctrL < labels.size() )
                {
                    stream<<"\t\t";
                }
            }
            if( ctrS < m->states.size() )
            {
                stream<<"),\n";
            }
            else
            {
                stream<<",)";
            }
        }
        stream<<"]";
    }
    file.close();
    qInfo()<<"Generating code";
}


bool ScribbleArea::setGridSize(int size) const
{/* Cannot set a grid size which is larger than the smallest state radius\n, smaller than 10 or not a multiple of 10 !!! */
    if( m->getSmallestStateRadius() > size && size >= 10 && size % 10 == 0)
    {
        grid->setSize(size);
        return true;
    }
    return false;
}

void ScribbleArea::rearrangeActions()
{
    m->Astar( getGridSize(), width(), height() );
    drawGrid();
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
    if (width() > image.width() || height() > image.height())
    {
        int newWidth = qMax(width() , image.width());
        int newHeight = qMax(height(), image.height());

        newWidth  = Maths::roundToGrid(newWidth, getGridSize());
        newHeight = Maths::roundToGrid(newHeight, getGridSize());

        resizeImage(&image, QSize(newWidth, newHeight));
        update();
        qInfo()<<"RESIZE"<<image.width();
    }
    QWidget::resizeEvent(event);
    grid->resize( width(), height() );
    drawGrid();
}

void ScribbleArea::drawGrid()
{
    view->clearImage();
    /* Every mouse update, redraw the whole thing */
    grid->draw();

    view->drawStates(m->states);

    if( !mState )
    {
        for( const auto & a: m->actions )
        {
            view->drawAction(a);
        }
    }
    update();
}
void ScribbleArea::boundToDrawingArea()
{
    if( currentPoint.x() - getStateRadius() < getGridSize() )
    {/* If the cursor is out of bounds, limit it inside drawing area*/
        int pos = getGridSize() + getStateRadius();
        currentPoint.setX( Maths::roundToGrid( pos, getGridSize()) );
    }
    if( currentPoint.y() - getStateRadius() < getGridSize() )
    {/* If the cursor is out of bounds, limit it inside drawing area*/
        int pos = getGridSize() + getStateRadius();
        currentPoint.setY( Maths::roundToGrid( pos, getGridSize()) );
    }

    if( currentPoint.x() + getStateRadius() > width() - getGridSize() )
    {/* If the cursor is out of bounds, limit it inside drawing area*/
        int pos = width() - getStateRadius() - getGridSize();
        currentPoint.setX( Maths::roundToGrid( pos, getGridSize()) );
    }

    if( currentPoint.y() + getStateRadius() > height() - getGridSize() )
    {/* If the cursor is out of bounds, limit it inside drawing area*/
        int pos = height() - getStateRadius() - getGridSize();
        currentPoint.setY( Maths::roundToGrid( pos, getGridSize()) );
    }
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
    qDebug()<<"PRINT";
#if QT_CONFIG(printdialog)
    QPrinter printer(QPrinter::HighResolution);

    QPrintDialog printDialog(&printer, this);
//! [21] //! [22]
    if (printDialog.exec() == QDialog::Accepted)
    {
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
