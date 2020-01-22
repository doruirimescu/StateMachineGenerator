 #!/usr/bin/python3

def statecpp(actions, revActions):
    
    constructor_code=""
    for a in actions:
            constructor_code += "\n\tthis -> "+str(a).lower()+" = this;"
    building_code=""
    for index, a in enumerate( actions ):
            building_code += "\nvoid State::add%s( State* s)\n{" % str(a)
            building_code +="\n\tthis -> "+str(a).lower()+" = s;"
            if revActions[ index ] is not -1:
                building_code += "\n\ts -> %s = this;\n}\n" % str(revActions[index]).lower()
            else:
                building_code += "\n}"

    code ='''/* ----Generated code---- */
#include \"State.h\" 
using namespace std;

/*Constructor Destructor*/
State::State( int state, string label )
{
    this -> state    = state;
    this -> label    = label;\
    %s
}
State::~State()
{
}

/* Building */\
%s

/* Debugging */ 
void State::print()  
{                  
    cout << this -> label <<' '<< this -> state <<endl;
}''' % (constructor_code, building_code)
    f= open("Generated/State.cpp","w+")
    f.write(code)
    f.close()