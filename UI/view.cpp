#include "view.h"



void View::drawLine(const QPoint &start, const QPoint &end)
{
    QPainter painter(&image);
    painter.setPen(QPen(QColor(0xE0E0E0), 1.1, Qt::SolidLine, Qt::RoundCap,
                        Qt::RoundJoin));
    painter.drawLine(start, end);
}

void View::drawAction( Action *a)
{/* Draw a full action that has been completed */
    drawAnchor(a->getEndPoint());
    drawAnchor(a->getStartPoint());
    drawActionLineDev(a);

    /* Draw action text */
    QPainter painter(&image);
    painter.setPen(QPen(QColor(0x1000E0), 1.1, Qt::SolidLine, Qt::RoundCap,
                        Qt::RoundJoin));
    QVector<QPoint>::iterator begin;
    QVector<QPoint>::iterator end;
    a->getSplits(begin, end);
    QPoint split1 = *( begin + a->getSplitsSize() / 2 - 1 );
    QPoint split2 = *( begin + a->getSplitsSize() / 2 );

    /* Choose text position */
    qint16 textX = split1.x()+ ( split2.x() - split1.x() ) / 2;
    qint16 textY = split1.y() + ( split2.y() - split1.y() ) / 2;

    QRectF rect = painter.boundingRect( textX, textY, 50, 50, Qt::AlignHCenter,  a->getLabel() );
    QRect r = QRect(textX - rect.width()/2 , textY, rect.width(), rect.height() );
    /* Set rectangle background color to white */
    brush.setColor( QColor( Qt::white) );
    painter.fillRect(r, brush);
    painter.drawRect(r);
    painter.drawStaticText(textX - rect.width()/2, textY, QStaticText( a->getLabel() ) ) ;
}

void View::drawAnchor(const QPoint &endPoint)
{/* Draw anchor point where an action could join */
    QPainter painter(&image);
    /*Remove previous point*/
    painter.setBrush( ( QColor(Qt::white) ) );
    painter.drawEllipse(endPoint, 5, 5 );

    painter.setPen( QPen( QColor(0x3333FF), 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin) );
    painter.setBrush( QColor(0xB0E3E6) );
    painter.setOpacity(0.7);
    painter.drawEllipse(endPoint, 5, 5 );
    painter.end();
}

void View::drawActionLineDev( Action *a)
{
    /* Get all the split points of the action */
    QVector<QPoint>::iterator begin;
    QVector<QPoint>::iterator end;
    a->getSplits(begin, end);

    /* Define the points needed for drawing an arrow */
    const QPoint endP       = *(end-1);//end point
    const QPoint lastSplit  = *(end-2);//last split
    const QPoint sLastSplit = *(end-3);//second last split

    QPainter painter(&image);
    painter.setPen(QPen(QColor(0x000001), 1.1, Qt::SolidLine, Qt::RoundCap,
                        Qt::RoundJoin));
    /* Draw the line with all the splits */
    while( begin + 1 != end )
    {
        painter.drawLine(*begin, *(begin + 1));
        begin++;
    }

    if( a->getEndPoint() != invalidPoint )
    {/* Action has an end, draw arrow */
        /* Create an arrow pointing towards end, mind the arrow directions */
        int arrowLength = a->getEnd()->getRad() / 5;
        int arrowWidth  = a->getEnd()->getRad() / 5;
        int dirX = Maths::sign( endP.x() - lastSplit.x() );
        if( dirX != 0 )
        {/* Moving right dirX = 1, Moving left dirX = -1 */
            drawArrow( endP.x() - arrowLength * dirX  , lastSplit.y(), endP.x(), endP.y(), arrowWidth, &painter );
        }
        else
        {/* Moving up dirY = -1, Moving down dirY = 1 */
            int dirY = Maths::sign( endP.y() - sLastSplit.y() );
            drawArrow( lastSplit.x(), endP.y() - arrowLength * dirY, endP.x(), endP.y(), arrowWidth, &painter );
        }
    }
}
void View::drawPossibleActionLine(const QPoint &start, const QPoint &end)
{
    /*
     * Draw a possible action line split into 2 parts.
     * The endpoint contains an arrow.
     */
    QPainter painter(&image);
    painter.setPen(QPen(QColor(0x000001), 1.1, Qt::SolidLine, Qt::RoundCap,
                        Qt::RoundJoin));

    /* First split happens horizontally */
    QPoint split1 = QPoint( start.x() + Maths::roundToGrid( ( end.x() - start.x() ) / 2, gridSize ), start.y() );

    /* Second split happens vertically */
    QPoint split2 = split1;
    split2.setY( end.y() );

    /* Draw lines */
    painter.drawLine(start, split1);
    painter.drawLine(split1, split2);
    painter.drawLine(split2, end);

    /* Create an arrow pointing towards end, mind the arrow directions */

    /* Define arrow dimensions -> should be a property of each action for later customization ? */
    int arrowLength = gridSize / 2;
    int arrowWidth  = gridSize / 2;

    int dirX = Maths::sign( end.x() - split2.x() );
    if( dirX != 0 )
    {/* Moving right dirX = 1, Moving left dirX = -1 */
        drawArrow( end.x() - arrowLength * dirX  , split2.y(), end.x(), end.y(), arrowWidth, &painter );
    }
    else
    {/* Moving up dirY = -1, Moving down dirY = 1 */
        int dirY = Maths::sign( end.y() - start.y() );
        drawArrow( split2.x(), end.y() - arrowLength * dirY, end.x(), end.y(), arrowWidth, &painter );
    }
}
void View::drawArrow(int x0, int y0, int x, int y, int w, QPainter *painter)
{
    int dx, dy;
    double sina, cosa, l;
    l = qSqrt( (x - x0) * (x - x0)  + (y - y0) * (y - y0) );
    sina = (y - y0) / l;
    cosa = (x - x0) / l;
    dx = ( (sina * w) / 2 );
    dy = ( (cosa * w) / 2 );


    QPoint start= QPoint(x0, y0);
    QPoint end  = QPoint(x, y);
    QPoint right= QPoint( dx + x0, (dy + y0) );
    QPoint left = QPoint( x0 - dx, (y0 - dy) );

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
void View::drawState(const State *s)
{
    QPainter painter(&image);
    /* Get state original pen */
    painter.setPen( s->getPen() );
    painter.setBrush( s->getCol() );
    painter.drawEllipse( s->getPos(), s->getRad(), s->getRad() );

    /* Get a bounding rectangle for the text */
    QString text;
    text.append( QString(s->getLabel() ) );
    QRectF rect = painter.boundingRect( s->getPos().x(), s->getPos().y(), 50, 50, Qt::AlignHCenter, text );

    painter.drawStaticText( s->getPos().x() - rect.width() / 2, s->getPos().y() - rect.height() / 2, QStaticText( text ) ) ;
    painter.end();
}

void View::drawCircleTo(const QPoint &endPoint)
{/* Draw current ellipse */

    QPainter painter(&image);
    /* Clear gridline where new circle is drawn */
    painter.setPen( QPen(QColor( 0xffffff ), penWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin) );
    painter.setBrush( QColor(0xffffff) );
    painter.drawEllipse( endPoint, stateRadius, stateRadius );

    /* Draw new circle */
    painter.setPen( QPen(penColor, penWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin) );
    painter.setBrush(stateColor);
    painter.setOpacity(0.2);
    painter.drawEllipse( endPoint, stateRadius, stateRadius );
    painter.end();
}
