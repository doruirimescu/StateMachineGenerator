#ifndef MANAGER_H
#define MANAGER_H
#include <QVector>
#include <state.h>
#include <action.h>
#include <QMap>
#include "maths.h"
#include "Astar/porting.hpp"
#include "Astar/Agent.hpp"
#include "Astar/Wall.hpp"
#include "Astar/heuristic.hpp"
#include "Astar/MAPPGridState.hpp"
#include "Astar/Astar.hpp"

class Manager
{
public:
    Manager(){};
    ~Manager();
    void addState( State * s );
    void deleteState( State * s );
    State * searchState( QPoint pos );
    bool intersectState( QPoint pos );
    bool intersectState( QPoint pos, State *s );
    QPoint onStateBorder( QPoint pos, QString & posInfo );
    void printStates();

    void addAction( Action* a);
    void printActions();
    QVector <State *> states;
    QVector <Action*> actions;
    Action* getLastAction();
    void deleteAction( Action *a );
    void Astar(int gridSize, int width, int height);
};

#endif // MANAGER_H
