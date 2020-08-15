#include "scribblearea.h"

/* Mouse event methods : Controller */

void ScribbleArea::mousePressEvent(QMouseEvent *event)
{/* When user clicks */

    QApplication::restoreOverrideCursor();
    if( event->button() != Qt::LeftButton )
    {/* Exit early if left mouse wasn't clicked */
        return;
    }

    /* If no state is moving, movingState is nullptr */
    bool intersectState = m->intersectState( currentPoint, getStateRadius(), getGridSize() );

    if( pState && intersectState == false )
    {/* State placement is done, and we are not intersecting another state */

        bool ok;
        QString newLabel= QInputDialog::getText(this, "State label", "Enter new state label", QLineEdit::Normal, "S", &ok);
        if( ok && !newLabel.isEmpty() )
        {
            m->addState(new State(newLabel, currentPoint,
                                  QPen(getPenColor(), getPenWidth(), Qt::SolidLine, Qt::SquareCap, Qt::RoundJoin),
                                  getStateColor(), getStateRadius() ));
            pState = false;
        }
    }
    else if( mState && intersectState == false )
    {/* State movement is done, and we are not intersecting another state*/
        /* Not moving a state anymore */
        mState = false;
        /*Update this state's actions start and end points*/
        m->updateActionStartEnd(movingState);
        /*There is no state which is moving anymore*/
        movingState = nullptr;
        /*Run A**/
        m->Astar( getGridSize(), width(), height() );
    }
    else if( eState )
    {/* State editing is opened */
        /* Try to find state at clicked position */
        State *s = m->searchState( event->pos() );

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
                bool ok;
                QString label = dialog->getText( this, "Edit label", "Enter the new value for the state label",
                                                 QLineEdit::Normal, s->getLabel(), &ok );
                if( ok && !label.isEmpty() && m->uniqueLabel(label) )
                {
                    s->setLabel( label );
                }
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

        if( pActionStart == false && m->states.size() > 0 && actionStartPoint != invalidPoint )
        {/* User wants to place the startpoint of the action */
            pActionStart = true;
            /* Create partial action*/
            bool ok;
            QString actionLabel= QInputDialog::getText(this, "Action label", "Enter new action label", QLineEdit::Normal,
                                                       "", &ok);
            if( ok && !actionLabel.isEmpty() )
            {
                m->addAction( new Action( actionLabel, actionStart, actionEnd, actionStartPoint, invalidPoint ) );
                m->getLastAction()->addSplit( actionStartPoint );
                m->getLastAction()->setStartAnchor(actionStartAnchor);
            }
            else
            {
                pAction = false;
                pActionStart = false;
            }
        }
        else if( pActionStart == true && m->getLastAction()->getStartPoint() != currentPoint)
        {/* Action startpoint placement has finished */

            if( actionEndPoint != invalidPoint )
            {
                currentPoint = actionEndPoint;
            }

            QPoint split1, split2;
            /* First split happens horizontally */
            split1 = QPoint( actionStartPoint.x() + Maths::roundToGrid( ( currentPoint.x() - actionStartPoint.x() ) / 2, view->getGridSize() ),
                                    actionStartPoint.y() );

            /* Second split happens vertically */
            /* Handle exceptional case just as in view. */
            if( qAbs(currentPoint.x() - actionStartPoint.x()) == view->getGridSize() )
            {
                split1 = actionStartPoint;
            }
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

                /* Update state-actions hash map */
                m->mapStateToActions();

                pActionStart = false;
                pAction = false;
            }
            lastAct->addSplit(currentPoint);
        }
    }
    drawGrid();
}


void ScribbleArea::mouseMoveEvent(QMouseEvent *event)
{/* When user is moving the mouse */

    updateCurrentPoint( Maths::roundToGrid( event->pos().x(), view->getGridSize() ),
                        Maths::roundToGrid( event->pos().y(), view->getGridSize() )
                        );
    if( pState || mState )
    {
        boundToDrawingArea();
    }

    drawGrid();

    /* Does the current cursor point intersect other states besides the state which I'm placing ?*/
    if( pState && m->intersectState(currentPoint, getStateRadius(), getGridSize() ) )
    {/* Placing a state, but intersecting another one */
        view->drawInvalidCircleTo(currentPoint);
    }
    else if( pState )
    {/* If moving mouse while placing state */
        view->drawCircleTo(currentPoint);
    }
    else if ( mState && !m->intersectState(currentPoint, movingState, getGridSize() ) ) //check if intersecting with moving state
    {/* If moving a state */
        movingState->setPos(currentPoint);
    }
    else if( mState )
    {/* Intersecting state */
        movingState->setPos(invalidPoint);
        view->drawInvalidCircleTo(currentPoint);
    }
    else if( pAction )
    {/* If placing action */
        QString anchorInfo;
        QPoint possibleAnchorPoint = m->onStateBorder( currentPoint, anchorInfo );
        if( possibleAnchorPoint != invalidPoint )
        {/* If cursor is at any valid anchor point */

            view->drawPossibleAnchor( possibleAnchorPoint );
            if( pActionStart == false )
            {/* The user is placing an action startpoint */

                /* Save current state to the startpoint of the action */
                actionStart = m->searchState(possibleAnchorPoint);
                actionStartPoint = possibleAnchorPoint;
                actionStartAnchor = anchorInfo;
            }
            else if( pActionStart == true )
            {/* The user is placing an action endpoint */

                actionEnd = m->searchState(possibleAnchorPoint);
                actionEndPoint = possibleAnchorPoint;
                actionEndAnchor = anchorInfo;
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
}
