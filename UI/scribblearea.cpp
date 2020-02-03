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

    label = new QLabel(this);
    label->setText("this is the first\nqlabel");
    label->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    label->setWordWrap(true);
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
    QApplication::restoreOverrideCursor();
    if (event->button() == Qt::LeftButton)
    {
        lastPoint = event->pos();
        if( pState && !m->intersectState(lastPoint) )
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
                QStringList editOptions;
                editOptions << "Edit label";
                editOptions << "Edit output code";

                QInputDialog *dialog = new QInputDialog;
                dialog->setComboBoxEditable(false);

                QString chooseEdit = "Edit " + s->getLabel();
                QString option = dialog->getItem(this, chooseEdit, "What property do you want to edit?", editOptions, 0, false);

                qInfo() << option;
                if( option.compare(editOptions[0]) == 0)
                {//edit label
                    QString label = dialog->getText( this, "Edit label", "Enter the new value for the state label",QLineEdit::Normal, s->getLabel() );
                    s->setLabel( label );
                }
                else if( option.compare(editOptions[1]) == 0 )
                {//edit code
                    QString code = dialog->getMultiLineText( this, "Edit code", "Enter the C++ code to \nrun on state output",s->getCode() );
                    s->setCode( code );
                }
            }
            eState = false;
        }
    }
}

void ScribbleArea::mouseMoveEvent(QMouseEvent *event)
{
    int x, y;
    x = (int) round((float)event->pos().x()/(float)gridSize) * gridSize;
    y = (int) round((float)event->pos().y()/(float)gridSize) * gridSize;

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
