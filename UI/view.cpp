#include "view.h"

#include "Shapes/arrow.h"
#include "Shapes/circle.h"
#include "Shapes/line.h"
#include <action.h>
#include <state.h>
#include <QPair>

void View::drawAction(const Action *const a)
{/* Draw a full action that has been completed */
    drawAnchor(a->getEndPoint());
    drawAnchor(a->getStartPoint());
    drawActionLine(a);

    /* Draw action text */
    if( a->getSplitsSize() > 1 )
    {/* Need at least 2 splits to draw text in between them */
        drawActionText(a);
    }
}

void View::drawActionText(const Action *const a)
{

    auto splits = a->getSplits();
    auto begin = splits.first;
    QPoint split1 = *( begin + a->getSplitsSize() / 2 - 1 );
    QPoint split2 = *( begin + a->getSplitsSize() / 2 );

    /* Choose text position */
    qint16 textX = split1.x()+ ( split2.x() - split1.x() ) / 2;
    qint16 textY = split1.y() + ( split2.y() - split1.y() ) / 2;

    QPainter painter(&image);
    QBrush brush;
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(*a->getPen());

    QRectF rect = painter.boundingRect( textX, textY, 50, 50, Qt::AlignHCenter,  a->getLabel() );
    QRect r = QRect(textX - rect.width()/2 , textY, rect.width(), rect.height() );

    /* Set rectangle background color to white */
    brush.setColor( QColor( Qt::white) );
    painter.fillRect(r, brush);
    painter.drawRect(r);
    painter.setRenderHint(QPainter::TextAntialiasing);
    painter.drawStaticText(textX - rect.width()/2, textY, QStaticText( a->getLabel() ) ) ;
    painter.end();
}

void View::drawAnchor(const QPoint &endPoint)
{/* Draw anchor point where an action could join */
    Circle anchor( &endPoint, 5, &image, new QColor(0xB0E3E6), new QPen(QColor(0x3333FF), 2), 0.7 );
    anchor.clear();
    anchor.draw();
}

void View::drawPossibleAnchor(const QPoint &endPoint)
{/* Draw a possible anchor point where an action could join */

    QPen* pen = new QPen(QColor(0x3333FF), 2);
    Circle anchor( &endPoint, 6, &image, new QColor(Qt::white), pen, 0.5 );
    anchor.clear();
    anchor.draw();

}
void View::drawActionLine(const Action *const a)
{
    /* Get all the split points of the action */
    auto splits = a->getSplits();
    auto begin = splits.first;
    auto end = splits.second;

    /* Define the points needed for drawing an arrow */
    const QPoint endP       = *(end-1);//end point
    const QPoint lastSplit  = *(end-2);//last split
    const QPoint sLastSplit = *(end-3);//second last split


    /* Draw the line with all the splits */
    while( begin + 1 != end )
    {
        Line line(*begin, *(begin+1), a->getPen(), &image );
        line.draw();
        begin++;
    }

    if( a->getEndPoint() != invalidPoint )
    {/* Action has an end, draw arrow */
        /* Create an arrow pointing towards end, mind the arrow directions */
        int arrowLength = 10;
        int arrowWidth  = 10;
        int dirX = Maths::sign( endP.x() - lastSplit.x() );
        if( dirX != 0 )
        {/* Moving right dirX = 1, Moving left dirX = -1 */
            Arrow arrow( endP.x() - arrowLength * dirX  , lastSplit.y(), endP.x(), endP.y(), arrowWidth, &image );
            arrow.draw();
        }
        else
        {/* Moving up dirY = -1, Moving down dirY = 1 */
            int dirY = Maths::sign( endP.y() - sLastSplit.y() );
            Arrow arrow( lastSplit.x(), endP.y() - arrowLength * dirY, endP.x(), endP.y(), arrowWidth, &image );
            arrow.draw();
        }
    }

}
void View::drawPossibleActionLine(const QPoint &start, const QPoint &end, int gridSize, QPen *pen)
{
    /*
     * Draw a possible action line split into 2 parts.
     * The endpoint contains an arrow.
     */

    /* First split happens horizontally */
    QPoint split1 = QPoint( start.x() + Maths::roundToGrid( ( end.x() - start.x() ) / 2, gridSize ), start.y() );

    /* Handle exceptional case just as in ScribbleArea. */
    if( qAbs(end.x() - start.x()) == gridSize )
    {
        split1 = start;
    }

    /* Second split happens vertically */
    QPoint split2 = split1;
    split2.setY( end.y() );

    /* Draw lines */
    Line line1(start, split1, pen , &image );
    Line line2(split1, split2, pen , &image );
    Line line3(split2, end, pen , &image );
    line1.draw();
    line2.draw();
    line3.draw();

    /* Create an arrow pointing towards end, mind the arrow directions */

    /* Define arrow dimensions -> should be a property of each action for later customization ? */
    int arrowLength = gridSize / 2;
    int arrowWidth  = gridSize / 2;

    int dirX = Maths::sign( end.x() - split2.x() );

    if( dirX != 0 )
    {/* Moving right dirX = 1, Moving left dirX = -1 */
        Arrow arrow( end.x() - arrowLength * dirX  , split2.y(), end.x(), end.y(), arrowWidth, &image );
        arrow.draw();

    }
    else
    {/* Moving up dirY = -1, Moving down dirY = 1 */
        int dirY = Maths::sign( end.y() - start.y() );
        Arrow arrow( split2.x(), end.y() - arrowLength * dirY, end.x(), end.y(), arrowWidth, &image );
        arrow.draw();
    }
}

void View::drawState(const State *s)
{
    QPoint pos = s->getPos();

    QPen pen = s->getPen();
    QColor col = s->getCol();

    Circle circle(&pos, s->getRad(), &image, &col, &pen, 1.0);
    circle.draw();

    QPainter painter(&image);
    painter.setRenderHint(QPainter::Antialiasing);
    /* Get a bounding rectangle for the text */
    QString text;
    text.append( QString(s->getLabel() ) );
    QRectF rect = painter.boundingRect( s->getPos().x(), s->getPos().y(), 50, 50, Qt::AlignHCenter, text );
    painter.drawStaticText( s->getPos().x() - rect.width() / 2, s->getPos().y() - rect.height() / 2, QStaticText( text ) ) ;
    painter.end();
}
void View::drawStates(const QVector<State *> &states)
{
    for( const auto & s : states )
    {
        drawState(s);
    }
}
