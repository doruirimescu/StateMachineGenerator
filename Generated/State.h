/* ----Generated code---- */
#pragma once
#include <iostream>
#include <string>
using namespace std;
class State
{
    public:
    /*---------------Constructor Destructor---------------*/
    State( int state, string label );
    ~State();

    /*---------------Methods---------------*/

    /* Building. Link resulting objects to actions */
    
    void addLeft(State* s);
    void addRight(State* s);
    void addUp(State* s);
    void addDown(State* s);
    void addSelect(State* s);
    /* Debug */
    void print();

    /*-----------Members--------*/
    int state;
    string label; 
    
    State* left;
    State* right;
    State* up;
    State* down;
    State* select;
};