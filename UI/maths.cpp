#include "maths.h"

Maths::Maths()
{

}

int Maths::sign(int expr)
{
    if( expr > 0 )
    {
        return 1;
    }
    else if( expr < 0)
    {
        return -1;
    }
    else
    {
        return 0;
    }
}

int Maths::roundToGrid(int var, int grid)
{
    return (int) round( (float)var / (float)grid ) * grid;
}
