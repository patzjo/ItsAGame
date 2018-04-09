#pragma once

#include "Collision.h"
#include "Level.h"
#include "GameObject.h"
#include "Obspat.h"

struct WorldForces
{
	sf::Vector2f wind;
	sf::Vector2f gravity;
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
	
	void update(float dT);
	void checkCollisions();
	
	GameObject *createObject(sf::Vector2f position, GameObject *object);
	void queueToRemove(GameObject *object);
	
	void notifySubject(int event, void *data);

	sf::Vector2f getGravity()	{ return activeForces.gravity; }
	sf::Vector2f getWind()		{ return activeForces.wind; }

private:
	Level level;
	std::vector <GameObject *> gameObjects;
	class Game *game = nullptr;

	CollisionQTree collisionTree;

	WorldForces activeForces;
};

