/* ----Generated code---- */
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

/* Actions */    
void StateMachine::moveLeft()
{
	ptr = ptr -> left;
}
void StateMachine::moveRight()
{
	ptr = ptr -> right;
}
void StateMachine::moveUp()
{
	ptr = ptr -> up;
}
void StateMachine::moveDown()
{
	ptr = ptr -> down;
}
void StateMachine::moveSelect()
{
	ptr = ptr -> select;
}
int StateMachine::getState()
{
    return ptr -> state;
}

/* Input */
void StateMachine::input( int input )
{/* As many inputs as there are actions */
    switch( input )
    {    
	case 0:
		cout<<"1"<<endl;
		break;
	case 1:
		cout<<"2"<<endl;
		break;
	case 2:
		cout<<"3"<<endl;
		break;
	case 3:
		cout<<"4"<<endl;
		break;
	case 4:
		cout<<"5"<<endl;
		break;
	case 5:
		cout<<"6"<<endl;
		break;
	case 6:
		cout<<"7"<<endl;
		break;
	case 7:
		cout<<"8"<<endl;
		break;
	case 8:
		cout<<"Game1"<<endl;
		break;
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
    
	State state0(0,"Menu 1");
	State state1(1,"Menu 2");
	State state2(2,"Menu 3");
	State state3(3,"Menu1 Feature 1");
	State state4(4,"Menu1 Feature 2");
	State state5(5,"Menu1 Feature1 Subfeature 1");
	State state6(6,"Menu3 Feature 1");
	State state7(7,"Menu2 Feature 1");
	State state8(8,"Enter game1");
	state0.addLeft(&state1);
	state0.addDown(&state3);
	state0.addSelect(&state8);
	state1.addLeft(&state2);
	state1.addDown(&state7);
	state2.addDown(&state6);
	state3.addLeft(&state4);
	state3.addDown(&state5);
    State* ptr;
    ptr =&state0;
    
	return 0;
}