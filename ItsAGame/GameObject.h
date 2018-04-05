#pragma once
#include <SFML/Graphics.hpp>
#include "Obspat.h"
#include "Utils.h"
enum CollisionMode { IGNORE_COLLISION, CIRCLE_COLLISION, BOX_COLLISION };

// COMPONENTS ////////////////////////////////////////////////////////////////////////////
struct Component
{
	friend class GameObject;
	Component(class GameObject *Parent) { parent = Parent; }
	class GameObject *parent;

	virtual void update() {};
};

struct RenderComponent : public Component
{
	float time = 0.0f;
	float timePerFrame = 0.5f;

	int numAnimFrames = 0;
	int anim[10]; // Sprite ids

	RenderComponent(class GameObject *Parent, class Renderer *rendererPointer);
	~RenderComponent();
	class Renderer *renderer;
};

struct PhysicsComponent : public Component
{
	sf::Vector2f acc;
	sf::Vector2f vel;

	Rectangle<float> collisionArea;
	float circleCollisionRadius;
	
	int collisionMode = BOX_COLLISION;

	void update( float dT );
	
	
	PhysicsComponent(class GameObject *Parent);

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
	virtual void update(float dT);

	int getID() { return id; };

	void setPosition(sf::Vector2f pos) { position = pos; }

	PhysicsComponent *getPhysicsComponent() { return physicsComponent; }
	RenderComponent  *getRenderComponent() { return renderComponent;  }

protected:
	int id;

	RenderComponent  *renderComponent = nullptr;
	PhysicsComponent *physicsComponent = nullptr;

};

// PLAYER /////////////////////////////////////////////////////////////////////////////////////
class Player : public GameObject
{
public:
	void setName(std::string Name) { name = Name; };
	std::string getName() { return name; }
	void onNotify( GameObject *gameObject, int eventType, void *eventData);
	void checkCollision(GameObject *other);
	

private:
	std::string name;
};


// TESTOBJECT ////////////////////////////////////////////////////////////////////////////////
class TestObject : public GameObject
{
public:
	TestObject(class Renderer *renderer);
	~TestObject();
	void onNotify(GameObject *gameObject, int eventType, void *eventData);
	void update(float dT);
};