#include "Renderer.h"
#include "GameObject.h"
#include "Level.h"
#include "Utils.h"
#include "World.h"

// GameObject
GameObject::GameObject()
{
}


GameObject::~GameObject()
{
	if (renderComponent)
	{
		for (auto i : renderComponent->graphics)
			delete i;
		delete renderComponent;
	}

	if (physicsComponent)
		delete physicsComponent;
	
	if (collisionComponent)
		delete collisionComponent;

	collisionComponent = nullptr;
	physicsComponent   = nullptr;
	renderComponent    = nullptr;
}

void GameObject::update(class World *world, float dT )
{
	
}



// RenderComponent
RenderComponent::RenderComponent(GameObject * Parent)
	: Component(Parent)
{
}

RenderComponent::~RenderComponent()
{
}



// PhysicsComponent
PhysicsComponent::PhysicsComponent(class GameObject *Parent)
	: Component(Parent)
{

}

void PhysicsComponent::update(class World *world, float dT)
{
	parent->vel += world->getGravity() * dT;
	parent->vel += world->getWind() * dT;
	parent->position += parent->vel;
	if (parent->position.y > 4000.0f || parent->position.x < -2000.0f || parent->position.x > 10000.0f || parent->position.y < -10000.f)
		world->queueToRemove(parent);
}


// CollisionComponent
CollisionComponent::CollisionComponent(class GameObject *Parent)
	: Component(Parent)
{

}

Rectangle<float> CollisionComponent::getCollisionBox()
{
	Rectangle<float> area;
	area.halfSize = collisionArea.halfSize;
	area.centerPos = parent->position;

	return area;
}

bool CollisionComponent::checkCollision(GameObject *object)
{
	if (object == nullptr || !object->active)
		return false;

	bool result = false;

	CollisionComponent *otherCollisionComponent = object->getCollisionComponent();
	
	if (otherCollisionComponent == nullptr)
		return false;

	// Circle collision
	if (collisionMode == CIRCLE_COLLISION && otherCollisionComponent->collisionMode == CIRCLE_COLLISION)
		result = collisionBetweenCircles(parent->position, circleCollisionRadius, object->position, otherCollisionComponent->circleCollisionRadius);

	// Box collision
	if (collisionMode == BOX_COLLISION && otherCollisionComponent->collisionMode == BOX_COLLISION)
		result = getCollisionBox().boxIntersect(otherCollisionComponent->getCollisionBox());

	// Circle collision vs Box collision
	if (collisionMode == CIRCLE_COLLISION && otherCollisionComponent->collisionMode == BOX_COLLISION)
		result = otherCollisionComponent->getCollisionBox().circleIntersect(object->position, circleCollisionRadius);

	// Box collision vs Circle collision
	if (collisionMode == BOX_COLLISION && otherCollisionComponent->collisionMode == CIRCLE_COLLISION)
		result = getCollisionBox().circleIntersect(object->position, otherCollisionComponent->circleCollisionRadius);

	return result;
}


void CollisionComponent::addOverlappingObject(GameObject * GO)
{
	OverlappingWrapper olw;
	olw.object = GO;
	olw.time = 0.0f;
	overlappedObjects.push_back(olw);
}

void CollisionComponent::removeOverlappingObject(GameObject * GO)
{
	for (auto itr = overlappedObjects.begin(); itr != overlappedObjects.end(); itr++)
	{
		if (itr->object == GO )
		{
			overlappedObjects.erase(itr);
			return;
		}
	}
}

bool CollisionComponent::checkOverlapping(GameObject * other)
{
	for (auto& olw : overlappedObjects)
	{
		if (olw.object == other)
			return true;
	}
	return false;
}

void CollisionComponent::addCollisionPoint(sf::Vector2f point)
{
	collisionPoints.push_back(point);
}

void CollisionComponent::increaseOverlapTime(GameObject * GO, float dT)
{
	for (auto i : overlappedObjects)
	{
		if (i.object == GO)
			i.time += dT;
	}
}



