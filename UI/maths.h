#ifndef MATHS_H
#define MATHS_H
#include <QtMath>
#include <QPoint>

constexpr double PI_OVER_4              = M_PI/4;
constexpr double MINUS_PI_OVER_4        = -M_PI/4;
constexpr double THREE_PI_OVER_4        = 3*M_PI/4;
constexpr double MINUS_THREE_PI_OVER_4  = -3*M_PI/4;
constexpr QPoint invalidPoint = QPoint(-1000, -1000);

class Maths
{
public:
    Maths();
    static int sign(int expr);
    static inline int roundToGrid(int var, int grid) { return (int) round( (float)var / (float)grid ) * grid; };
};

#endif // MATHS_H
