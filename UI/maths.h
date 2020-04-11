#ifndef MATHS_H
#define MATHS_H
#include <QtMath>

#define PI_OVER_4       M_PI/4
#define MINUS_PI_OVER_4 -M_PI/4
#define THREE_PI_OVER_4 3*M_PI/4
#define MINUS_THREE_PI_OVER_4 3*M_PI/4

class Maths
{
public:
    Maths();
    static int sign(int expr);
};

#endif // MATHS_H
