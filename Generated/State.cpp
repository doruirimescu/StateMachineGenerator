/* ----Generated code---- */
#include "State.h" 
using namespace std;

/*Constructor Destructor*/
State::State( int state, string label )
{
    this -> state    = state;
    this -> label    = label;    
	this -> left = this;
	this -> right = this;
	this -> up = this;
	this -> down = this;
	this -> select = this;
}
State::~State()
{
}

/* Building */
void State::addLeft( State* s)
{
	this -> left = s;
	s -> right = this;
}

void State::addRight( State* s)
{
	this -> right = s;
	s -> left = this;
}

void State::addUp( State* s)
{
	this -> up = s;
	s -> down = this;
}

void State::addDown( State* s)
{
	this -> down = s;
	s -> up = this;
}

void State::addSelect( State* s)
{
	this -> select = s;
}

/* Debugging */ 
void State::print()  
{                  
    cout << this -> label <<' '<< this -> state <<endl;
}