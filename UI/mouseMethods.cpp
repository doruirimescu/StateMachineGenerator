#include "scribblearea.h"

/* Mouse event methods : Controller */

void ScribbleArea::mousePressEvent(QMouseEvent *event)
{/* When user clicks */

    QApplication::restoreOverrideCursor();
    if (event->button() == Qt::LeftButton)
    {
        currentPoint.setX(Maths::roundToGrid( event->pos().x(), view->getGridSize() ));
        currentPoint.setY(Maths::roundToGrid( event->pos().y(), view->getGridSize() ));

        if( pState && !m->intersectState(currentPoint) )
        {/* State placement is done, and we are not intersecting another state */

            QString newLabel= QInputDialog::getText(this, "State label", "Enter new state label");
            if( newLabel.compare("") )
            {
                m->addState(new State(newLabel, currentPoint,
                                      QPen(getPenColor(), getPenWidth(), Qt::SolidLine, Qt::SquareCap, Qt::RoundJoin),
                                      getStateColor(), getStateRadius() ));
            }
            pState = false;
        }
        else if( mState && !m->intersectState(currentPoint, movingState) )
        {
            mState = false;
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
                editOptions << "Edit position";
                editOptions << "Delete state";

                QInputDialog *dialog = new QInputDialog;
                dialog->setComboBoxEditable(false);

                QString chooseEdit = "Edit " + s->getLabel();
                QString option = dialog->getItem( this, chooseEdit, "What property do you want to edit?", editOptions, 0, false );

                if( option.compare(editOptions[0]) == 0)
                {/* Edit label */
                    QString label = dialog->getText( this, "Edit label", "Enter the new value for the state label",
                                                     QLineEdit::Normal, s->getLabel() );
                    s->setLabel( label );
                }
                else if( option.compare(editOptions[1]) == 0 )
                {/* Edit code */
                    QString code = dialog->getMultiLineText( this, "Edit code", "Enter the C++ code to \nrun on state output", s->getCode() );
                    s->setCode( code );
                }
                else if( option.compare(editOptions[2]) == 0 )
                {/* Edit position */
                    mState = true;
                    movingState = s;
                }
                else if( option.compare(editOptions[3]) == 0 )
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
                QString actionLabel= QInputDialog::getText(this, "Action label", "Enter new action label");
                m->addAction( new Action( actionLabel, actionStart, actionEnd, actionStartPoint, invalidPoint ) );
                m->getLastAction()->addSplit( actionStartPoint );
                m->getLastAction()->setStartAnchor(actionStartAnchor);
            }
            else if( pActionStart == true )
            {/* Action startpoint placement has finished */
                tst = true;

                if( actionEndPoint != invalidPoint )
                {
                    currentPoint = actionEndPoint;
                }

                QPoint split1, split2;
                /* First split happens horizontally */
                split1 = QPoint( actionStartPoint.x() + Maths::roundToGrid( ( currentPoint.x() - actionStartPoint.x() ) / 2, view->getGridSize() ),
                                        actionStartPoint.y() );

                /* Second split happens vertically */
                split2 = split1;
                split2.setY( currentPoint.y() );

                /* Get current action, add the current splits */
                Action *lastAct = m->getLastAction();
                lastAct->addSplit(split1);
                lastAct->addSplit(split2);
                actionStartPoint = currentPoint;

                if( actionEndPoint != invalidPoint )
                {/* The user wants to finalize placing the action */
                    Action *lastAct = m->getLastAction();
                    lastAct->setEnd(actionEnd);
                    lastAct->setEndPoint(&actionEndPoint);
                    lastAct->setEndAnchor(actionEndAnchor);
                    pActionStart = false;
                    pAction = false;
                    tst = false;
                }

                lastAct->addSplit(currentPoint);
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

        /* Draw at potentially new position */
        view->drawCircleTo(currentPoint);

        /* Update old position with new position */
    }
    else if (mState)
    {
        movingState->setPos(currentPoint);
    }
    else if( pAction )
    {/* If placing action */

        QString posInfo;
        QPoint possibleAnchorPoint = m->onStateBorder( currentPoint, posInfo );
        if( possibleAnchorPoint != invalidPoint )
        {/* If cursor is at any valid anchor point */

            view->drawAnchor( possibleAnchorPoint );
            if( pActionStart == false )
            {/* The user is placing an action startpoint */

                /* Save current state to the startpoint of the action */
                actionStart = m->searchState(possibleAnchorPoint);
                actionStartPoint = possibleAnchorPoint;
                actionStartAnchor = posInfo;
            }
            else if( pActionStart == true )
            {/* The user is placing an action endpoint */

                actionEnd = m->searchState(possibleAnchorPoint);
                actionEndPoint = possibleAnchorPoint;
                actionEndAnchor = posInfo;
            }
        }
        else
        {
            /* Reset action endpoint in case the user changes his mind */
            actionEndPoint = invalidPoint;
        }
        if( pActionStart == true )
        {/* The user has placed the first anchor point */
            view->drawPossibleActionLine(actionStartPoint, currentPoint);
        }
    }
    update();
}
