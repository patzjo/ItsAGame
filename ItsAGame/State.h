#pragma once
#include <iostream>

enum StateEnum { QUIT, MENU, PLAYING, START_NEW_GAME, STATE_NONE };

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
			std::cout << "Change state! State: " << state << " -> " << stateRequested << std::endl;
			setState(stateRequested);
			stateRequested = StateEnum::STATE_NONE;
		}
	}

	State()	{}
private:
	StateEnum state = StateEnum::START_NEW_GAME;
	StateEnum stateRequested = StateEnum::STATE_NONE;
};