// Cannonball
CannonBall::CannonBall()
{
	renderComponent = new RenderComponent(this);
	physicsComponent = new PhysicsComponent(this);
	collisionComponent = new CollisionComponent(this);

	vel = { 0.0f, 0.0f };
	collisionComponent->collisionMode = CIRCLE_COLLISION;

	collisionComponent->circleCollisionRadius = size*2;
	collisionComponent->collisionPoints.push_back({0.0f, 0.0f});	// Center point
	collisionComponent->collisionPoints.push_back({-size, 0.0f });	// Left point
	collisionComponent->collisionPoints.push_back({ size, 0.0f });	// Right point
	collisionComponent->collisionPoints.push_back({ 0.0f, -size});	// Top point
	collisionComponent->collisionPoints.push_back({ 0.0f, size});	// Bottom point

	Graphics *graphics = new Graphics;

	graphics->radius = size;
	graphics->type = RENDER_SHAPE;
	graphics->shape = CIRCLE;
	graphics->fillColor = sf::Color::Red;

	renderComponent->graphics.push_back(graphics);
}

CannonBall::~CannonBall()
{
}

void CannonBall::onNotify(GameObject * gameObject, int eventType, void * eventData)
{
	int soundIndex = 1;
	if (this == gameObject)
	{
		switch (eventType)
		{
		case E_START_OVERLAP:
		{
			processCollision((CollisionData *)eventData);
		} break;

		case E_COLLISION_WITH_LEVEL:
		{
			Level *levelPointer = ((CollisionData *)eventData)->level;
			levelPointer->doCircleHole(position, explosionRadius, sf::Color(0,0,0,0));
		
			Rectangle<float> areaToLook(position, { explosionRadius*2, explosionRadius*2 });
			std::vector <GameObject *> objectsInArea = ((CollisionData *)eventData)->world->getObjectsInRange(areaToLook);

			for (GameObject *object : objectsInArea)
			{
				if (object->getType() == PLAYER)
				{
					float radius = explosionRadius * explosionRadius * 2;
					sf::Vector2f distance = position - object->position;
					float distanceSquared = distance.x *distance.x + distance.y + distance.y;
//					std::cout << "Distance: " << distanceSquared << " radius: " << radius << " multiply: " << distanceSquared/radius<< std::endl;
					if (distanceSquared <= radius)
					{
						((Player *)object)->takeDamage(owner, (radius - distanceSquared)/radius * damage);
					}
				}
			}

			ExplosionData explosionData{ position, 0 };
			subject->notifySubject(E_SPAWN_EXPLOSION, &explosionData);

			subject->notifySubject(E_PLAY_SOUND, &soundIndex);
			subject->notifySubject(E_REMOVE_GAMEOBJECT, this);
		} break;

		case E_ANOTHER_GAMEOBJECT_COLLIED:
		{
			ExplosionData explosionData{ position, 0 };
			subject->notifySubject(E_SPAWN_EXPLOSION, &explosionData);
			subject->notifySubject(E_PLAY_SOUND, &soundIndex);
			subject->notifySubject(E_REMOVE_GAMEOBJECT, this);
		} break;

		default:break;
		}
	}
	else
	{
		switch (eventType)
		{
		case E_INFORM_GAMEOBJECT_REMOVED:
			collisionComponent->removeOverlappingObject((GameObject *)eventData);
			break;

		default:
			break;
		}
	}
}


void CannonBall::update(class World *world, float dT)
{
	physicsComponent->update(world, dT);
}

void CannonBall::processCollision(CollisionData * collisionData)
{
	if (collisionData->colliedGameObject != this->getOwner())
	{
		int soundIndex = 1;
		CollisionData data;
		data.gameObject			= collisionData->colliedGameObject;
		data.colliedGameObject	= collisionData->gameObject;
		data.world				= collisionData->world;

		if (collisionData->colliedGameObject->getType() == PLAYER)
		{
			Player *player = (Player *)collisionData->colliedGameObject;
			player->takeDamage(collisionData->gameObject->getOwner(), damage);
		}
		subject->notify(data.gameObject, E_ANOTHER_GAMEOBJECT_COLLIED, &data);

		ExplosionData explosionData{ position, 0 };
		subject->notifySubject(E_SPAWN_EXPLOSION, &explosionData);
		subject->notifySubject(E_PLAY_SOUND, &soundIndex);
		subject->notifySubject(E_REMOVE_GAMEOBJECT, this);



	}
}


