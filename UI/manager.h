#ifndef MANAGER_H
#define MANAGER_H
#include <QVector>
#include <state.h>
#include <action.h>
#include <QMap>
#include "maths.h"
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
};

#endif // MANAGER_H
