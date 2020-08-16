#ifndef MANAGER_H
#define MANAGER_H
#include <QVector>
#include <state.h>
#include <action.h>
#include <QMap>
#include <QHash>
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
    Manager(){smallestStateRadius = INT_MAX;};
    ~Manager();
    void addState( State * s );
    void deleteState( State * s );
    State * searchState( QPoint pos );
    bool intersectState( QPoint pos, int rad, int gridSize );
    bool intersectState( QPoint pos, State * s, int gridSize );
    QPoint onStateBorder( QPoint pos, QString & posInfo );
    void printStates();

    void addAction( Action* a);
    void printActions();
    QVector <State *> states;
    QVector <Action*> actions;
    Action* getLastAction();
    void deleteAction( Action *a );
    void Astar(int gridSize, int width, int height);

    void mapStateToActions();
    void updateActionStartEnd(State* s);

    bool uniqueLabel(QString);

    int getSmallestStateRadius(){return smallestStateRadius;};
private:
    int smallestStateRadius;
    QHash<State*, vector<Action*> > stateActionStartMap;
    QHash<State*, vector<Action*> > stateActionEndMap;
};

#endif // MANAGER_H
