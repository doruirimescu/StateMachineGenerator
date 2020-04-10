#ifndef MANAGER_H
#define MANAGER_H
#include <QVector>
#include <component.h>
#include <state.h>
#include <action.h>
#include <QMap>
#include <QtMath>
class Manager
{
public:
    Manager(){};
    ~Manager();
    void addComponent( Component * c );
    void addState( State * s );
    void deleteState( State * s );
    State * searchState( QPoint pos );
    bool intersectState(QPoint pos);
    QPoint onStateBorder(QPoint pos);
    void print();

    QVector <Component *> components;
    QVector <State *> states;
};

#endif // MANAGER_H
