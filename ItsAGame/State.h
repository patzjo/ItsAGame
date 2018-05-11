#pragma once

enum StateEnum { QUIT, MENU, PLAYING, START_NEW_GAME };

class State
{
public:
	void setState(StateEnum State) { state = State;	}
	StateEnum getState() { return state; }

	State()	{ state = START_NEW_GAME; }
private:
	StateEnum state;
};