/* ----Generated code---- */
#pragma once
#include "State.h"
using namespace std;
class StateMachine
{
	public:
	/*---------------Constructor Destructor---------------*/
	StateMachine( State * ptr );
	~StateMachine();

	/*---------------Methods---------------*/

	/* Actions*/
	void moveLeft();
	void moveRight();
	void moveUp();
	void moveDown();
	void moveSelect();
	int getState();

	/* Input */
	void input( int input );

	 /* Output */
	void output();
	void print();

	/*-------------Members--------------*/
	State * ptr;
};