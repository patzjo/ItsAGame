#pragma once
#include <SFML/Graphics.hpp>
#include "Obspat.h"


struct Component
{
	friend class GameObject;
	Component(class GameObject *Parent) { parent = Parent; }
	class GameObject *parent;

	virtual void update() {};
};

struct RenderComponent : Component
{
	float time = 0.0f;
	float timePerFrame = 0.5f;

	int anim[10]; // Sprite id

	sf::Sprite sprite;

	RenderComponent(class GameObject *Parent, class Renderer *rendererPointer);
	~RenderComponent();
	class Renderer *renderer;
};

struct PhysicsComponent : Component
{
	sf::Vector2f acc;
	sf::Vector2f vel;

	void update( float dT );
	PhysicsComponent(class GameObject *Parent);

};

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

protected:
	int id;
};



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

class TestObject : public GameObject
{
public:
	TestObject(class Renderer *renderer);
	~TestObject();
	void onNotify(GameObject *gameObject, int eventType, void *eventData);
	void update(float dT);
protected:
	RenderComponent  *renderComponent;
	PhysicsComponent *physicsComponent;
};