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



RenderComponent::RenderComponent(GameObject * Parent, Renderer * rendererPointer)
	: Component(Parent), renderer(rendererPointer)
{
	renderer->pushRenderable(this);
}

RenderComponent::~RenderComponent()
{
	renderer->removeRenderable(this);
}

PhysicsComponent::PhysicsComponent(class GameObject *Parent)
	: Component(Parent)
{

}

TestObject::TestObject(class Renderer *renderer)
{
	renderComponent = new RenderComponent(this, renderer);
	physicsComponent = new PhysicsComponent(this);

	physicsComponent->vel = { 100.0f, 100.0f };
	physicsComponent->collisionMode = CIRCLE_COLLISION;
	physicsComponent->circleCollisionRadius = 100.0f;
}

TestObject::~TestObject()
{
}

void TestObject::onNotify(GameObject * gameObject, int eventType, void * eventData)
{
	switch (eventType)
	{
	case E_START_OVERLAP:
		if (this == gameObject)
		{
			CollisionData *data = (CollisionData *)eventData;
			physicsComponent->vel *= -1;
		}
		break;

	case E_COLLISION_WITH_LEVEL:
		if (this == gameObject)
		{
			Level *levelPointer = (Level *)eventData;
			levelPointer->doCircleHole(position, physicsComponent->circleCollisionRadius, sf::Color::Black);
			subject->notifySubject(E_REMOVE_GAMEOBJECT, (void*)this);
		}
		break;

	default:break;
	}
}


void TestObject::update(float dT)
{
	physicsComponent->update(dT);
}

Rectangle<float> PhysicsComponent::getCollisionBox()
{
	Rectangle<float> area;
	area.halfSize = collisionArea.halfSize;
	area.centerPos = parent->position;

	return area;
}

void PhysicsComponent::update(float dT)
{
	parent->position += vel * dT;
	if (parent->position.x <= 0.0f || parent->position.x >= 1920)
		vel.x *= -1;

	if (parent->position.y <= 0 || parent->position.y >= 1080)
		vel.y *= -1;
}


