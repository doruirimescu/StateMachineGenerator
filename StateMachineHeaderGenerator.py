 #!/usr/bin/python3
def machineheader(actions, states):
    #generate code snippets
    add_actions = ""
    for a in actions:
        add_actions +="\n\tvoid move" + a + "();"

    code='''/* ----Generated code---- */
#pragma once
#include \"State.h\"
using namespace std;
class StateMachine
{
\tpublic:
\t/*---------------Constructor Destructor---------------*/
\tStateMachine( State * ptr );
\t~StateMachine();\n
\t/*---------------Methods---------------*/

\t/* Actions*/\
%s
\tint getState();\n
\t/* Input */
\tvoid input( int input );

\t /* Output */
\tvoid output();
\tvoid print();

\t/*-------------Members--------------*/
\tState * ptr;
};''' % add_actions
    f= open("Generated/StateMachine.h","w+")
    f.write(code)
    f.close 
