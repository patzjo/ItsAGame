#pragma once
#include <SFML/Graphics.hpp>
#include "Obspat.h"
#include "Utils.h"
enum CollisionMode { IGNORE_COLLISION, CIRCLE_COLLISION, BOX_COLLISION };
enum RenderShape   { CIRCLE, RECTANGLE };

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
	RenderComponent(class GameObject *Parent);
	~RenderComponent();

	int type = 0; 

	float time = 0.0f;
	float timePerFrame = 0.5f;
	int numAnimFrames = 0;
	int textureID = 0;
	int currentFrame=0;
	class Renderer *renderer = nullptr;
	int shape = CIRCLE;
	sf::Color fillColor;
	sf::Color outlineColor;
	float outlineThickness = 1.0f;
	
	float radius;

	Rectangle<float> rect;

	sf::Sprite sprite;
};

struct PhysicsComponent : public Component
{
	PhysicsComponent(class GameObject *Parent);

	sf::Vector2f acc;
	virtual void update(class World *world, float dT );
};

struct PlayerPhysicsComponent : public PhysicsComponent
{
	PlayerPhysicsComponent(class GameObject *parent) : PhysicsComponent(parent) {};
	void update(class World *world, float dT);
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

	void addCollisionPoint(sf::Vector2f point);	// Points to check for "better" collision
	void increaseOverlapTime(class GameObject *GO, float dT);


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
	void setOwner(class GameObject *Owner) { owner = Owner; }
	class GameObject *getOwner() { return owner; }



	PhysicsComponent   *getPhysicsComponent()	{ return physicsComponent; }
	RenderComponent    *getRenderComponent()	{ return renderComponent;  }
	CollisionComponent *getCollisionComponent() { return collisionComponent; }

protected:
	int id;

	RenderComponent    *renderComponent    = nullptr;
	PhysicsComponent   *physicsComponent   = nullptr;
	CollisionComponent *collisionComponent = nullptr;

	class GameObject *owner = nullptr; // In case this item is owned by other gameObject.

};


// PLAYER /////////////////////////////////////////////////////////////////////////////////////
class Player : public GameObject
{
public:
	Player(std::string name);

	void setName(std::string Name) { name = Name; };
	std::string getName() { return name; }
	void onNotify( GameObject *gameObject, int eventType, void *eventData);

	void update(class World *world, float dT);
	void handleLevelCollision(class Level *level);

	bool inGround = false;
	void shoot(class World *world, float dT);

	void moveCannonAngleUp(float dT);
	void moveCannonAngleDown(float dT);

	float getCannonAngle() { return cannonAngle; }

private:
	std::string name;
	int health		= 100;
	int fullHealth	= 100;

	float cannonAngle = 0.0f;
	float cannonPower = 15.0f;
	float cannonAngleSpeed = 100.0f;

	RenderComponent cannon;
};


// CANNONBALL //////////////////////////////////////////////////////////////////////////////////
class CannonBall : public GameObject
{
public:
	CannonBall();
	~CannonBall();
	void onNotify(GameObject *gameObject, int eventType, void *eventData);
	void update(class World *world, float dT);
};