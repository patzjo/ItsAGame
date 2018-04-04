#include "Game.h"
#include "World.h"



World::World()
{
}


World::~World()
{
	for (auto object : gameObjects)
	{
		if ( object )
			delete object;
	}
}

void World::initialize()
{
	collisionTree.initialize(5, 4, { { (float)game->options.levelWidth / 2.0f, (float)game->options.levelHeight / 2.0f },{(float)game->options.levelWidth/2.0f, (float)game->options.levelHeight/2.0f} });
}

void World::update(float dT)
{
	for (auto go : gameObjects)
	{
		go->update(dT);
	}
}

void World::checkCollisions()
{
	for (auto go : gameObjects)
	{
		
	}
}

GameObject* World::createObject(sf::Vector2f position, GameObject * object)
{
	object->setPosition(position);
	gameObjects.push_back(object);
	collisionTree.add(object);

	return object;
}
