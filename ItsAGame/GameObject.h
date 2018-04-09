#pragma once
#include <SFML/Graphics.hpp>
#include "Obspat.h"
#include "Utils.h"
enum CollisionMode { IGNORE_COLLISION, CIRCLE_COLLISION, BOX_COLLISION };

struct OverlappingWrapper
{
	float time;
	class GameObject *object;
};


// COMPONENTS ////////////////////////////////////////////////////////////////////////////
struct Component
{
	friend class GameObject;
	Component(class GameObject *Parent) { parent = Parent; }

	class GameObject *parent = nullptr;
	virtual void update() {};
};

struct RenderComponent : public Component
{
	RenderComponent(class GameObject *Parent, class Renderer *rendererPointer);
	~RenderComponent();

	float time = 0.0f;
	float timePerFrame = 0.5f;

	int numAnimFrames = 0;
	int anim[10]; 
	
	int currentFrame;

	class Renderer *renderer = nullptr;

	sf::Sprite sprite;
};

struct PhysicsComponent : public Component
{
	PhysicsComponent(class GameObject *Parent);

	sf::Vector2f acc;
	virtual void update(class World *world, float dT );
};

struct CollisionComponent : public Component
{
	CollisionComponent(class GameObject *Parent);
	
	Rectangle<float> collisionArea;
	float circleCollisionRadius;

	int collisionMode = BOX_COLLISION;
	Rectangle<float> getCollisionBox();

	bool checkCollision(class GameObject *object);

	void addOverlappingObject(class GameObject *GO);
	void removeOverlappingObject(class GameObject *GO);
	bool checkOverlapping(class GameObject *other);
	std::vector <OverlappingWrapper> overlappedObjects;
	std::vector <sf::Vector2f> collisionPoints;
};

// GAMEOBJECTS ///////////////////////////////////////////////////////////////////////////////

class GameObject : public Observer
{
	friend struct Component;
	friend class Renderer;
	friend class Game;
public:
	GameObject();
	~GameObject();

	sf::Vector2f position;
	sf::Vector2f vel;

	bool active = true;

	virtual void update(class World *world, float dT);

	int getID() { return id; };

	void setPosition(sf::Vector2f pos) { position = pos; }

	PhysicsComponent *getPhysicsComponent()		{ return physicsComponent; }
	RenderComponent  *getRenderComponent()		{ return renderComponent;  }
	CollisionComponent *getCollisionComponent() { return collisionComponent; }

protected:
	int id;

	RenderComponent    *renderComponent    = nullptr;
	PhysicsComponent   *physicsComponent   = nullptr;
	CollisionComponent *collisionComponent = nullptr;

};


// PLAYER /////////////////////////////////////////////////////////////////////////////////////
class Player : public GameObject
{
public:
	Player(std::string name);

	void setName(std::string Name) { name = Name; };
	std::string getName() { return name; }
	void onNotify( GameObject *gameObject, int eventType, void *eventData);

private:
	std::string name;
	int health		= 100;
	int fullHealth	= 100;
};


// TESTOBJECT ////////////////////////////////////////////////////////////////////////////////
class TestObject : public GameObject
{
public:
	TestObject(class Renderer *renderer);
	~TestObject();
	void onNotify(GameObject *gameObject, int eventType, void *eventData);
	void update(class World *world, float dT);
};