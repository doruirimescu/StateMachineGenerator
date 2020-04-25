#include "state.h"
void State::print()
{
    qInfo()<<" x: "<<pos.x()<<" y:"<<pos.y();
}
void State::bonundToDrawingArea( int width, int height, int gridSize)
{
    if( pos.x() - radius < gridSize )
    {/* If the cursor is out of bounds, limit it inside drawing area*/
        pos.setX( gridSize + radius );
    }
    if( pos.y() - radius < gridSize )
    {/* If the cursor is out of bounds, limit it inside drawing area*/
        pos.setY( gridSize + radius );
    }

    if( pos.x() + radius > width - gridSize )
    {/* If the cursor is out of bounds, limit it inside drawing area*/
        pos.setX( width - radius - gridSize );
    }

    if( pos.y() + radius > height - gridSize )
    {/* If the cursor is out of bounds, limit it inside drawing area*/
        pos.setY( height - radius - gridSize );
    }
}
