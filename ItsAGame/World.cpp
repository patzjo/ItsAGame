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
		PhysicsComponent *currentGameObjectPhysicsComponent = GO->getPhysicsComponent();

		for (auto anotherGO : otherObjects)
		{
			if (GO == anotherGO)
				continue;
			
			bool collied = false;

			PhysicsComponent *anotherGameObjectPhysicsComponent = GO->getPhysicsComponent();


			if (currentGameObjectPhysicsComponent->collisionMode == IGNORE_COLLISION || 
				anotherGameObjectPhysicsComponent->collisionMode == IGNORE_COLLISION)
				continue;
			
			if (currentGameObjectPhysicsComponent->collisionMode == CIRCLE_COLLISION && anotherGameObjectPhysicsComponent->collisionMode == CIRCLE_COLLISION)
				collied = collisionBetweenCircles(GO->position, currentGameObjectPhysicsComponent->circleCollisionRadius, anotherGO->position, anotherGameObjectPhysicsComponent->circleCollisionRadius);

			if (currentGameObjectPhysicsComponent->collisionMode == BOX_COLLISION && anotherGameObjectPhysicsComponent->collisionMode == BOX_COLLISION)
			{
				collied = currentGameObjectPhysicsComponent->getCollisionBox().boxIntersect(anotherGameObjectPhysicsComponent->getCollisionBox());
			}

			if (currentGameObjectPhysicsComponent->collisionMode == CIRCLE_COLLISION && anotherGameObjectPhysicsComponent->collisionMode == BOX_COLLISION)
			{
				collied = anotherGameObjectPhysicsComponent->getCollisionBox().circleIntersect(GO->position, currentGameObjectPhysicsComponent->circleCollisionRadius);
			}

			if (currentGameObjectPhysicsComponent->collisionMode == BOX_COLLISION && anotherGameObjectPhysicsComponent->collisionMode == CIRCLE_COLLISION)
			{
				collied = currentGameObjectPhysicsComponent->getCollisionBox().circleIntersect(anotherGO->position, anotherGameObjectPhysicsComponent->circleCollisionRadius);
			}

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

	// Collision with level

		auto levelCollisionBoxes = collisionTree.getLevelCollisionBoxes();
		switch (currentGameObjectPhysicsComponent->collisionMode)
		{
		case BOX_COLLISION:
			for (auto& levelRect : levelCollisionBoxes)
			{
				if (currentGameObjectPhysicsComponent->collisionArea.boxIntersect(levelRect) )
				{
					if ( level.getDataFrom((unsigned int)GO->position.x, (unsigned int)GO->position.y) != sf::Color::Black )
						notify(GO, E_COLLISION_WITH_LEVEL, (void*)&level);
					
					continue;
				}
			}
			break;
		
		case CIRCLE_COLLISION:
			for (auto& levelRect : levelCollisionBoxes)
			{
				if (levelRect.circleIntersect(GO->position, currentGameObjectPhysicsComponent->circleCollisionRadius) )
				{
					if ( level.getDataFrom((unsigned int)GO->position.x, (unsigned int)GO->position.y) != sf::Color::Black )
						notify(GO, E_COLLISION_WITH_LEVEL, (void*)&level);

					continue;
				}
			}
			break;

		default: break;

		}

	}
}

void World::notifySubject(int event, void *data)
{
	switch (event)
	{
	case E_REMOVE_GAMEOBJECT:
	{
		GameObject * GO = (GameObject*)data;
		removeObject(GO);
	}
		break;
	
	default: break;
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
	removeObserver(object);
	
	for (auto itr = gameObjects.begin(); itr != gameObjects.end(); itr++)
	{
		if (*itr == object)
		{
			delete *itr;
			itr = gameObjects.erase(itr);
			return;
		}
	}

}
