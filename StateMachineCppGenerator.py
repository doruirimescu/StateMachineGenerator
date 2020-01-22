 #!/usr/bin/python3
def machinecpp(actions, states):
    #generate code snippets
    add_actions = ""
    for a in actions:
        add_actions+="\nvoid StateMachine::move{}()".format(a)
        add_actions+="\n{"
        add_actions+="\n\tptr = ptr -> {};".format(a.lower())
        add_actions+="\n}"

    add_states = "" 
    for s in states:
        add_states += "\n\tcase {}:".format(str(s.index))
        add_states += "\n\t\t{};".format(s.output)
        add_states += "\n\t\tbreak;"
    
    add_main = ""
    for s in states:
        add_main += s.addNode()
    for s in states:
        add_main += s.addTransition()
        
    #generate the code
    code='''/* ----Generated code---- */
#include "StateMachine.h"
using namespace std;
/*Constructor Destructor*/
StateMachine::StateMachine(State* ptr)
{
    this->ptr = ptr;
}
StateMachine::~StateMachine()
{
}

/* Actions */\
    %s
int StateMachine::getState()
{
    return ptr -> state;
}

/* Input */
void StateMachine::input( int input )
{/* As many inputs as there are actions */
    switch( input )
    {\
    %s
    default:
        print();
        break;
    }
}
void StateMachine::print()
{
    cout << "State: " << ptr -> state <<" "<<
        "Label: " << ptr->label <<endl;
}
/* Main */
int main()
{
    %s
    State* ptr;
    ptr =&state%s;
    ''' % (add_actions, add_states, add_main, str(states[0].index) )
    code +="\n\treturn 0;\n}"
    f= open("Generated/StateMachine.cpp","w+")
    f.write(code)
    f.close