#ifndef ACTION_H
#define ACTION_H

#include <component.h>
#include <state.h>
#include <QString>
class Action : public Component
{
public:
    Action(State* from, State* to, QString label) : Component(label), from_(from), to_(to){};
    ~Action(){ delete this; }
private:
    State * from_;
    State * to_;
};

#endif // ACTION_H
