#pragma once

#include "Collision.h"
#include "Level.h"
#include "GameObject.h"
#include "Obspat.h"

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

private:
	Level level;
	std::vector <GameObject *> gameObjects;
	class Game *game = nullptr;

	CollisionQTree collisionTree;
};