// Player
Player::Player(std::string Name)
{
	type = PLAYER;
	name = Name;
	renderComponent =	 new RenderComponent(this);
	collisionComponent = new CollisionComponent(this);
	physicsComponent =   new PlayerPhysicsComponent(this);
	inputComponent	   = new InputComponent(this);

	cannonAngle = 90.0f;

	Graphics *graphics = new Graphics;
	graphics->type = RENDER_SPRITE;
	graphics->textureID = 0; // Assets.h
	renderComponent->graphics.push_back(graphics);

	cannon = new Graphics;
	cannon->type = RENDER_SHAPE;
	cannon->shape = RECTANGLE;
	cannon->rect = { {0.0f, 0.0f}, { 35.0f, 5.0f } };
	cannon->origin = { 0.0f, cannon->rect.halfSize.y*0.5f };
	cannon->center = false;
	cannon->angle = 360.0f-cannonAngle;
	cannon->offset = {8.0f, -15.0f};
	cannon->fillColor = sf::Color(0, 100, 0);
	cannon->outlineColor = sf::Color(0, 150, 0);
	renderComponent->graphics.push_back(cannon);

	Graphics *graphics2 = new Graphics;
	graphics2->type = RENDER_SHAPE;
	graphics2->shape = CIRCLE;
	graphics2->radius = 8.0f;
	graphics2->offset = cannon->offset;
	graphics2->fillColor = sf::Color(0, 50, 0);
	renderComponent->graphics.push_back(graphics2);

	collisionComponent->addCollisionPoint(sf::Vector2f(0.0f, 0.0f));	 // Center point
	
	collisionComponent->addCollisionPoint(sf::Vector2f(-40.0f, 25.0f));  // Bottom left
	collisionComponent->addCollisionPoint(sf::Vector2f(0.0f, 25.0f));    // bottom Center
	collisionComponent->addCollisionPoint(sf::Vector2f(40.0f, 25.0f));   // Bottom right
	
	collisionComponent->addCollisionPoint(sf::Vector2f(-40.0f, 0.0f));	 // Left
	collisionComponent->addCollisionPoint(sf::Vector2f(40.0f, 0.0f));	 // Right

	collisionComponent->collisionMode = BOX_COLLISION;
	collisionComponent->collisionArea.centerPos = position;
	collisionComponent->collisionArea.halfSize = { 40.0f, 25.0f };

	
}


void Player::onNotify(GameObject * gameObject, int eventType, void * eventData)
{
	if (gameObject == this)
	{
		switch (eventType)
		{
		case E_START_OVERLAP:
			break;
	
		case E_COLLISION_WITH_LEVEL:
				handleLevelCollision(((CollisionData *)eventData)->level);
			break;

		default: 
			break;
		}
	}
}

void Player::handleLevelCollision(Level *level)
{
	int point = 0;
	int biggestClimbAmount = 0;
	for (auto collisionPoint : collisionComponent->collisionPoints)
	{
		int climbAmount = 0;
		unsigned int XCheck = (unsigned int)collisionPoint.x + (unsigned int)position.x;
		unsigned int YCheck = (unsigned int)collisionPoint.y + (unsigned int)position.y;

		while (level->getDataFrom(XCheck, YCheck) != sf::Color(0, 0, 0, 0))
		{
			climbAmount++;
			YCheck = (unsigned int)(collisionPoint.y + position.y - climbAmount);

			falling = false;
		}
		point++;

		if (climbAmount > biggestClimbAmount)
			biggestClimbAmount = climbAmount;
	}

	if (biggestClimbAmount > 20)	// Do not climb too steep wall
		position = ((PlayerPhysicsComponent *)getPhysicsComponent())->oldPosition;
	else
		position.y = position.y - biggestClimbAmount;

}

void Player::shoot(World * world, float dT)
{
	if (loadingTime < fireRate)
		return;
	
	CannonBall *newCannonBall = new CannonBall;
	newCannonBall->setOwner(this);

	float sinAngle = -sinf(toRad(cannonAngle));
	float cosAngle = cosf(toRad(cannonAngle));
    	
	newCannonBall->vel = {
		cosAngle*cannonPower,
		sinAngle*cannonPower
	};
   	newCannonBall->position = {
		position.x+cannon->offset.x + (cosAngle * 30.0f),
		position.y+cannon->offset.y + (sinAngle * 30.0f)
	};

	world->createObject(newCannonBall->position, newCannonBall);
	loadingTime = 0.0f;
	int soundIndex = 0;
	world->notifySubject(E_PLAY_SOUND, (void*)(&soundIndex));
}

