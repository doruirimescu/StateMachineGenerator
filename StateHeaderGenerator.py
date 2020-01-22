 #!/usr/bin/python3
def stateheader(actions):
    #generate code snippets
    add_actions = ""
    for a in actions:
        add_actions += "\n    void add"+str(a)+"(State* s);"

    add_states = ""
    for a in actions:
        add_states += "\n    State* " + str(a).lower()+";"

    #generate the code
    code = '''/* ----Generated code---- */
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
    %s
    /* Debug */
    void print();

    /*-----------Members--------*/
    int state;
    string label; 
    %s
};''' % ( add_actions, add_states )

    f= open("Generated/State.h","w+")
    f.write(code)
    f.close 
