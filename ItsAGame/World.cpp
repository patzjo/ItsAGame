#include "Game.h"
#include "World.h"
#include "Utils.h"




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
		Rectangle<float> areaToLook = { GO->position, {50.0f, 50.0f} }; // TODO(Jonne): Remember to fix this to bounding box
		std::vector <GameObject *> otherObjects = collisionTree.getObjectsInRange(areaToLook);
		for (auto anotherGO : otherObjects)
		{
			if (GO == anotherGO)
				continue;
			
			bool collied = false;

			PhysicsComponent *thisone = GO->getPhysicsComponent();
			PhysicsComponent *otherone = GO->getPhysicsComponent();


			if (thisone->collisionMode == IGNORE_COLLISION || otherone->collisionMode == IGNORE_COLLISION)
				continue;
			
			if (thisone->collisionMode == CIRCLE_COLLISION && otherone->collisionMode == CIRCLE_COLLISION)
				collied = collisionBetweenCircles(GO->position, thisone->circleCollisionRadius, anotherGO->position, otherone->circleCollisionRadius);

			if (collied)
			{
				CollisionData curGOData;
				CollisionData anotherGOData;
			
				curGOData.colliedGameObject = anotherGO;
				anotherGOData.colliedGameObject = GO;

				notify(GO, E_START_OVERLAP, &curGOData);
//				notify(anotherGO, E_START_OVERLAP, &anotherGOData);
			}
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