void Player::moveCannonAngleUp(float dT)
{
	cannonAngle += cannonAngleSpeed * dT;

	if (cannonAngle > 360.0f)
		cannonAngle = (float)((int)cannonAngle % 360);

	cannon->angle = 360-cannonAngle;
}

void Player::moveCannonAngleDown(float dT)
{
	cannonAngle -= cannonAngleSpeed * dT;

	if (cannonAngle < 0)
		cannonAngle = 360.0f + cannonAngle;

	cannon->angle = 360-cannonAngle;
}

void Player::takeDamage(GameObject *from, float amount)
{
	health -= amount;
	if (health <= 0)
	{
		ExplosionData explosionData{ position, 1 };
		subject->notifySubject(E_SPAWN_EXPLOSION, &explosionData);
		PlayerKilledData data{ from, this };
		subject->notifySubject(E_REMOVE_PLAYER, &data);
	}
}

void Player::update(class World *world, float dT)
{
	if ( loadingTime < fireRate )
		loadingTime += dT;
	physicsComponent->update(world, dT);
	
	if (getRenderComponent()->renderer)
	{
		getRenderComponent()->renderer->pushText(name, position + sf::Vector2f(0.0f, -120.0f), 0, 16, sf::Color::Yellow, true);
		getRenderComponent()->renderer->pushText(std::to_string((int)health), position + sf::Vector2f(0.0f, -100.0f), 0, 16, sf::Color::Green, true);
	}
}

void PlayerPhysicsComponent::update(World * world, float dT)
{
	oldPosition = parent->position;
	parent->vel += world->getGravity() * dT;
	parent->vel += acc * dT;
	

	if (!parent->falling)
	{
		if (parent->vel.y > 1.0f)
			parent->vel.y = (int)1;
	}
	
	parent->position += parent->vel;
	parent->vel.x = 0;
	acc.x = 0;
	acc.y = 0;

	if (parent->position.y > 4000.0f || parent->position.x < -2000.0f || parent->position.x > 10000.0f || parent->position.y < -10000.f)
	{
		PlayerKilledData data{ (Player *)parent, (Player *)parent };
		((Player *)parent)->getSubject()->notifySubject(E_REMOVE_PLAYER, &data);
	}

}

InputComponent::InputComponent(GameObject * Parent) : Component(Parent)
{
}

void InputComponent::pushCommand(sf::Keyboard::Key Key, class Command * commandToExecute)
{
	commands.push_back({ Key, commandToExecute });
}

InputComponent::~InputComponent()
{
	for (auto i : commands)
	{
		if (i.command)
			delete i.command;

	}
}

Explosion::Explosion()
{
	renderComponent = new RenderComponent(this);
	Graphics *graphics = new Graphics;
	

	renderComponent->animHeight = 256;
	renderComponent->animWidth = 256;
	renderComponent->currentFrame = 0;
	renderComponent->numAnimFrames = 64;
	renderComponent->timePerFrame = 0.25f;

	graphics->type = RENDER_ANIM;
	graphics->textureID = 1;
	
	graphics->vertices.setPrimitiveType(sf::Quads);
	graphics->vertices.resize(4);

	renderComponent->graphics.push_back(graphics);
}

Explosion::~Explosion()
{
}

void Explosion::onNotify(GameObject * gameObject, int evenType, void * eventData)
{
}

void Explosion::update(World * world, float dT)
{
}

Explosion2::Explosion2()
{
	renderComponent = new RenderComponent(this);
	Graphics *graphics = new Graphics;


	renderComponent->animHeight = 512;
	renderComponent->animWidth = 512;
	renderComponent->currentFrame = 0;
	renderComponent->numAnimFrames = 64;
	renderComponent->timePerFrame = 0.25f;

	graphics->type = RENDER_ANIM;
	graphics->textureID = 3;

	graphics->vertices.setPrimitiveType(sf::Quads);
	graphics->vertices.resize(4);

	renderComponent->graphics.push_back(graphics);
}

Explosion2::~Explosion2()
{
}
