#include "view.h"

#include "arrow.h"

void View::drawLine(const QPoint &start, const QPoint &end)
{
    QPainter painter(&image);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QPen(QColor(0xE0E0E0), 1.1, Qt::SolidLine, Qt::RoundCap,
                        Qt::RoundJoin));
    painter.drawLine(start, end);
    painter.end();
}
void View::drawActions(const QVector<Action *> &actions)
{
    for( const auto & a: actions )
    {
        drawAction(a);
    }
}
void View::drawAction(const Action *const a)
{/* Draw a full action that has been completed */
    drawAnchor(a->getEndPoint());
    drawAnchor(a->getStartPoint());
    drawActionLineDev(a);

    /* Draw action text */
    if( a->getSplitsSize() > 1 )
    {/* Need at least 2 splits to draw text in between them */
        drawActionText(a);
    }
}
void View::drawActionText(const Action *const a)
{
    QPainter painter(&image);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QPen(QColor(0x1000E0), 1.1, Qt::SolidLine, Qt::RoundCap,
                        Qt::RoundJoin));
    QVector<QPoint>::const_iterator begin;
    QVector<QPoint>::const_iterator end;
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
    painter.setRenderHint(QPainter::TextAntialiasing);
    painter.drawStaticText(textX - rect.width()/2, textY, QStaticText( a->getLabel() ) ) ;
    painter.end();
}
void View::drawAnchor(const QPoint &endPoint)
{/* Draw anchor point where an action could join */
    QPainter painter(&image);
    painter.setRenderHint(QPainter::Antialiasing);
    /*Remove previous point*/
    painter.setBrush( ( QColor(Qt::white) ) );
    painter.drawEllipse(endPoint, 5, 5 );

    painter.setPen( QPen( QColor(0x3333FF), 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin) );
    painter.setBrush( QColor(0xB0E3E6) );
    painter.setOpacity(0.7);
    painter.drawEllipse(endPoint, 5, 5 );
    painter.end();
}

void View::drawPossibleAnchor(const QPoint &endPoint)
{/* Draw a possible anchor point where an action could join */
    QPainter painter(&image);
    painter.setRenderHint(QPainter::Antialiasing);
    /*Remove previous point*/
    painter.setBrush( ( QColor(Qt::white) ) );
    painter.drawEllipse(endPoint, 6, 6 );

    painter.setPen( QPen( QColor(0x3333FF), 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin) );
    painter.setBrush( QColor(0xC0D3E6) );
    painter.setOpacity(0.2);
    painter.drawEllipse(endPoint, 6, 6 );
    painter.end();

}
void View::drawActionLineDev(const Action *const a)
{
    /* Get all the split points of the action */
    QVector<QPoint>::const_iterator begin;
    QVector<QPoint>::const_iterator end;
    a->getSplits(begin, end);

    /* Define the points needed for drawing an arrow */
    const QPoint endP       = *(end-1);//end point
    const QPoint lastSplit  = *(end-2);//last split
    const QPoint sLastSplit = *(end-3);//second last split

    QPainter painter(&image);
    painter.setRenderHint(QPainter::Antialiasing);
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
            Arrow arrow( endP.x() - arrowLength * dirX  , lastSplit.y(), endP.x(), endP.y(), arrowWidth, &painter );
            arrow.draw();
        }
        else
        {/* Moving up dirY = -1, Moving down dirY = 1 */
            int dirY = Maths::sign( endP.y() - sLastSplit.y() );
            Arrow arrow( lastSplit.x(), endP.y() - arrowLength * dirY, endP.x(), endP.y(), arrowWidth, &painter );
            arrow.draw();
        }
    }
    painter.end();
}
void View::drawPossibleActionLine(const QPoint &start, const QPoint &end)
{
    /*
     * Draw a possible action line split into 2 parts.
     * The endpoint contains an arrow.
     */
    QPainter painter(&image);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QPen(QColor(0x000001), 1.1, Qt::SolidLine, Qt::RoundCap,
                        Qt::RoundJoin));

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
        Arrow arrow( end.x() - arrowLength * dirX  , split2.y(), end.x(), end.y(), arrowWidth, &painter );
        arrow.draw();
    }
    else
    {/* Moving up dirY = -1, Moving down dirY = 1 */
        int dirY = Maths::sign( end.y() - start.y() );
        Arrow arrow( split2.x(), end.y() - arrowLength * dirY, end.x(), end.y(), arrowWidth, &painter );
        arrow.draw();
    }
    painter.end();
}

void View::drawState(const State *s)
{
    QPainter painter(&image);
    painter.setRenderHint(QPainter::Antialiasing);
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
void View::drawStates(const QVector<State *> &states)
{
    for( const auto & s : states )
    {
        drawState(s);
    }
}
void View::drawCircleTo(const QPoint &endPoint)
{/* Draw current ellipse */

    QPainter painter(&image);
    painter.setRenderHint(QPainter::Antialiasing);
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

void View::drawInvalidCircleTo(const QPoint &endPoint)
{/* Draw current ellipse */

    QPainter painter(&image);
    painter.setRenderHint(QPainter::Antialiasing);
    /* Clear gridline where new circle is drawn */
    painter.setPen( QPen(QColor( 0xffffff ), penWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin) );
    painter.setBrush( QColor(0xffffff) );
    painter.drawEllipse( endPoint, stateRadius, stateRadius );

    /* Draw new circle */
    painter.setPen( QPen(penColor, penWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin) );
    painter.setBrush(QColor(Qt::red));
    painter.setOpacity(0.3);
    painter.drawEllipse( endPoint, stateRadius, stateRadius );
    painter.end();
}
