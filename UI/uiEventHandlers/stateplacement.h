#ifndef STATEPLACEMENT_H
#define STATEPLACEMENT_H

#include "scribblearea.h"
#include "QtDebug"

class ScribbleArea;

class statePlacement
{
public:
    statePlacement(ScribbleArea *s) : canvas(s) { pState = false; };
    void showDebug();
private:
    ScribbleArea *canvas;
    bool pState;
};

#endif // STATEPLACEMENT_H
