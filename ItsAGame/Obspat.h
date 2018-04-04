#pragma once
#include <iostream>
#include <memory>
#include <vector>


// Event data casting is made using these types
enum EVENT_TYPE { E_ERROR, E_COLLISION, E_INPUT, E_DAMAGE_TAKEN };


class Observer
{
	friend class Subject;
public:
	Observer();
	~Observer();

	virtual void onNotify(class GameObject *gameObject, int eventType, void *eventData) = 0;
private:
	
};

class Subject
{
public:
	void addObserver(Observer *observer)
	{
		observers.push_back(observer);
	}

	void removeObserver(Observer *observer)
	{
		for (auto itr = observers.begin(); itr != observers.end(); itr++)
		{
			if (*itr == observer)
			{
				itr = observers.erase(itr);
				return;
			}
		}
	}

	void notify(class GameObject *gameObject, int eventType, void *eventData)
	{
		for (Observer *obs : observers)
		{
			obs->onNotify(gameObject, eventType, eventData);
		}
	}

private:

	std::vector <Observer *> observers;
	
};

