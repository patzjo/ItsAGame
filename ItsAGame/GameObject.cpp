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
		delete renderComponent;

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



// TestObject
TestObject::TestObject()
{
	renderComponent = new RenderComponent(this);
	physicsComponent = new PhysicsComponent(this);
	collisionComponent = new CollisionComponent(this);

	vel = { 0.0f, 0.0f };
	collisionComponent->collisionMode = CIRCLE_COLLISION;
	float colRad = 10.0f;
	collisionComponent->circleCollisionRadius = colRad;

	collisionComponent->collisionPoints.push_back({0.0f, 0.0f});		// Center point
	collisionComponent->collisionPoints.push_back({-colRad, 0.0f });	// Left point
	collisionComponent->collisionPoints.push_back({ colRad, 0.0f });	// Right point
	collisionComponent->collisionPoints.push_back({ 0.0f, -colRad });	// Top point
	collisionComponent->collisionPoints.push_back({ 0.0f, colRad });	// Bottom point
}

TestObject::~TestObject()
{
}

void TestObject::onNotify(GameObject * gameObject, int eventType, void * eventData)
{
	if (this == gameObject)
	{
		switch (eventType)
		{
		case E_START_OVERLAP:
		{
				CollisionData *data = (CollisionData *)eventData;
		} break;

		case E_COLLISION_WITH_LEVEL:
		{
				Level *levelPointer = (Level *)eventData;
				levelPointer->doCircleHole(position, 100, sf::Color::Black);
				subject->notifySubject(E_REMOVE_GAMEOBJECT, (void*)this);

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


void TestObject::update(class World *world, float dT)
{
	physicsComponent->update(world, dT);
}


// Player
Player::Player(std::string Name)
{
	name = Name;
	renderComponent = new RenderComponent(this);
	collisionComponent = new CollisionComponent(this);
	physicsComponent = new PlayerPhysicsComponent(this);

	renderComponent->type = RENDER_SPRITE;
	
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
			// Cant be inside of level, popping gameObject off
//			if (!inGround)
//			{
				handleLevelCollision((Level *)eventData);

//			}
//			else
//				inGround = false;


			break;
		}
	}
}

void Player::handleLevelCollision(Level *level)
{
	for (auto collisionPoint : collisionComponent->collisionPoints)
	{
		unsigned int XCheck = (unsigned int)(collisionPoint.x + position.x);
		unsigned int YCheck = (unsigned int)(collisionPoint.y + position.y);
		while (level->getDataFrom(XCheck, YCheck) != sf::Color::Black)
		{
			inGround = true;
			position.y--;
			YCheck = (unsigned int)((int)collisionPoint.y + (int)position.y);
		} 
	
	}
}

void Player::update(class World *world, float dT)
{
	physicsComponent->update(world, dT);
}

void PlayerPhysicsComponent::update(World * world, float dT)
{
	parent->vel += world->getGravity() * dT;

	if (((Player *)parent)->inGround)
	{
		if (parent->vel.y > 0.0f)
			parent->vel.y = (int)0;
		((Player *)parent)->inGround = false;
	}

	parent->position += parent->vel;

	if (parent->position.y > 4000.0f || parent->position.x < -2000.0f || parent->position.x > 10000.0f || parent->position.y < -10000.f)
		world->queueToRemove(parent);

}
