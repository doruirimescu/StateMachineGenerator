#ifndef MANAGER_H
#define MANAGER_H
#include <QVector>
#include <component.h>
#include <state.h>
#include <action.h>
#include <QMap>

class Manager
{
public:
    Manager(){};
    ~Manager();
    void addComponent( Component * c );
    void addState( State * c );
    State * searchState( QPoint pos );
    void print();
    QVector <Component *> components;
    QVector <State *> states;
};

#endif // MANAGER_H
