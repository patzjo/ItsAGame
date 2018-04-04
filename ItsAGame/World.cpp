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
		go->update(dT);

	collisionTree.updateTree();

	checkCollisions();
}

void World::checkCollisions()
{
	for (auto GO : gameObjects)
	{
		Square<float> areaToLook = { GO->position, {50.0f, 50.0f} }; // TODO(Jonne): Remember to fix this to bounding box
		std::vector <GameObject *> otherObjects = collisionTree.getObjectsInRange(areaToLook);
		for (auto anotherGO : otherObjects)
		{
			if (GO == anotherGO)
				continue;

			// TODO(Jonne): Collision messaging here.
			CollisionData data;
			data.colliedGameObject = anotherGO;
			
			notify(GO, E_START_OVERLAP, &data);
		}
	}
}

GameObject* World::createObject(sf::Vector2f position, GameObject * object)
{
	object->setPosition(position);
	gameObjects.push_back(object);
	collisionTree.add(object);
	this->addObserver(object);

	return object;
}

void World::removeObject(GameObject * object)
{
	

	for (auto itr = gameObjects.begin(); itr != gameObjects.end(); itr++)
	{
		if (*itr == object)
		{
			gameObjects.erase(itr);
			return;
		}
	}
}
