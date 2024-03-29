#pragma once

#include <queue>
#include "Collision.h"
#include "Level.h"
#include "GameObject.h"
#include "Obspat.h"

struct WorldForces
{
	sf::Vector2f wind;
	sf::Vector2f gravity;
};

struct CreationQueue
{
	GameObject *object;
	sf::Vector2f position;
};

class World : public Subject
{
	friend class Game;
	friend class Renderer;
public:
	World();
	~World();

	void setGamePointer(class Game *Game) { game = Game; }
	void initialize();

	void startAgain();
	
	void update(float dT);
	void checkCollisions(float dT);
	
	GameObject *createObject(sf::Vector2f position, GameObject *object);
	void queueToCreate(sf::Vector2f position, GameObject *object);
	void queueToRemove(GameObject *object);
	void processQueues();
	void notifySubject(int event, void *data);

	void applyWind(int left, int right);

	sf::Vector2f getGravity()	{ return activeForces.gravity; }
	sf::Vector2f getWind()		{ return activeForces.wind; }

	class Game *getGame()		{ return game; }

	std::vector <GameObject *> getObjectsInRange(Rectangle<float> area) { return collisionTree.getObjectsInRange(area); }

private:
	Level level;
	std::vector <GameObject *> gameObjects;
	class Game *game = nullptr;

	CollisionQTree collisionTree;

	WorldForces activeForces;	// Gravity, wind etc

	std::queue <CreationQueue> objectsToCreate;
	std::queue <GameObject *>  objectsToRemove;
};

