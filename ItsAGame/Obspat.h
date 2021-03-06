#pragma once
#include <iostream>
#include <memory>
#include <vector>
#include <SFML/Graphics.hpp>

// Event data casting is made using these types
enum EVENT_TYPE { E_ERROR, E_START_OVERLAP, E_END_OVERLAP, E_INPUT, E_DAMAGE_TAKEN, E_COLLISION_WITH_LEVEL, E_REMOVE_GAMEOBJECT, E_REMOVE_PLAYER, E_INFORM_GAMEOBJECT_REMOVED, E_PLAY_SOUND, E_SPAWN_EXPLOSION, E_START_AGAIN,
				  E_ANOTHER_GAMEOBJECT_COLLIED};


struct CollisionData
{
	class GameObject *colliedGameObject = nullptr;
	class GameObject *gameObject = nullptr;
	class World		 *world = nullptr;
	class Level		 *level = nullptr;
};

struct PlayerKilledData
{
	class GameObject *whoKilled = nullptr;
	class Player *player = nullptr;
};

struct ExplosionData
{
	sf::Vector2f position;
	int type = 0;
};

class Observer
{
	friend class Subject;
public:
	Observer();
	~Observer();

	virtual void onNotify(class GameObject *gameObject, int eventType, void *eventData) = 0;
	class Subject *getSubject() { return subject;  }
protected:
	class Subject *subject;
};

class Subject
{
public:
	void addObserver(Observer *observer)
	{
		observer->subject = this;
		observers.push_back(observer);
	}

	virtual void notifySubject(int event, void *data) {};


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

