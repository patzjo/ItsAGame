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
	gameObjects.erase(std::remove_if(gameObjects.begin(), gameObjects.end(), [](GameObject *obj) { bool result = !obj->active; if (result) { delete obj; std::cout << "VITTU" << std::endl; } return result; }), gameObjects.end());
	for(auto itr = gameObjects.begin(); itr != gameObjects.end(); itr++ )
	{
		if (!(*itr)->active) continue;
		GameObject *GO = *itr;
		CollisionComponent *currentGameObjectCollisionComponent = GO->getCollisionComponent();

		float areaToLookX = 0.0f;
		float areaToLookY = 0.0f;

		if (currentGameObjectCollisionComponent->collisionMode == CIRCLE_COLLISION)
		{
			areaToLookY = areaToLookX = currentGameObjectCollisionComponent->circleCollisionRadius * 2.5f;
		}
		else if (currentGameObjectCollisionComponent->collisionMode == BOX_COLLISION)
		{
			areaToLookX = currentGameObjectCollisionComponent->getCollisionBox().halfSize.x*2.5f;
			areaToLookY = currentGameObjectCollisionComponent->getCollisionBox().halfSize.y*2.5f;
		}

		// Get objects that are close enought for collision
		Rectangle<float> areaToLook = { GO->position, {areaToLookX, areaToLookY} }; 
		std::vector <GameObject *> otherObjects = collisionTree.getObjectsInRange(areaToLook);

		for( auto itr2 = otherObjects.begin(); itr2 != otherObjects.end(); itr2++)
		{
			if (!(*itr2)->active) continue;

			GameObject *anotherGO = *itr2;
			if (GO == anotherGO)
				continue;
			
			bool collied = false;

			CollisionComponent *anotherGameObjectCollisionComponent = GO->getCollisionComponent();

			if (currentGameObjectCollisionComponent->collisionMode == IGNORE_COLLISION || 
				anotherGameObjectCollisionComponent->collisionMode == IGNORE_COLLISION)
				continue;
			
			collied = currentGameObjectCollisionComponent->checkCollision(anotherGO);

			if (collied)
			{
				if (!currentGameObjectCollisionComponent->checkOverlapping(anotherGO))
				{
					CollisionData curGOData;
					CollisionData anotherGOData;
			
					curGOData.colliedGameObject = anotherGO;
					anotherGOData.colliedGameObject = GO;

					currentGameObjectCollisionComponent->addOverlappingObject(anotherGO);

					notify(GO, E_START_OVERLAP, &curGOData);
	//				notify(anotherGO, E_START_OVERLAP, &anotherGOData);
				}
			}
			else
			{
				if (currentGameObjectCollisionComponent->checkOverlapping(anotherGO))
				{
					currentGameObjectCollisionComponent->removeOverlappingObject(anotherGO);
					notify(GO, E_END_OVERLAP, anotherGO);
				}
			}
		}

	// Collision with level

		if (!(*itr)->active) continue; // If current gameObject is qued for deletion, continue itertion with next object

		auto levelCollisionBoxes = collisionTree.getLevelCollisionBoxes();
		switch (currentGameObjectCollisionComponent->collisionMode)
		{
		case BOX_COLLISION:
			for (auto& levelRect : levelCollisionBoxes)
			{
				if (currentGameObjectCollisionComponent->collisionArea.boxIntersect(levelRect) )
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
				if (levelRect.circleIntersect(GO->position, currentGameObjectCollisionComponent->circleCollisionRadius) )
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
		notify(nullptr, E_INFORM_GAMEOBJECT_REMOVED, GO);
		queueToRemove(GO);
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

void World::queueToRemove(GameObject * object)
{
	removeObserver(object);
	collisionTree.remove(object);
	object->active = false;
}
