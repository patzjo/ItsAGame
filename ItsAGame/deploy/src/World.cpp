#include "Game.h"
#include "World.h"
#include "Utils.h"

World::World()
{
	activeForces.gravity.y = 9.81f;
	std::cout << "Gravity: " << activeForces.gravity << std::endl;
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

void World::startAgain()
{
	float xPos = 200.0f;
	game->playerCount = 0;
	for (int c = 0; c < game->options.players; c++)
	{
		if (game->players[c]) delete game->players[c];

		std::string name = "P";
		name += std::to_string(c + 1);
		if ((c % 2) == 0)
			game->players[game->playerCount++] = (Player *)createObject({ 1920 - xPos, 400.0f }, new Player(name));
		else
		{
			game->players[game->playerCount++] = (Player *)createObject({ xPos, 400.0f }, new Player(name));
			xPos = 200.0f*(c - 1);
		}

	}
	game->input.setKeysFromOptions(game);
}

void World::update(float dT)
{
	for (auto go : gameObjects)
		go->update(this, dT);

	collisionTree.updateTree();

	checkCollisions(dT);
}

void World::checkCollisions(float dT)
{
	auto levelCollisionBoxes = collisionTree.getLevelCollisionBoxes();

	for(auto itr = gameObjects.begin(); itr != gameObjects.end(); itr++ )
	{
		if (!(*itr)->active) continue;
		GameObject *GO = *itr;
		CollisionComponent *currentGameObjectCollisionComponent = GO->getCollisionComponent();
		if (!currentGameObjectCollisionComponent)
			continue;

		float areaToLookX = 0.0f;
		float areaToLookY = 0.0f;

		if (currentGameObjectCollisionComponent->collisionMode == CIRCLE_COLLISION)
		{
			areaToLookY = areaToLookX = currentGameObjectCollisionComponent->circleCollisionRadius * 5.0f;
		}
		else if (currentGameObjectCollisionComponent->collisionMode == BOX_COLLISION)
		{
			areaToLookX = currentGameObjectCollisionComponent->getCollisionBox().halfSize.x*5.0f;
			areaToLookY = currentGameObjectCollisionComponent->getCollisionBox().halfSize.y*5.0f;
		}

		// Get objects that are close enought for collision
		Rectangle<float> areaToLook = { GO->position, {areaToLookX, areaToLookY} }; 
		std::vector <GameObject *> otherObjects = collisionTree.getObjectsInRange(areaToLook);

		for( auto itr2 = otherObjects.begin(); itr2 != otherObjects.end(); itr2++)
		{
			if (!(*itr2)->active) continue;

			GameObject *anotherGO = *itr2;
			if (GO == anotherGO || GO == anotherGO->getOwner())
				continue;
			
			bool collied = false;

			CollisionComponent *anotherGameObjectCollisionComponent = GO->getCollisionComponent();

			if (currentGameObjectCollisionComponent->collisionMode == IGNORE_COLLISION || 
				anotherGameObjectCollisionComponent->collisionMode == IGNORE_COLLISION)
				continue;
			
			// Check collision
			collied = currentGameObjectCollisionComponent->checkCollision(anotherGO);

			// Process collision
			if (collied)
			{
				// If object is overlapping with another object
				if (!currentGameObjectCollisionComponent->checkOverlapping(anotherGO))
				{
					CollisionData curGOData;
					CollisionData anotherGOData;
			
					curGOData.colliedGameObject = anotherGO;
					curGOData.gameObject = GO;
					curGOData.world = this;
					
					anotherGOData.colliedGameObject = GO;
					anotherGOData.gameObject = anotherGO;
					anotherGOData.world = this;

					currentGameObjectCollisionComponent->addOverlappingObject(anotherGO);

					notify(GO, E_START_OVERLAP, &curGOData);
				}
				else // If object is not overlapping with another object
					currentGameObjectCollisionComponent->increaseOverlapTime(anotherGO, dT); // Increase time

			}
			else // If not collied, check if they have been overlapped.
			{
				if (currentGameObjectCollisionComponent->checkOverlapping(anotherGO))
				{
					// Report that that these objects aint overlapping anymore.
					currentGameObjectCollisionComponent->removeOverlappingObject(anotherGO);
					notify(GO, E_END_OVERLAP, anotherGO);
				}
			}
		}

	// Collision with level

		if (!(*itr)->active) continue; // If current gameObject is qued for deletion, continue itertion with next object
		switch (currentGameObjectCollisionComponent->collisionMode)
		{
		case BOX_COLLISION:
			GO->falling = true;
			for (auto& levelRect : levelCollisionBoxes)
			{
				if (currentGameObjectCollisionComponent->collisionArea.boxIntersect(levelRect, GO->position) )
				{
					for (auto& point : currentGameObjectCollisionComponent->collisionPoints)
					{
						sf::Vector2f pointToCheck = (point + GO->position);
						if (level.getDataFrom((unsigned int)pointToCheck.x, (unsigned int)pointToCheck.y) != sf::Color(0,0,0,0))
						{
							CollisionData data;
							data.level = &level;
							data.world = this;
							notify(GO, E_COLLISION_WITH_LEVEL, &data);
							break;
						}
					}
					continue;
				}
			}
			break;
		
		case CIRCLE_COLLISION:
			GO->falling = true;
			for (auto& levelRect : levelCollisionBoxes)
			{
				if (levelRect.circleIntersect(GO->position, currentGameObjectCollisionComponent->circleCollisionRadius) )
				{
					for (auto& point : currentGameObjectCollisionComponent->collisionPoints)
					{
						sf::Vector2f pointToCheck = point + GO->position;
						if (level.getDataFrom((unsigned int)pointToCheck.x, (unsigned int)pointToCheck.y) != sf::Color(0, 0, 0, 0))
						{
							CollisionData data;
							data.level = &level;
							data.world = this;
							notify(GO, E_COLLISION_WITH_LEVEL, &data);
							break;
						}
					}

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
	} break;
	
	case E_REMOVE_PLAYER:
	{
		
		Player *player = ((PlayerKilledData *)data)->player;;
		player->active = false;
		for (unsigned int c = 0; c < game->getPlayerCount(); c++)
		{
			if (game->players[c] == player)
			{
				std::string playerName = game->players[c]->getName();
				notify(nullptr, E_INFORM_GAMEOBJECT_REMOVED, player);
				queueToRemove(player);
				game->players[c] = nullptr;
			}
		}
		game->updateGameSituation(((PlayerKilledData *)data));
	}break;

	case E_PLAY_SOUND:
	{
		game->sound.playSound(*(int *)data);
	} break;

	case E_SPAWN_EXPLOSION:
	{
		switch (((ExplosionData *)data)->type)
		{
		case 1:
			queueToCreate(((ExplosionData *)data)->position, new Explosion2());
			break;
		
		case 0:
		default:
			queueToCreate(((ExplosionData *)data)->position, new Explosion());
			break;
		}

	} break;
	
	case E_START_AGAIN:
	{
		for (unsigned int c = 0; c < gameObjects.size(); c++)
			switch (gameObjects[c]->getType())
			{
			case PLAYER:
			{
				PlayerKilledData data;
				data.whoKilled = nullptr;
				data.player = (Player *)gameObjects[c];
				notifySubject(E_REMOVE_PLAYER, &data);
			} break;
			
			default:
				notifySubject(E_REMOVE_GAMEOBJECT, gameObjects[c]);
				break;
			}
	} break;

	default: break;
	}
}

void World::applyWind(int left, int right)
{
	activeForces.wind.x = -left + rand() % (left+right);
}


GameObject* World::createObject(sf::Vector2f position, GameObject * object)
{
	object->setPosition(position);
	gameObjects.push_back(object);
	collisionTree.add(object);
	this->addObserver(object);
	game->renderer.pushRenderable(object->getRenderComponent());
	
	return object;
}

void World::queueToRemove(GameObject * object)
{

	/*
	for ( auto itr = gameObjects.begin(); itr != gameObjects.end(); itr ++)
		if (*itr == object)
		{
			itr = gameObjects.erase(itr);
			break;
		}
		*/
	object->active = false;
	objectsToRemove.push(object);
}

void World::queueToCreate(sf::Vector2f position, GameObject *object)
{
	CreationQueue que;

	que.object = object;
	que.position = position;
	objectsToCreate.push(que);
}

void World::processQueues()
{
	while (!objectsToRemove.empty())
	{
		
		removeObserver(objectsToRemove.front());
		game->renderer.removeRenderable(objectsToRemove.front()->getRenderComponent());
		//		collisionTree.remove(objectsToRemove.front());

		objectsToRemove.pop();
	}

	gameObjects.erase(std::remove_if(gameObjects.begin(), gameObjects.end(), [](GameObject *obj) { bool result = !obj->active; if (result) delete obj; return result; }), gameObjects.end());	// Remove destroyed objects

	// Create queuedObjects
	while (!objectsToCreate.empty())
	{
		createObject(objectsToCreate.front().position, objectsToCreate.front().object);
		objectsToCreate.pop();
	}
}



