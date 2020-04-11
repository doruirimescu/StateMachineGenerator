#ifndef MATHS_H
#define MATHS_H
#include <QtMath>
#include <QPoint>
#define PI_OVER_4       M_PI/4
#define MINUS_PI_OVER_4 -M_PI/4
#define THREE_PI_OVER_4 3*M_PI/4
#define MINUS_THREE_PI_OVER_4 -3*M_PI/4
constexpr QPoint invalidPoint = QPoint(-1000, -1000);
class Maths
{
public:
    Maths();
    static int sign(int expr);
    static int roundToGrid(int var, int grid);
};

#endif // MATHS_H
