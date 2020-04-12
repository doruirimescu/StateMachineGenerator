#include "scribblearea.h"

/* Mouse event methods : Controller */

void ScribbleArea::mousePressEvent(QMouseEvent *event)
{/* When user clicks */

    QApplication::restoreOverrideCursor();
    if (event->button() == Qt::LeftButton)
    {
        lastPoint = event->pos();

        if( pState && !m->intersectState(lastPoint) )
        {/* State placement is done, and we are not intersecting another state */

            QString newLabel= QInputDialog::getText(this, "State label", "Enter new state label");
            if( newLabel.compare("") )
            {
                m->addState(new State(newLabel, prevPoint,
                                      QPen(getPenColor(), getPenWidth(), Qt::SolidLine, Qt::SquareCap, Qt::RoundJoin), getStateColor(), getStateRadius() ));
            }
            pState = false;
        }
        else if( eState )
        {/* State editing is opened */

            /* Try to find state at clicked position */
            State *s = m->searchState(event->pos());

            if( s != nullptr )
            {/* If clicking inside a valid state */

                QStringList editOptions;
                editOptions << "Edit label";
                editOptions << "Edit output code";
                editOptions << "Delete state";

                QInputDialog *dialog = new QInputDialog;
                dialog->setComboBoxEditable(false);

                QString chooseEdit = "Edit " + s->getLabel();
                QString option = dialog->getItem( this, chooseEdit, "What property do you want to edit?", editOptions, 0, false );

                if( option.compare(editOptions[0]) == 0)
                {/* Edit label */
                    QString label = dialog->getText( this, "Edit label", "Enter the new value for the state label", QLineEdit::Normal, s->getLabel() );
                    s->setLabel( label );
                }
                else if( option.compare(editOptions[1]) == 0 )
                {/* Edit code */
                    QString code = dialog->getMultiLineText( this, "Edit code", "Enter the C++ code to \nrun on state output", s->getCode() );
                    s->setCode( code );
                }
                else if( option.compare(editOptions[2]) == 0 )
                {/* Delete state */
                    m->deleteState(s);
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

                Action *lastAct = m->getLastAction();
                lastAct->setEnd(actionEnd);
                lastAct->setEndPoint(&actionEndPoint);
                pActionStart = false;
                pAction = false;
                m->printActions();
            }
        }
    }
    drawGrid();
}


void ScribbleArea::mouseMoveEvent(QMouseEvent *event)
{/* When user is moving the mouse */

    int x, y;
    x = Maths::roundToGrid( event->pos().x(), view->getGridSize() );
    y = Maths::roundToGrid( event->pos().y(), view->getGridSize() );
    QPoint currentPoint = QPoint(x,y);
    drawGrid();
    if( pState )
    {/* If moving mouse while placing state */

        /* Delete from previous position */
        view->deleteCircleFrom(prevPoint);
        /* Draw at potentially new position */
        view->drawCircleTo(currentPoint);

        /* Update old position with new position */
        prevPoint = currentPoint;
    }
    else if( pAction )
    {/* If placing first part of action */

        QString posInfo;
        QPoint possibleAnchorPoint = m->onStateBorder( event->pos(), posInfo );
        if( possibleAnchorPoint != invalidPoint )
        {/* If cursor is at any valid anchor point */

            view->drawAnchor( possibleAnchorPoint );
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
                view->drawActionLine(actionStartPoint, possibleAnchorPoint);
            }
        }
        else if( pActionStart == true )
        {/* The user has placed the first anchor point */

            view->drawActionLine(actionStartPoint, currentPoint);
        }
    }
    update();
}
