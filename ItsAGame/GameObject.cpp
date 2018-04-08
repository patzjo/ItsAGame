#include "Renderer.h"
#include "GameObject.h"
#include "Level.h"
#include "Utils.h"


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
	physicsComponent = nullptr;
	renderComponent = nullptr;
	
	active = false;
	std::cout << "Dying!" << std::endl;
}

void GameObject::update( float dT )
{
	
}

void Player::onNotify(GameObject * gameObject, int eventType, void * eventData)
{
	switch (eventType)
	{
	case E_START_OVERLAP:
		checkCollision(gameObject);
		break;
	}
}

void Player::checkCollision(GameObject * other) 
{
	
}


// RenderComponent
RenderComponent::RenderComponent(GameObject * Parent, Renderer * rendererPointer)
	: Component(Parent), renderer(rendererPointer)
{
	renderer->pushRenderable(this);
}

RenderComponent::~RenderComponent()
{
	renderer->removeRenderable(this);
}



// PhysicsComponent
PhysicsComponent::PhysicsComponent(class GameObject *Parent)
	: Component(Parent)
{

}

void PhysicsComponent::update(float dT)
{
	parent->position += parent->vel * dT;
	if (parent->position.x <= 0.0f || parent->position.x >= 1920)
		parent->vel.x *= -1;

	if (parent->position.y <= 0 || parent->position.y >= 1080)
		parent->vel.y *= -1;
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



// TestObject
TestObject::TestObject(class Renderer *renderer)
{
	renderComponent = new RenderComponent(this, renderer);
	physicsComponent = new PhysicsComponent(this);
	collisionComponent = new CollisionComponent(this);

	vel = { 100.0f, 100.0f };
	collisionComponent->collisionMode = CIRCLE_COLLISION;
	collisionComponent->circleCollisionRadius = 10.0f;
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
				vel *= -1;
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


void TestObject::update(float dT)
{
	physicsComponent->update(dT);
}




