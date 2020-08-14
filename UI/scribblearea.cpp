#include "scribblearea.h"
#include <QSet>
/*
 * The area which reacts to user inputs
 */
ScribbleArea::ScribbleArea(QWidget *parent) : QWidget(parent)
{
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
    drawGrid();
}

void ScribbleArea::drawGrid()
{
    view->clearImage();
    /* Every mouse update, redraw the whole thing */
    for( int x = 0; x < width(); x += view->getGridSize() )
    {/* Draw horizontal lines */
        view->drawLine( QPoint(x,0), QPoint(x,height()) );
    }

    for( int y = 0; y < height(); y += view->getGridSize() )
    {/* Draw vertical lines */
        view->drawLine( QPoint(0, y), QPoint(width(), y) );
    }

    view->drawStates(m->states);

    if( !mState )
    {
        view->drawActions(m->actions);
    }
    update();
}

void ScribbleArea::boundToDrawingArea()
{
    if( currentPoint.x() - getStateRadius() < getGridSize() )
    {/* If the cursor is out of bounds, limit it inside drawing area*/
        currentPoint.setX( getGridSize() + getStateRadius() );
    }
    if( currentPoint.y() - getStateRadius() < getGridSize() )
    {/* If the cursor is out of bounds, limit it inside drawing area*/
        currentPoint.setY( getGridSize() + getStateRadius() );
    }

    if( currentPoint.x() + getStateRadius() > width() - getGridSize() )
    {/* If the cursor is out of bounds, limit it inside drawing area*/
        currentPoint.setX( width() - getStateRadius() - getGridSize() );
    }

    if( currentPoint.y() + getStateRadius() > height() - getGridSize() )
    {/* If the cursor is out of bounds, limit it inside drawing area*/
        currentPoint.setY( height() - getStateRadius() - getGridSize() );
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
