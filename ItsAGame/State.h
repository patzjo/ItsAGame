#pragma once
#include <iostream>

enum StateEnum { QUIT, MENU, PLAYING, START_NEW_GAME, STATE_NONE, ANNOUNCE_WINNER };

class State
{
public:
	void setState(StateEnum State) { state = State;	}
	
	void requestState(StateEnum State) { stateRequested = State; }
	
	StateEnum getState() { return state; }

	void changeRequestedState() 
	{
		if ( state != stateRequested && 
			stateRequested != StateEnum::STATE_NONE)
		{
			setState(stateRequested);
			stateRequested = StateEnum::STATE_NONE;
		}
	}

	State()	{}
private:
	StateEnum state = StateEnum::MENU;
	StateEnum stateRequested = StateEnum::STATE_NONE;
};