#pragma once
#include <SFML/Graphics.hpp>
#include "Obspat.h"
#include "Utils.h"
#include "Command.h"

enum CollisionMode { IGNORE_COLLISION, CIRCLE_COLLISION, BOX_COLLISION };
enum RenderShape   { CIRCLE, RECTANGLE };
enum ObjectType	   { PLAYER, CANNONBALL, NONE };

struct OverlappingWrapper
{
	float time = 0.0f;
	class GameObject *object = nullptr;
};

struct InputData
{
	sf::Keyboard::Key key;
	class Command *command;
};

struct Graphics
{
	int textureID = 0;
	float angle = 0.0f;
	int type = 0;
	bool center = true;

	int shape = CIRCLE;
	float outlineThickness = 1.0f;
	float radius = 0.0f;
	Rectangle<float> rect;


	sf::Color fillColor;
	sf::Color outlineColor;
	sf::Sprite sprite;
	sf::VertexArray vertices;
	sf::Vector2f offset;
	sf::Vector2f origin;
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

	float time = 0.0f;
	float timePerFrame = 0.5f;
	int numAnimFrames = 0;

	float animWidth, animHeight;

	int currentFrame=0;

	bool loop = false;

	class Renderer *renderer = nullptr;

	std::vector <Graphics *> graphics;
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

	sf::Vector2f oldPosition;
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

struct InputComponent : public Component
{
	~InputComponent();

	InputComponent(class GameObject *Parent);
	void pushCommand(sf::Keyboard::Key Key, class Command *command);

	std::vector <InputData> commands;
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
	bool falling = true;
	ObjectType getType() { return type; }

protected:
	int id;
	ObjectType type = NONE;

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

//	bool falling = true;
	void shoot(class World *world, float dT);

	void moveCannonAngleUp(float dT);
	void moveCannonAngleDown(float dT);
	float getCannonAngle() { return cannonAngle; }

	InputComponent *getInputComponent() { return inputComponent; }
	void takeDamage(GameObject *from, float amount);

private:
	std::string name;
	float playerAngle	= 0.0f; // 180 or 0 degrees, debends on which side on screen he is
	float health		= 100.0f; 
	float fullHealth	= 100.0f;

	float cannonAngle = 0.0f;
	float cannonPower = 15.0f;
	float cannonAngleSpeed = 100.0f;
	float fireRate    = 1.0f;
	float loadingTime = 20.0f;

	Graphics *cannon = nullptr;
	InputComponent *inputComponent;
};


// CANNONBALL //////////////////////////////////////////////////////////////////////////////////
class CannonBall : public GameObject
{
public:
	CannonBall();
	~CannonBall();
	void onNotify(GameObject *gameObject, int eventType, void *eventData);
	void update(class World *world, float dT);
	void processCollision(CollisionData *collisionData);

private:
	float explosionRadius = 50.0f;
	float size = 4;
	float damage = 20.0f;
};


// EXPLOSIONS //////////////////////////////////////////////////////////////////////////////////
class Explosion : public GameObject
{
public:
	Explosion();
	~Explosion();

	void onNotify(GameObject *gameObject, int evenType, void *eventData);
	void update(class World *world, float dT);

private:

};

class Explosion2 : public GameObject
{
public:
	Explosion2();
	~Explosion2();

	void onNotify(GameObject *gameObject, int evenType, void *eventData) {};
	void update(class World *world, float dT) {};

private:

};